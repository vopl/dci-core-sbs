/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/himpl.hpp>
#include <dci/sbs/implMetaInfo.hpp>
#include "api.hpp"

namespace dci::sbs
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class API_DCI_SBS Owner
        : public himpl::FaceLayout<Owner, impl::Owner>
    {
        Owner(const Owner&) = delete;
        void operator=(const Owner&) = delete;

    public:
        Owner();
        ~Owner();

        void flush();
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class F>
    struct OwnedFunctor
    {
        Owner&  _owner;
        F&&     _f;

        OwnedFunctor(Owner& owner, F&& f)
            : _owner{owner}
            , _f{std::forward<F>(f)}
        {}

        OwnedFunctor(const OwnedFunctor&) = delete;
        OwnedFunctor(OwnedFunctor&&) = delete;

        void operator=(const OwnedFunctor&) = delete;
        void operator=(OwnedFunctor&&) = delete;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class F>
    OwnedFunctor<F> operator*(Owner& owner, F&& f)
    {
        return {owner, std::forward<F>(f)};
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class F>
    OwnedFunctor<F> operator*(Owner* owner, F&& f)
    {
        return {*owner, std::forward<F>(f)};
    }
}
