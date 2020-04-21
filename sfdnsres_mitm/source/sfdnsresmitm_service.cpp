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
#include "debug.hpp"
#include "sys/socket.h"
#include "utils.hpp"

#include <string>
#include <switch.h>
#ifdef __cplusplus
extern "C"
{
#endif
#include <switch/services/sfdnsres.h>
#ifdef __cplusplus
}
#endif

namespace ams::mitm::sfdnsres::util
{
    bool getAddr(std::string name, sockaddr_in& addr)
    {
        if (globalHostMap.count(name) > 0)
        {
            addr = globalHostMap.at(name);
            return true;
        }
        return false;
    };
} // namespace ams::mitm::sfdnsres::util

namespace ams::mitm::sfdnsres
{
    Result SfdnsresMitmService::GetAddrInfoRequest(u32 cancel_handle,
                                                   const sf::ClientProcessId& client_pid,
                                                   bool use_nsd_resolve,
                                                   const sf::InBuffer& host,
                                                   const sf::InBuffer& service,
                                                   const sf::InBuffer& hints,
                                                   const sf::OutBuffer& out_addr_infos,
                                                   sf::Out<u32> out_errno,
                                                   sf::Out<s32> out_ret,
                                                   sf::Out<u32> out_buf_len)
    {
        Result res;
        sts::debug::DebugLog("Host: %s\n", reinterpret_cast<const char*>(host.GetPointer()));
        sts::debug::DebugLog("Service: %s\n", reinterpret_cast<const char*>(service.GetPointer()));
        sts::debug::DebugLog("Hints: 0x%x\n", reinterpret_cast<const char*>(hints.GetPointer()));
        sts::debug::DebugLog("out_buf_len: %d\n", out_buf_len.GetValue());
        sts::debug::DebugLog("Cancel Handle: %d\n", cancel_handle);
        u32 ownCancelHandle = 0;
        res = sfdnsresGetCancelHandleRequest(&ownCancelHandle);
        sockaddr_in addr;
        if (ams::mitm::sfdnsres::util::getAddr(std::string(reinterpret_cast<const char*>(host.GetPointer())), addr))
        {
            ams::mitm::sfdnsres::util::AddrInfo addrInfo;
            sts::debug::DebugLog("Access to %s\n logged", reinterpret_cast<const char*>(host.GetPointer()));
        }
        sts::debug::DebugLog("Result CancelHandleRequest: 0x%X (2%03d-%04d)\n", res.GetValue(), res.GetModule(), res.GetDescription());
        sts::debug::DebugLog("Own Cancel Handle: %d\n\n", ownCancelHandle);

        res = sfdnsresGetAddrInfoRequest(ownCancelHandle,
                                         use_nsd_resolve,
                                         reinterpret_cast<const char*>(host.GetPointer()),
                                         reinterpret_cast<const char*>(service.GetPointer()),
                                         reinterpret_cast<const void*>(hints.GetPointer()),
                                         hints.GetSize(),
                                         reinterpret_cast<void*>(out_addr_infos.GetPointer()),
                                         out_addr_infos.GetSize(),
                                         out_errno.GetPointer(),
                                         out_ret.GetPointer(),
                                         out_buf_len.GetPointer());

        sts::debug::DebugLog("Result GetAddrInfoRequest: 0x%X (2%03d-%04d)\n", res.GetValue(), res.GetModule(), res.GetDescription());
        sts::debug::DebugLog("out_addr_infos.GetSize: %d\n", out_addr_infos.GetSize());
        sts::debug::DebugLog("Map Size: %d\n", ams::mitm::sfdnsres::util::globalHostMap.size());
        //sts::debug::DebugLog("out_errno: %d\n", out_buf_len.GetValue());
        //sts::debug::DebugLog("out_buf_len: %d\n", out_buf_len.GetValue());
        exosphere::ForceRebootToRcm();
        return res;
    }

} // namespace ams::mitm::sfdnsres