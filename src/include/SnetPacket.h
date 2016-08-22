/**
 * \file SnetPacket.h
 * \brief 
 *
 * Copyright (c) 2016, Florian Evers, florian-evers@gmx.de
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.  
 *     
 *     (3)The name of the author may not be used to
 *     endorse or promote products derived from this software without
 *     specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SNET_PACKET_H
#define SNET_PACKET_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class SnetPacket {
public:
    SnetPacket(uint16_t a_SrcSSA = 0xFFFE, uint16_t a_DstSSA = 0xFFFE, bool a_OnAirARQ = true): m_SrcSSA(a_SrcSSA), m_DstSSA(a_DstSSA), m_OnAirARQ(a_OnAirARQ) {}
    virtual ~SnetPacket(){}

    // Setters and getters
    void     SetSrcSSA(uint16_t a_SrcSSA = 0xFFFE) { m_SrcSSA = a_SrcSSA; }
    uint16_t GetSrcSSA() const { return m_SrcSSA; }
    void     SetDstSSA(uint16_t a_DstSSA = 0xFFFE) { m_DstSSA = a_DstSSA; }
    uint16_t GetDstSSA() const { return m_DstSSA; }
    void     SetOnAirARQ(bool a_OnAirARQ = true) { m_OnAirARQ = a_OnAirARQ; }
    bool     GetOnAirARQ() const { return m_OnAirARQ; }

    // Deserializer
    virtual size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        if (a_Buffer.size() < 6) { return 0; }
        if (a_Buffer[0] != 0x00) { return 0; }
        m_OnAirARQ = (a_Buffer[1] & 0x10);
        m_SrcSSA   = ntohs(*(uint16_t*)(reinterpret_cast<const uint16_t*>(&a_Buffer[2])));
        m_DstSSA   = ntohs(*(uint16_t*)(reinterpret_cast<const uint16_t*>(&a_Buffer[4])));
        if (m_OnAirARQ) {
            if (a_Buffer.size() >= 7) {
                if (a_Buffer[6] == 0x80) {
                    // Ok
                    return 7;
                } // if
            } // if
        } else {
            // Ok
            return 6;
        } // else

        // Not ok
        return 0;
    }

    // Dissector
    virtual std::string Dissect() const {
        std::stringstream l_Output;
        l_Output << "SrcSSA=0x"   << std::hex << std::setw(4) << std::setfill('0') << m_SrcSSA
                 << ", DstSSA=0x" << std::hex << std::setw(4) << std::setfill('0') << m_DstSSA
                 << ", ARQ=" << std::dec << (int)m_OnAirARQ;
        return l_Output.str();
    }
    
protected:
    // Serializer
    virtual std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(0x00);
        if (m_OnAirARQ) {
            l_Buffer.emplace_back(0x10);
        } else {
            l_Buffer.emplace_back(0x00);
        } // else

        l_Buffer.emplace_back((m_SrcSSA >> 8) & 0xFF);
        l_Buffer.emplace_back((m_SrcSSA >> 0) & 0xFF);
        l_Buffer.emplace_back((m_DstSSA >> 8) & 0xFF);
        l_Buffer.emplace_back((m_DstSSA >> 0) & 0xFF);
        if (m_OnAirARQ) {
            l_Buffer.emplace_back(0x80);
        } // if

        return l_Buffer;
    }

private:
    // Protocol members
    uint16_t m_SrcSSA;
    uint16_t m_DstSSA;
    bool     m_OnAirARQ;
};

#endif // SNET_PACKET_H
