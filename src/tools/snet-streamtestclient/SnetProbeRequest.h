/**
 * \file SnetProbeRequest.h
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

#ifndef SNET_PROBE_REQUEST_H
#define SNET_PROBE_REQUEST_H

#include <vector>
#include "SnetAppMessage.h"

class SnetProbeRequest: public SnetAppMessage {
public:
    SnetProbeRequest(uint16_t a_UnicastSSA): SnetAppMessage(0x22, 0x22, 0x00, 0x4000, a_UnicastSSA, true), m_LocalSeed(0), m_LocalSeqNbr(0) {}
    
    // Setters
    void SetLocalSeed  (uint32_t a_LocalSeed   = 0) { m_LocalSeed   = a_LocalSeed; }
    void SetLocalSeqNbr(uint32_t a_LocalSeqNbr = 0) { m_LocalSeqNbr = a_LocalSeqNbr; }

    // Serializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetAppMessage::Serialize());
        l_Buffer.emplace_back((m_LocalSeed   & 0xFF000000) >> 24);
        l_Buffer.emplace_back((m_LocalSeed   & 0x00FF0000) >> 16);
        l_Buffer.emplace_back((m_LocalSeed   & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_LocalSeed   & 0x000000FF);
        l_Buffer.emplace_back((m_LocalSeqNbr & 0xFF000000) >> 24);
        l_Buffer.emplace_back((m_LocalSeqNbr & 0x00FF0000) >> 16);
        l_Buffer.emplace_back((m_LocalSeqNbr & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_LocalSeqNbr & 0x000000FF);
        return l_Buffer;
    }

private:
    // Members   
    uint32_t m_LocalSeed;
    uint32_t m_LocalSeqNbr;
};

#endif // SNET_PROBE_REQUEST_H
