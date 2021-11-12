/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "box.hpp"
#include <dci/sbs/subscription.hpp>
#include <dci/sbs/box.hpp>
#include <dci/himpl/impl2Face.hpp>
#include <dci/utils/atScopeExit.hpp>
#include <dci/utils/dbg.hpp>

namespace dci::sbs::impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Box::Box()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Box::~Box()
    {
        while(_first)
        {
            removeAndDelete(_first);
        }
        dbgAssert(!_first);
        dbgAssert(!_last);

        _enumerators.flush([](Enumerator* enumerator)
        {
            enumerator->_nextSubscription = nullptr;
            enumerator->_lastSubscription = nullptr;
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Box::empty() const
    {
        dbgAssert(!_first == !_last);
        return !_first;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Box::push(Subscription* subscription)
    {
        if(!_first)
        {
            dbgAssert(!_last);

            subscription->_box = this;
            subscription->_nextInBox = subscription->_prevInBox = nullptr;

            _first = _last = subscription;
        }
        else
        {
            dbgAssert(_last);
            dbgAssert(!_first->_prevInBox);
            dbgAssert(!_last->_nextInBox);

            subscription->_box = this;
            subscription->_nextInBox = nullptr;
            subscription->_prevInBox = _last;

            _last->_nextInBox = subscription;
            _last = subscription;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Subscription* Box::remove(Subscription* subscription)
    {
        dbgAssert(this == subscription->_box);

        {
            _enumerators.each([&](Enumerator* enumerator)
            {
                if(subscription == enumerator->_nextSubscription)
                {
                    if(enumerator->_nextSubscription == enumerator->_lastSubscription)
                    {
                        enumerator->_nextSubscription = enumerator->_lastSubscription = nullptr;
                    }
                    else
                    {
                        enumerator->_nextSubscription = enumerator->_nextSubscription->_nextInBox;
                    }
                }
                else if(subscription == enumerator->_lastSubscription)
                {
                    enumerator->_lastSubscription = enumerator->_lastSubscription->_prevInBox;
                }
            });
        }

        if(_first == subscription)
        {
            dbgAssert(!subscription->_prevInBox);

            if(subscription->_nextInBox)
            {
                dbgAssert(_last != subscription);
                dbgAssert(subscription->_nextInBox->_prevInBox == subscription);

                subscription->_nextInBox->_prevInBox = nullptr;
                _first = subscription->_nextInBox;
                subscription->_nextInBox = nullptr;
            }
            else
            {
                dbgAssert(_last == subscription);

                _first = _last = nullptr;
            }
        }
        else if(_last == subscription)
        {
            dbgAssert(!subscription->_nextInBox);
            dbgAssert(subscription->_prevInBox);
            dbgAssert(subscription->_prevInBox->_nextInBox == subscription);

            subscription->_prevInBox->_nextInBox = nullptr;
            _last = subscription->_prevInBox;
            subscription->_prevInBox = nullptr;
        }
        else
        {
            dbgAssert(subscription->_prevInBox);
            dbgAssert(subscription->_prevInBox->_nextInBox == subscription);

            dbgAssert(subscription->_nextInBox);
            dbgAssert(subscription->_nextInBox->_prevInBox == subscription);

            subscription->_prevInBox->_nextInBox = subscription->_nextInBox;
            subscription->_nextInBox->_prevInBox = subscription->_prevInBox;

            subscription->_prevInBox = nullptr;
            subscription->_nextInBox = nullptr;
        }

        subscription->_box = nullptr;
        return subscription;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Box::removeAndDelete(Subscription* subscription)
    {
        subscription->removeSelf();
        subscription->activate(nullptr, 0, true);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Box::removeAndDeleteAll()
    {
        while(_first)
        {
            removeAndDelete(_first);
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Box::activate(void* context, std::uint_fast8_t flags)
    {
        bool flagDel = flags & sbs::Box::del;

        if(_first == _last)
        {
            if(_first)
            {
                if(flagDel)
                {
                    Subscription* s = _first;
                    s->removeSelf();
                    dbgAssert(!_first && !_last);
                    s->activate(context, sbs::Subscription::act4Last, true);
                }
                else
                {
                    _first->activate(context, sbs::Subscription::act4Last, false);
                }
                return;
            }
            return;
        }

        Enumerator enumerator {_first, _last};
        _enumerators.push(&enumerator);

        {

            utils::AtScopeExit cleaner{[&]
            {
                if(enumerator._next)
                {
                    dbgAssert(enumerator._prev);
                    _enumerators.remove(&enumerator);
                }
            }};

            while(enumerator._nextSubscription != enumerator._lastSubscription)
            {
                Subscription* cur = enumerator._nextSubscription;

                if(flagDel)
                {
                    cur->removeSelf();
                    dbgAssert(cur != enumerator._nextSubscription);
                }
                else
                {
                    enumerator._nextSubscription = enumerator._nextSubscription->_nextInBox;
                }

                cur->activate(context, sbs::Subscription::act4Next, flagDel);
            }
        }

        if(enumerator._nextSubscription)
        {
            dbgAssert(enumerator._nextSubscription == enumerator._lastSubscription);

            Subscription* cur = enumerator._nextSubscription;
            if(flagDel)
            {
                cur->removeSelf();
            }

            cur->activate(context, sbs::Subscription::act4Last, flagDel);
        }
    }
}
