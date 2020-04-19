#include "hostsconfig.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>

namespace ams::mitm::sfdnsres::config
{
    Hosts::Hosts()
    {
        std::ifstream hostfile = std::ifstream("sdmc:/sfdnsres_mitm/hosts");
        std::string line;
        while (std::getline(hostfile, line))
        {
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

                    while (lineStream >> name)
                    {
                        gHosts.emplace(name, ip);
                    }
                }
            }
        }
        return;
    }
    Hosts::~Hosts()
    {
        return;
    }
} // namespace ams::mitm::sfdnsres::config
