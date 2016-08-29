/**
 * \file SnetPacketPrinter.h
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

#ifndef SNET_PACKET_PRINTER_H
#define SNET_PACKET_PRINTER_H

#include <iostream>
#include <vector>
#include "HdlcdPacketData.h"
#include "SnetServiceMessage.h"

void PrintDissectedSnetPacket(const HdlcdPacketData& a_PacketData) {
    // Print dissected s-net packet
    SnetServiceMessage l_ServiceMessage;
    if (l_ServiceMessage.Deserialize(a_PacketData.GetData())) {
        if (a_PacketData.GetWasSent()) {
            //std::cout << "<<< Sent " << static_cast<SnetPacket>(l_ServiceMessage).Dissect() << std::endl;;
            //std::cout << "<<< Sent " << static_cast<ApplicationMessage>(l_ServiceMessage).Dissect() << std::endl;;
            std::cout << "<<< Sent " << l_ServiceMessage.Dissect() << std::endl;;
        } else {
            //std::cout << ">>> Rcvd " << static_cast<SnetPacket>(l_ServiceMessage).Dissect() << std::endl;;
            //std::cout << ">>> Rcvd " << static_cast<ApplicationMessage>(l_ServiceMessage).Dissect() << std::endl;;
            std::cout << ">>> Rcvd " << l_ServiceMessage.Dissect() << std::endl;;
        } // else
    } // if
}

#endif // SNET_PACKET_PRINTER_H
