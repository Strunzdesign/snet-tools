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
        size_t l_Offset = 0;
        m_OnAirARQ = (a_Buffer[1] & 0x10);
        m_SrcSSA   = ntohs(*(uint16_t*)(reinterpret_cast<const uint16_t*>(&a_Buffer[2])));
        m_DstSSA   = ntohs(*(uint16_t*)(reinterpret_cast<const uint16_t*>(&a_Buffer[4])));
        l_Offset = 7;
        return l_Offset; 
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
        l_Buffer.emplace_back(0x10);
        l_Buffer.emplace_back((m_SrcSSA >> 8) & 0xFF);
        l_Buffer.emplace_back((m_SrcSSA >> 0) & 0xFF);
        l_Buffer.emplace_back((m_DstSSA >> 8) & 0xFF);
        l_Buffer.emplace_back((m_DstSSA >> 0) & 0xFF);
        l_Buffer.emplace_back(0x80);
        return l_Buffer;
    }

private:
    // Protocol members
    uint16_t m_SrcSSA;
    uint16_t m_DstSSA;
    bool     m_OnAirARQ;
};

class SnetApplicationMessage: public SnetPacket {
public:
    SnetApplicationMessage(uint8_t a_SrcServiceId = 0, uint8_t a_DstServiceId = 0, uint8_t a_Token = 0, uint16_t a_SrcSSA = 0xFFFE, uint16_t a_DstSSA = 0xFFFE, bool a_OnAirARQ = true): SnetPacket(a_SrcSSA, a_DstSSA, a_OnAirARQ), m_SrcServiceId(a_SrcServiceId), m_DstServiceId(a_DstServiceId), m_Token(a_Token) {}
    
    // Setters and getters
    void     SetSrcServiceId(uint8_t a_SrcServiceId = 0) { m_SrcServiceId = a_SrcServiceId; }
    uint8_t  GetSrcServiceId() const { return m_SrcServiceId; }
    void     SetDstServiceId(uint8_t a_DstServiceId = 0) { m_DstServiceId = a_DstServiceId; }
    uint8_t  GetDstServiceId() const { return m_DstServiceId; }
    void     SetToken(uint8_t a_Token = 0) { m_Token = a_Token; }
    uint16_t GetToken() const { return m_Token; }
    
    // Deserializer
    size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        size_t l_Offset = SnetPacket::Deserialize(a_Buffer);
        if (l_Offset) {
            m_SrcServiceId = a_Buffer[l_Offset + 1];
            m_DstServiceId = a_Buffer[l_Offset + 2];
            m_Token        = a_Buffer[l_Offset + 4];
            l_Offset += 5;
        } // if
        
        return l_Offset; 
    }
    
    // Dissector
    std::string Dissect() const {
        std::stringstream l_Output;
        l_Output << SnetPacket::Dissect()
                 << ", SrcSrv=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)m_SrcServiceId
                 << ", DstSrv=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)m_DstServiceId
                 << ", Token=0x"  << std::hex << std::setw(2) << std::setfill('0') << (int)m_Token;
        return l_Output.str();
    }
    
protected:
    // Serializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetPacket::Serialize());
        l_Buffer.emplace_back(0x00);
        l_Buffer.emplace_back(m_SrcServiceId);
        l_Buffer.emplace_back(m_DstServiceId);
        l_Buffer.emplace_back(0x00);
        l_Buffer.emplace_back(m_Token);
        return l_Buffer;
    }

private:
    uint8_t  m_SrcServiceId;
    uint8_t  m_DstServiceId;
    uint8_t  m_Token;
};


class SnetServiceMessage: public SnetApplicationMessage {
public:
    SnetServiceMessage(uint8_t a_SrcServiceId = 0, uint8_t a_DstServiceId = 0, uint8_t a_Token = 0, uint16_t a_SrcSSA = 0xFFFE, uint16_t a_DstSSA = 0xFFFE, bool a_OnAirARQ = true): SnetApplicationMessage(a_SrcServiceId, a_DstServiceId, a_Token, a_SrcSSA, a_DstSSA, a_OnAirARQ) {}

    // Setters and getters
    void SetPayload(const std::vector<unsigned char> &a_Payload) { m_Payload = a_Payload; }
    std::vector<unsigned char> GetPayload() const { return m_Payload; }

    // Serializer and deserializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetApplicationMessage::Serialize());
        l_Buffer.insert(l_Buffer.end(), m_Payload.begin(), m_Payload.end());
        return l_Buffer;
    }

    size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        size_t l_Offset = SnetApplicationMessage::Deserialize(a_Buffer);
        if (l_Offset) {
            m_Payload.assign((a_Buffer.begin() + l_Offset), a_Buffer.end());
            l_Offset += m_Payload.size();
        } // if
        
        return l_Offset; 
    }

    // Dissector
    std::string Dissect() const {
        std::stringstream l_Output;
        l_Output << SnetApplicationMessage::Dissect() << ", with " << std::dec << std::setw(3) << m_Payload.size() << " bytes payload:";
        for (auto it = m_Payload.begin(); it != m_Payload.end(); ++it) {
            l_Output << " " << std::hex << std::setw(2) << std::setfill('0') << int(*it);
        } // for

        return l_Output.str();
    }
    
private:
    std::vector<unsigned char> m_Payload;
};

#endif // SNET_PACKET_H
