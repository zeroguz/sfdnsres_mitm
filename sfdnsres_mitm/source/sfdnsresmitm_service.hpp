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
};

namespace ams::mitm::sfdnsres
{

    class SfdnsresMitmService : public sf::IMitmServiceObject
    {
    private:
        enum class CommandId
        {
            GetAddrInfo = 6
        };

    public:
        SF_MITM_SERVICE_OBJECT_CTOR(SfdnsresMitmService)
        { /* ... */
        }
        static bool ShouldMitm(const sm::MitmProcessInfo& client_info)
        {
            return true;
        }

    public:
    protected:
        /* Overridden commands. */
        Result GetAddrInfo(sf::Out<s32> ret,
                           sf::Out<u32> bsd_errno,
                           sf::Out<u32> packed_addrinfo_size,
                           sf::OutBuffer response,
                           u32 enable_nsd_resolve,
                           u64 pid_placeholder,
                           // pid?
                           sf::InBuffer host,
                           sf::InBuffer service,
                           sf::InBuffer hints);

    public:
        DEFINE_SERVICE_DISPATCH_TABLE{
            MAKE_SERVICE_COMMAND_META(GetAddrInfo),
        };
    };

} // namespace ams::mitm::sfdnsres