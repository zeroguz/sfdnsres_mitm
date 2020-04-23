/*
 * Copyright (c) 2018-2020 Atmosphère-NX
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

#pragma once
#include <stratosphere.hpp>
#include <switch.h>

enum SfdnsresCmd : u32
{
    // All cmds:
    // SfdnsresCmd_SetDnsAddressesPrivate = 0,
    // SfdnsresCmd_GetDnsAddressPrivate = 1,
    // SfdnsresCmd_GetHostByName = 2,
    // SfdnsresCmd_GetHostByAddr = 3,
    // SfdnsresCmd_GetHostStringError = 4,
    // SfdnsresCmd_GetGaiStringError = 5,
    SfdnsresCmd_GetAddrInfo = 6,
    // SfdnsresCmd_GetNameInfo = 7,
    // SfdnsresCmd_RequestCancelHandle = 8,
    // SfdnsresCmd_CancelSocketCall = 9,
    // SfdnsresCmd_? = 10,
    // SfdnsresCmd_ClearDnsIpServerAddressArray = 11,
    SfdnsresCmd_GetAddrInfoWithOptions = 12,
};

namespace ams::mitm::sfdnsres
{

    class SfdnsresMitmService : public sf::IMitmServiceObject
    {
    private:
        enum class CommandId
        {
            GetAddrInfoRequest = 6,
            GetAddrInfoRequestWithOptions = 12
        };

    public:
        SF_MITM_SERVICE_OBJECT_CTOR(SfdnsresMitmService)
        { /* ... */
        }
        static bool ShouldMitm(const sm::MitmProcessInfo& client_info)
        {
            return true;
        }

    protected:
        /* Overridden commands. */
        Result GetAddrInfoRequestWithOptions(u32 cancel_handle,
                                             sf::InBuffer const& host,
                                             sf::InBuffer const& service,
                                             sf::InBuffer const& hints,
                                             sf::OutBuffer const& out_addr_infos,
                                             sf::Out<unsigned int, void> u1,
                                             sf::Out<int, void> u2,
                                             unsigned int u3,
                                             sf::InBuffer const& u4,
                                             unsigned int u5,
                                             sf::Out<int, void> u6,
                                             sf::Out<int, void> u7);
        Result GetAddrInfoRequest(u32 cancel_handle, const sf::ClientProcessId& client_pid, bool use_nsd_resolve, const sf::InBuffer& host, const sf::InBuffer& service, const sf::InBuffer& hints, const sf::OutBuffer& out_addr_infos, sf::Out<u32> out_errno, sf::Out<s32> out_ret, sf::Out<u32> out_buf_len);

    public:
        DEFINE_SERVICE_DISPATCH_TABLE{
            MAKE_SERVICE_COMMAND_META(GetAddrInfoRequest),
            MAKE_SERVICE_COMMAND_META(GetAddrInfoRequestWithOptions),
        };
    };

} // namespace ams::mitm::sfdnsres