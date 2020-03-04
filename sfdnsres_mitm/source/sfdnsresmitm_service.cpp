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
#include <switch.h>

extern FILE* g_logging_file;

ams::Result ams::mitm::sfdnsres::SfdnsresMitmService::GetAddrInfo(int arg,
                                                                  int timeout,
                                                                  sf::ClientProcessId pid,
                                                                  u64 pid_placeholder,
                                                                  sf::InBuffer node,
                                                                  sf::InBuffer service,
                                                                  sf::InBuffer hints_serialized,
                                                                  sf::OutBuffer res_serialized)
{
    fprintf(g_logging_file, "GetAddrInfo(); // Resolving\n");
    fflush(g_logging_file);

    return 0;
}