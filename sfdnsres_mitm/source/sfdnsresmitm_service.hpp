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

namespace ams::mitm::sfdnsres
{

    class SfdnsresMitmService : public sf::IMitmServiceObject
    {
    private:
        enum class CommandId
        {
            GetHostByNameRequest = 2,
            GetAddrInfoRequest = 6,
            GetHostByNameRequestWithOptions = 10,
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
                                             const sf::ClientProcessId& client_pid,
                                             bool use_nsd,
                                             const sf::InBuffer& host,
                                             const sf::InBuffer& service,
                                             const sf::InBuffer& hints,
                                             const sf::OutAutoSelectBuffer& buf2,
                                             const sf::InAutoSelectBuffer& buf3,
                                             sf::Out<u32> h_errno,
                                             sf::Out<u32> out_errno,
                                             sf::Out<u32> serialized_size);

        Result GetHostByNameRequestWithOptions(u32 cancel_handle,
                                               const sf::ClientProcessId& client_pid,
                                               bool use_nsd,
                                               const sf::InAutoSelectBuffer& buf1,
                                               const sf::OutAutoSelectBuffer& buf2,
                                               const sf::InAutoSelectBuffer& buf3,
                                               sf::Out<u32> h_errno,
                                               sf::Out<u32> out_errno,
                                               sf::Out<u32> serialized_size);

        Result GetHostByNameRequest(u32 cancel_handle,
                                    const sf::ClientProcessId& client_pid,
                                    bool use_nsd_resolve,
                                    const sf::InBuffer& name,
                                    const sf::OutBuffer& out_addr_infos,
                                    sf::Out<u32> out_errno,
                                    sf::Out<u32> h_errno,
                                    sf::Out<u32> out_buf_len);

        Result GetAddrInfoRequest(u32 cancel_handle,
                                  const sf::ClientProcessId& client_pid,
                                  bool use_nsd_resolve,
                                  const sf::InBuffer& host,
                                  const sf::InBuffer& service,
                                  const sf::InBuffer& hints,
                                  const sf::OutBuffer& out_addr_infos,
                                  sf::Out<u32> out_errno,
                                  sf::Out<s32> out_ret,
                                  sf::Out<u32> out_buf_len);

    public:
        DEFINE_SERVICE_DISPATCH_TABLE{
            //MAKE_SERVICE_COMMAND_META(GetHostByNameRequest),
            MAKE_SERVICE_COMMAND_META(GetAddrInfoRequest),
            MAKE_SERVICE_COMMAND_META(GetHostByNameRequestWithOptions),
            MAKE_SERVICE_COMMAND_META(GetAddrInfoRequestWithOptions),
        };
    };

} // namespace ams::mitm::sfdnsres