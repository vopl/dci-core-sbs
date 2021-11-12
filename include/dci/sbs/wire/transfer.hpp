/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "transfer/arg.hpp"
#include "transfer/ret.hpp"
#include "agg.hpp"

namespace dci::sbs::wire
{
    struct TransferBase
    {
        Agg _agg;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class R, Agg agg, class... Args>
    struct Transfer
        : TransferBase
    {
        transfer::Ret<R, agg>               _ret;
        std::tuple<transfer::Arg<Args>...>  _args;

        template <class... RawArgs>
        Transfer(RawArgs&&... rawArgs)
            : TransferBase{agg}
            , _args(std::forward<RawArgs>(rawArgs)...)
        {
        }
    };
}
