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

#include "sfdnsresmitm_service.hpp"

extern "C"
{
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;
    u32 __nx_fs_num_sessions = 1;
    u32 __nx_fsdev_direntry_cache_size = 1;

#define INNER_HEAP_SIZE 0x1000000
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

    ncm::ProgramId CurrentProgramId = {0x4200000000000010ul};

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

#define SOCK_BUFFERSIZE 0x1000
    const SocketInitConfig socketInitConfig = {
        .bsdsockets_version = 1,

        .tcp_tx_buf_size = 0x800,
        .tcp_rx_buf_size = 0x1000,
        .tcp_tx_buf_max_size = 0x2000,
        .tcp_rx_buf_max_size = 0x2000,

        .udp_tx_buf_size = 0x2000,
        .udp_rx_buf_size = 0x2000,

        .sb_efficiency = 4,

        .num_bsd_sessions = 3,
        .bsd_service_type = BsdServiceType_User,
    };
    hos::SetVersionForLibnx();

    sm::DoWithSession([&]() {
        R_ABORT_UNLESS(fsInitialize());
        // R_ABORT_UNLESS(pmdmntInitialize());
        // R_ABORT_UNLESS(pminfoInitialize());
        // R_ABORT_UNLESS(splFsInitialize());
        R_ABORT_UNLESS(socketInitialize(&socketInitConfig));
    });

    ams::CheckApiVersion();
}

void __appExit(void)
{
    /* Cleanup services. */
    socketExit();
    //splFsExit();
    //pminfoExit();
    //pmdmntExit();
    fsExit();
}

FILE* g_logging_file = nullptr;

struct SfdnsresManagerOptions
{
    static const size_t PointerBufferSize = 0x100;
    static const size_t MaxDomains = 4;
    static const size_t MaxDomainObjects = 0x100;
};

int main(int argc, char** argv)
{
    g_logging_file = fopen("sfdnsres.log", "a+");

    constexpr sm::ServiceName MitmServiceName = sm::ServiceName::Encode("sfdnsres");
    sf::hipc::ServerManager<2, SfdnsresManagerOptions, 4> server_manager;

    R_ASSERT(server_manager.RegisterMitmServer<ams::mitm::sfdnsres::SfdnsresMitmService>(MitmServiceName));

    server_manager.LoopProcess();
    fflush(g_logging_file);

    fclose(g_logging_file);
    return 0;
}
