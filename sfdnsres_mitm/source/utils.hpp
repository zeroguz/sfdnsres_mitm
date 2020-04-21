#pragma once
#include <map>
#include <netinet/in.h>
#include <stratosphere.hpp>
#include <string>
namespace ams::mitm::sfdnsres::util
{
    extern std::map<std::string, sockaddr_in> globalHostMap;

    struct AddrInfo
    {
        u32 magic;
        u32 ai_flags;
        u32 ai_family;
        u32 ai_socktype;
        u32 ai_protocol;
        u32 ai_addrlen;
    };

} // namespace ams::mitm::sfdnsres::util