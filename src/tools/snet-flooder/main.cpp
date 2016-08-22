/**
 * \file main.cpp
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

#include <iostream>
#include <boost/asio.hpp>
#include "HdlcdAccessClient.h"
#include "FloodingPacketSource.h"

int main(int argc, char* argv[]) {
    try {
        std::cerr << "s-net(r) flooder tool\n";
        if (argc != 5) {
            std::cerr << "Usage: snet-flooder <host> <port> <usb-device> <Unicast-HEX-SSA>\n";
            return 1;
        } // if

        // Convert the provided hexadecimal SSA
        uint16_t l_UnicastSSA;
        std::istringstream l_Converter(argv[4]);
        l_Converter >> std::hex >> l_UnicastSSA;

        // Install signal handlers
        boost::asio::io_service io_service;
        boost::asio::signal_set signals_(io_service);
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
        signals_.async_wait([&io_service](boost::system::error_code a_ErrorCode, int a_SignalNumber){io_service.stop();});
        
        // Resolve destination
        boost::asio::ip::tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });

        // Prepare access protocol entity
        HdlcdAccessClient l_AccessClient(io_service, endpoint_iterator, argv[3], 0x00);
        l_AccessClient.SetOnClosedCallback([&io_service](){io_service.stop();});

        // Prepare input
        FloodingPacketSource l_FloodingPacketSource(l_AccessClient, l_UnicastSSA);

        // Start event processing
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    } // catch
    
    return 0;
}

