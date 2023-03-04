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
#include "subscription.hpp"

namespace dci::sbs
{
    class API_DCI_SBS Box
        : public himpl::FaceLayout<Box, impl::Box>
    {
        Box(const Box&) = delete;
        void operator=(const Box&) = delete;

    public:
        enum Flags
        {
            //actFirst    = 0x21,
            //actAll      = 0x22,
            del         = 0x30,
        };

    public:
        Box();
        ~Box();

        bool empty() const;
        void push(Subscription* subscription);
        void removeAndDelete(Subscription* subscription);
        void removeAndDeleteAll();
        void activate(void* context = nullptr, std::uint_fast8_t flags = 0);
    };
}
