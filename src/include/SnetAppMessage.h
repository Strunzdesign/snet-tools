/**
 * \file SnetAppMessage.h
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

#ifndef SNET_APP_MESSAGE_H
#define SNET_APP_MESSAGE_H

#include "SnetPacket.h"

class SnetAppMessage: public SnetPacket {
public:
    SnetAppMessage(uint8_t a_SrcServiceId = 0, uint8_t a_DstServiceId = 0, uint8_t a_Token = 0, uint16_t a_SrcSSA = 0xFFFE, uint16_t a_DstSSA = 0xFFFE, bool a_OnAirARQ = true): SnetPacket(a_SrcSSA, a_DstSSA, a_OnAirARQ), m_SrcServiceId(a_SrcServiceId), m_DstServiceId(a_DstServiceId), m_Token(a_Token) {}
    
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
            if ((a_Buffer.size() - l_Offset) < 5) { return 0; }
            if (a_Buffer[l_Offset + 0] != 0x00) { return 0; }
            m_SrcServiceId = a_Buffer[l_Offset + 1];
            m_DstServiceId = a_Buffer[l_Offset + 2];
            if (a_Buffer[l_Offset + 3] != 0x00) { return 0; }
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
    // Query the size of the packet in bytes
    size_t GetSize() const { return (SnetPacket::GetSize() + 5); }
    
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

#endif // SNET_APP_MESSAGE_H
