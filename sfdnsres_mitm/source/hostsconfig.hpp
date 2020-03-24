#pragma once
#include <map>
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
        u32 getAddr(std::string name)
        {
            return g_hosts[name];
        };

    private:
        std::map<std::string, u32> g_hosts;
    };
} // namespace ams::mitm::sfdnsres::config