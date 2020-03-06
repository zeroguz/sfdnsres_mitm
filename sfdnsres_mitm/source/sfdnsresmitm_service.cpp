/*
 * Copyright (c) 2019 znxDomain
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sfdnsresmitm_service.hpp"
#include <switch.h>

ams::Result ams::mitm::sfdnsres::SfdnsresMitmService::GetAddrInfo(sf::Out<s32> ret,
                                                                  sf::Out<u32> bsd_errno,
                                                                  sf::Out<u32> packed_addrinfo_size,
                                                                  sf::OutBuffer response,
                                                                  u32 enable_nsd_resolve,
                                                                  u64 pid_placeholder,
                                                                  // pid?
                                                                  sf::InBuffer host,
                                                                  sf::InBuffer service,
                                                                  sf::InBuffer hints)
{
    u32 cancel_handle = 0;
    size_t hints_size = 0;
    void* out_buffer = NULL;
    size_t out_buffer_size = 0;
    u32* response_serialized_size = NULL;
    sfdnsresGetAddrInfoRequest(cancel_handle, enable_nsd_resolve, reinterpret_cast<const char*>(host.GetPointer()), reinterpret_cast<const char*>(service.GetPointer()), reinterpret_cast<const char*>(hints.GetPointer()), hints_size, out_buffer, out_buffer_size, bsd_errno.GetPointer(), reinterpret_cast<s32*>(response.GetPointer()), response_serialized_size);
    return ams::ResultSuccess();
}
