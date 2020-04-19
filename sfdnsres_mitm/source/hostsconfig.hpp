#pragma once
#include <map>
#include <netinet/in.h>
#include <stratosphere.hpp>
#include <string>
#include <switch.h>

namespace ams::mitm::sfdnsres::config
{
    class Hosts
    {
    public:
        Hosts();
        ~Hosts();
        sockaddr_in getAddr(std::string name)
        {
            return gHosts[name];
        };

    private:
        std::map<std::string, sockaddr_in> gHosts;
    };
} // namespace ams::mitm::sfdnsres::config