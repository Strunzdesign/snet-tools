/**
 * \file FloodingPacketSource.h
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

#ifndef FLOODING_PACKET_SOURCE_H
#define FLOODING_PACKET_SOURCE_H

#include "HdlcdClient.h"
#include "SnetPlatformRequest.h"

class FloodingPacketSource {
public:
    FloodingPacketSource(HdlcdClient& a_HdlcdClient, uint16_t a_UnicastSSA): m_HdlcdClient(a_HdlcdClient), m_UnicastSSA(a_UnicastSSA), m_usSeqNr(0) {
    }
    
    void Start() {
        // Trigger activity
        SendNextPacket();
    }
    
private:
    // Helpers
    void SendNextPacket() {
        // Create firmware request message
        SnetPlatformRequest l_PlatformRequest(m_UnicastSSA);

        // Send one firmware request, and if done, call this method again via a provided lambda-callback
        if (m_HdlcdClient.Send(std::move(HdlcdPacketData::CreatePacket(l_PlatformRequest.Serialize(), false)), [this](){ SendNextPacket(); })) {
            // One packet is on its way
            if (((++m_usSeqNr) % 1000) == 0) {
                m_usSeqNr = 0;
                std::cout << "1000 Packets written to the TCP socket" << std::endl;
            } // if
        } // if
    }
    
    // Members
    HdlcdClient& m_HdlcdClient;
    uint16_t m_UnicastSSA;
    unsigned short m_usSeqNr;
};

#endif // FLOODING_PACKET_SOURCE_H
