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
#include <stratosphere.hpp>
#include <switch.h>

#include "debug.hpp"
#include "sfdnsresmitm_service.hpp"
#include "utils.hpp"

#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>

namespace ams::mitm::sfdnsres::util
{
    std::map<std::string, sockaddr_in> globalHostMap;
}

extern "C"
{
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;
    u32 __nx_fs_num_sessions = 1;
    u32 __nx_fsdev_direntry_cache_size = 1;

#define INNER_HEAP_SIZE 0x50000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char nx_inner_heap[INNER_HEAP_SIZE];

    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);

    /* Exception handling. */
    alignas(16) u8 __nx_exception_stack[ams::os::MemoryPageSize];
    u64 __nx_exception_stack_size = sizeof(__nx_exception_stack);
    void __libnx_exception_handler(ThreadExceptionDump* ctx);
}

namespace ams
{

    ncm::ProgramId CurrentProgramId = {0x4206900000000012ul};

    namespace result
    {

        bool CallFatalOnResultAssertion = false;
    }

} // namespace ams

using namespace ams;

void __libnx_exception_handler(ThreadExceptionDump* ctx)
{
    ams::CrashHandler(ctx);
}

void __libnx_initheap(void)
{
    void* addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;

    /* Newlib */
    extern char* fake_heap_start;
    extern char* fake_heap_end;

    fake_heap_start = (char*)addr;
    fake_heap_end = (char*)addr + size;
}

void __appInit(void)
{
    hos::SetVersionForLibnx();

    sm::DoWithSession([&]() {
        R_ABORT_UNLESS(smInitialize());
        R_ABORT_UNLESS(fsInitialize());
        R_ABORT_UNLESS(pmdmntInitialize());
        R_ABORT_UNLESS(pminfoInitialize());
        R_ABORT_UNLESS(splFsInitialize());
    });
}

void __appExit(void)
{
    /* Cleanup services. */
    splFsExit();
    pminfoExit();
    pmdmntExit();
    fsExit();
    smExit();
}

struct SfdnsresManagerOptions
{
    static const size_t PointerBufferSize = 0x100;
    static const size_t MaxDomains = 4;
    static const size_t MaxDomainObjects = 0x100;
};

int main(int argc, char** argv)
{
    sts::debug::Initialize();

    fsdevMountSdmc();

    std::ifstream hostfile = std::ifstream("/sfdnsres_mitm/hosts");
    std::string line;
    sts::debug::DebugLog("Open?: %d\n", hostfile.is_open());
    while (std::getline(hostfile, line))
    {
        sts::debug::DebugLog("Line: %s\n", line.c_str());
        if (!(line.at(0) == '#'))
        {
            std::istringstream lineStream = std::istringstream(line);
            std::string ipString;
            std::string name;
            sockaddr_in ip;
            lineStream >> ipString;
            if (!(inet_pton(AF_INET, ipString.c_str(), &(ip.sin_addr))))
            {
                if (!(inet_pton(AF_INET6, ipString.c_str(), &(ip.sin_addr))))
                {
                    continue;
                }
            }
            while (lineStream >> name)
            {
                ams::mitm::sfdnsres::util::globalHostMap.emplace(name, ip);
                sts::debug::DebugLog("Added: %s\n", name.c_str());
            }
        }
    }

    constexpr sm::ServiceName MitmServiceName = sm::ServiceName::Encode("sfdnsres");
    sf::hipc::ServerManager<2, SfdnsresManagerOptions, 4> server_manager;

    R_ASSERT(server_manager.RegisterMitmServer<ams::mitm::sfdnsres::SfdnsresMitmService>(MitmServiceName));

    server_manager.LoopProcess();

    return 0;
}
