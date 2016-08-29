/**
 * \file SnetPeerStatistic.h
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

#ifndef SNET_PEER_STATISTIC_H
#define SNET_PEER_STATISTIC_H

#include <vector>
#include "SnetAppMessage.h"

class SnetPeerStatistic: public SnetAppMessage {
public:
    SnetPeerStatistic(): SnetAppMessage(), m_RemoteSeed(0), m_LocalSeed(0), m_LocalGeneration(0), m_AmountOfPackets(0) {}

    // Getters
    uint32_t GetRemoteSeed()      const { return m_RemoteSeed;      }
    uint32_t GetLocalSeed()       const { return m_LocalSeed;       }
    uint32_t GetLocalGeneration() const { return m_LocalGeneration; }
    uint16_t GetAmountOfPackets() const { return m_AmountOfPackets; }
    
    // Serializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetAppMessage::Serialize());
        l_Buffer.emplace_back((m_RemoteSeed      & 0xFF000000) >> 24);
        l_Buffer.emplace_back((m_RemoteSeed      & 0x00FF0000) >> 16);
        l_Buffer.emplace_back((m_RemoteSeed      & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_RemoteSeed      & 0x000000FF);
        l_Buffer.emplace_back((m_LocalSeed       & 0xFF000000) >> 24);
        l_Buffer.emplace_back((m_LocalSeed       & 0x00FF0000) >> 16);
        l_Buffer.emplace_back((m_LocalSeed       & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_LocalSeed       & 0x000000FF);
        l_Buffer.emplace_back((m_LocalGeneration & 0x00FF0000) >> 16);
        l_Buffer.emplace_back((m_LocalGeneration & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_LocalGeneration & 0x000000FF);
        l_Buffer.emplace_back((m_AmountOfPackets & 0x0000FF00) >>  8);
        l_Buffer.emplace_back (m_AmountOfPackets & 0x000000FF);
        return l_Buffer;
    }

    // Deserializer
    size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        size_t l_Offset = SnetAppMessage::Deserialize(a_Buffer);
        if (l_Offset) {
            // Checks
            if ((a_Buffer.size() - l_Offset) != 13) {
                // Invalid payload size
                l_Offset = 0;
            } else {
                // Parsing
                m_RemoteSeed      += (uint32_t(a_Buffer[l_Offset +  0]) << 24);
                m_RemoteSeed      += (uint32_t(a_Buffer[l_Offset +  1]) << 16);
                m_RemoteSeed      += (uint32_t(a_Buffer[l_Offset +  2]) <<  8);
                m_RemoteSeed      += (uint32_t(a_Buffer[l_Offset +  3]));
                m_LocalSeed       += (uint32_t(a_Buffer[l_Offset +  4]) << 24);
                m_LocalSeed       += (uint32_t(a_Buffer[l_Offset +  5]) << 16);
                m_LocalSeed       += (uint32_t(a_Buffer[l_Offset +  6]) <<  8);
                m_LocalSeed       += (uint32_t(a_Buffer[l_Offset +  7]));
                m_LocalGeneration += (uint32_t(a_Buffer[l_Offset +  8]) << 16);
                m_LocalGeneration += (uint32_t(a_Buffer[l_Offset +  9]) <<  8);
                m_LocalGeneration += (uint32_t(a_Buffer[l_Offset + 10]));
                m_AmountOfPackets += (uint16_t(a_Buffer[l_Offset + 11]) <<  8);
                m_AmountOfPackets += (uint16_t(a_Buffer[l_Offset + 12]));
                l_Offset += 13;
            } // else
        } // if
        
        return l_Offset; 
    }

private:
    // Members   
    uint32_t m_RemoteSeed;
    uint32_t m_LocalSeed;
    uint32_t m_LocalGeneration;
    uint16_t m_AmountOfPackets;
};

#endif // SNET_PEER_STATISTICS_H
