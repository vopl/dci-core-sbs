/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "subscription.hpp"
#include "owner.hpp"
#include "box.hpp"

#include <dci/sbs/subscription.hpp>
#include <dci/himpl/impl2Face.hpp>
#include <dci/utils/atScopeExit.hpp>
#include <dci/utils/dbg.hpp>

namespace dci::sbs::impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Subscription::Subscription(Activator activator, Owner* owner)
        : _activator(activator)
        , _owner(owner)
    {
        if(_owner)
        {
            _owner->push(this);
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Subscription::~Subscription()
    {
        removeSelf();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Subscription::removeSelf()
    {
        if(_owner)
        {
            _owner->remove(this);
        }
        dbgAssert(!_owner);
        dbgAssert(!_prevInOwner);
        dbgAssert(!_nextInOwner);

        if(_box)
        {
            _box->remove(this);
        }
        dbgAssert(!_box);
        dbgAssert(!_prevInBox);
        dbgAssert(!_nextInBox);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Subscription::activate(void* context, std::uint_fast8_t flags, bool deletionRequested)
    {
        dbgAssert(!(flags & sbs::Subscription::del));

        _deletionRequested |= deletionRequested;

        _activationDepth++;
        utils::AtScopeExit cleaner{[this]
        {
            dbgAssert(_activationDepth);
            _activationDepth--;

            if(!_activationDepth && _deletionRequested && !_deletionActivated)
            {
                _deletionActivated = true;
                _activator(himpl::impl2Face<sbs::Subscription>(this), nullptr, sbs::Subscription::del);
            }
        }};

        if(flags)
        {
            _activator(himpl::impl2Face<sbs::Subscription>(this), context, flags);
        }
    }

}
