/**
 * \file main-snet-flooder.cpp
 * \brief 
 *
 * A set of tools to exchange and handle packets of s-net(r) devices via the HDLC Daemon.
 * Copyright (C) 2016  Florian Evers, florian-evers@gmx.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Config.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include "HdlcdClient.h"
#include "FloodingPacketSource.h"

int main(int argc, char* argv[]) {
    try {
        // Declare the supported options.
        boost::program_options::options_description l_Description("Allowed options");
        l_Description.add_options()
            ("help,h",    "produce this help message")
            ("version,v", "show version information")
            ("connect,c", boost::program_options::value<std::string>(),
                          "syntax: SerialPort@IPAddess:PortNbr\n"
                          "  linux:   /dev/ttyUSB0@localhost:5001\n"
                          "  windows: //./COM1@example.com:5001")
            ("destination,d", boost::program_options::value<std::string>(),
                              "the destination address (SSA) as a 16bit hex value, e.g., 3FF4")
        ;

        // Parse the command line
        boost::program_options::variables_map l_VariablesMap;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, l_Description), l_VariablesMap);
        boost::program_options::notify(l_VariablesMap);
        if (l_VariablesMap.count("version")) {
            std::cerr << "s-net(r) flooder tool version " << SNET_TOOLS_VERSION_MAJOR << "." << SNET_TOOLS_VERSION_MINOR
                      << " built with hdlcd-devel version " << HDLCD_DEVEL_VERSION_MAJOR << "." << HDLCD_DEVEL_VERSION_MINOR
                      << " and snet-devel version " << SNET_DEVEL_VERSION_MAJOR << "." << SNET_DEVEL_VERSION_MINOR << std::endl;
        } // if

        if (l_VariablesMap.count("help")) {
            std::cout << l_Description << std::endl;
            std::cout << "The s-net(r) packet flooder for the HDLC Daemon is Copyright (C) 2016, and GNU GPL'd, by Florian Evers." << std::endl;
            std::cout << "Bug reports, feedback, admiration, abuse, etc, to: https://github.com/Strunzdesign/snet-tools" << std::endl;
            return 1;
        } // if
        
        if (!l_VariablesMap.count("connect")) {
            std::cout << "snet-flooder: you have to specify one device to connect to" << std::endl;
            std::cout << "snet-flooder: Use --help for more information." << std::endl;
            return 1;
        } // if
        
        uint16_t l_UnicastSSA = 0x3FF4;
        if (!l_VariablesMap.count("destination")) {
            std::cout << "snet-flooder: you have to specify the destination address (SSA)" << std::endl;
            std::cout << "snet-flooder: Use --help for more information." << std::endl;
            return 1;
        } else {
            // Convert the provided hexadecimal SSA
            std::istringstream l_Converter(l_VariablesMap["destination"].as<std::string>());
            l_Converter >> std::hex >> l_UnicastSSA;
        } // else

        // Install signal handlers
        boost::asio::io_service l_IoService;
        boost::asio::signal_set l_Signals(l_IoService);
        l_Signals.add(SIGINT);
        l_Signals.add(SIGTERM);
        l_Signals.async_wait([&l_IoService](boost::system::error_code, int){ l_IoService.stop(); });
        
        // Parse the destination specifier
        static boost::regex s_RegEx("^(.*?)@(.*?):(.*?)$");
        boost::smatch l_Match;
        if (boost::regex_match(l_VariablesMap["connect"].as<std::string>(), l_Match, s_RegEx)) {
            // Resolve destination
            boost::asio::ip::tcp::resolver l_Resolver(l_IoService);
            auto l_EndpointIterator = l_Resolver.resolve({ l_Match[2], l_Match[3] });
            
            // Prepare the HDLCd client entity: 0x00 = Payload TX only, Ctrl RX and TX
            HdlcdClient l_HdlcdClient(l_IoService, l_Match[1], 0x00);
            l_HdlcdClient.SetOnClosedCallback([&l_IoService](){ l_IoService.stop(); });
            
            // Prepare input then connect
            FloodingPacketSource l_FloodingPacketSource(l_HdlcdClient, l_UnicastSSA);
            l_HdlcdClient.AsyncConnect(l_EndpointIterator, [&l_FloodingPacketSource, &l_Signals](bool a_bSuccess) {
                if (a_bSuccess) {
                    l_FloodingPacketSource.Start();
                } else {
                    std::cout << "Failed to connect to the HDLC Daemon!" << std::endl;
                    l_Signals.cancel();
                } // else
            }); // AsyncConnect

            // Start event processing
            l_IoService.run();
        } else {
            throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value);
        } // else
    } catch (std::exception& a_Error) {
        std::cerr << "Exception: " << a_Error.what() << "\n";
    } // catch

    return 0;
}

