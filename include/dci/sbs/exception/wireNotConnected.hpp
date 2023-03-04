/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/exception.hpp>

namespace dci::sbs::exception
{
    class WireNotConnected
        : public dci::exception::Skeleton<WireNotConnected, dci::Exception>
    {
    public:
        using dci::exception::Skeleton<WireNotConnected, dci::Exception>::Skeleton;

    public:
        static constexpr Eid _eid {0xd7,0x24,0x41,0xd0,0x0c,0x0c,0x41,0x44,0x87,0x44,0x1e,0xae,0x96,0x6c,0x30,0x60};
    };
}
