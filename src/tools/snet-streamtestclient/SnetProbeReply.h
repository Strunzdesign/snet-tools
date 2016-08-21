/**
 * \file SnetProbeReply.h
 * \brief 
 *
 * The hdlc-tools implement the HDLC protocol to easily talk to devices connected via serial communications
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

#ifndef SNET_PROBE_REPLY_H
#define SNET_PROBE_REPLY_H

#include <vector>
#include "SnetAppMessage.h"

class SnetProbeReply: public SnetAppMessage {
public:
    SnetProbeReply(): SnetAppMessage(), m_RemoteSeed(0), m_LocalSeed(0), m_RemoteSeqNbr(0), m_LocalSeqNbr(0) {}

    // Getters
    uint32_t GetRemoteSeed()   const { return m_RemoteSeed;   }
    uint32_t GetLocalSeed()    const { return m_LocalSeed;    }
    uint32_t GetRemoteSeqNbr() const { return m_RemoteSeqNbr; }
    uint32_t GetLocalSeqNbr()  const { return m_LocalSeqNbr;  }
    
    // Deserializer
    size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        size_t l_Offset = SnetAppMessage::Deserialize(a_Buffer);
        if (l_Offset) {
            // Checks
            if ((a_Buffer.size() - l_Offset) != 16) {
                // Invalid payload size
                l_Offset = 0;
            } else {
                // Parsing
                m_RemoteSeed    = (uint32_t(a_Buffer[l_Offset +  0]) << 24);
                m_RemoteSeed   += (uint32_t(a_Buffer[l_Offset +  1]) << 16);
                m_RemoteSeed   += (uint32_t(a_Buffer[l_Offset +  2]) <<  8);
                m_RemoteSeed   += (uint32_t(a_Buffer[l_Offset +  3]));
                m_LocalSeed     = (uint32_t(a_Buffer[l_Offset +  4]) << 24);
                m_LocalSeed    += (uint32_t(a_Buffer[l_Offset +  5]) << 16);
                m_LocalSeed    += (uint32_t(a_Buffer[l_Offset +  6]) <<  8);
                m_LocalSeed    += (uint32_t(a_Buffer[l_Offset +  7]));
                m_RemoteSeqNbr  = (uint32_t(a_Buffer[l_Offset +  8]) << 24);
                m_RemoteSeqNbr += (uint32_t(a_Buffer[l_Offset +  9]) << 16);
                m_RemoteSeqNbr += (uint32_t(a_Buffer[l_Offset + 10]) <<  8);
                m_RemoteSeqNbr += (uint32_t(a_Buffer[l_Offset + 11]));
                m_LocalSeqNbr   = (uint32_t(a_Buffer[l_Offset + 12]) << 24);
                m_LocalSeqNbr  += (uint32_t(a_Buffer[l_Offset + 13]) << 16);
                m_LocalSeqNbr  += (uint32_t(a_Buffer[l_Offset + 14]) <<  8);
                l_Offset += 16;
            } // else
        } // if
        
        return l_Offset; 
    }

private:
    // Members   
    uint32_t m_RemoteSeed;
    uint32_t m_LocalSeed;
    uint32_t m_RemoteSeqNbr;
    uint32_t m_LocalSeqNbr;
};

#endif // SNET_PROBE_REPLY_H
