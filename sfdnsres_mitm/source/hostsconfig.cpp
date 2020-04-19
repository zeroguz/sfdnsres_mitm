#include "hostsconfig.hpp"

namespace ams::mitm::sfdnsres::config
{
    Hosts::Hosts()
    {
        FILE *f = fopen("sdmc:/sfdnsres_mitm", "r");
        return;
    }
    Hosts::~Hosts()
    {
        return;
    }
} // namespace ams::mitm::sfdnsres::config
