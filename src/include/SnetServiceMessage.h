/**
 * \file SnetServiceMessage.h
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

#ifndef SNET_SERVICE_MESSAGE_H
#define SNET_SERVICE_MESSAGE_H

#include "SnetAppMessage.h"

class SnetServiceMessage: public SnetAppMessage {
public:
    SnetServiceMessage(uint8_t a_SrcServiceId = 0, uint8_t a_DstServiceId = 0, uint8_t a_Token = 0, uint16_t a_SrcSSA = 0xFFFE, uint16_t a_DstSSA = 0xFFFE, bool a_OnAirARQ = true): SnetAppMessage(a_SrcServiceId, a_DstServiceId, a_Token, a_SrcSSA, a_DstSSA, a_OnAirARQ) {}

    // Setters and getters
    void SetPayload(const std::vector<unsigned char> &a_Payload) { m_Payload = a_Payload; }
    std::vector<unsigned char> GetPayload() const { return m_Payload; }

    // Serializer and deserializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetAppMessage::Serialize());
        l_Buffer.insert(l_Buffer.end(), m_Payload.begin(), m_Payload.end());
        return l_Buffer;
    }

    size_t Deserialize(const std::vector<unsigned char>& a_Buffer) {
        size_t l_Offset = SnetAppMessage::Deserialize(a_Buffer);
        if (l_Offset) {
            m_Payload.assign((a_Buffer.begin() + l_Offset), a_Buffer.end());
            l_Offset += m_Payload.size();
        } // if
        
        return l_Offset; 
    }

    // Dissector
    std::string Dissect() const {
        std::stringstream l_Output;
        l_Output << SnetAppMessage::Dissect() << ", with " << std::dec << std::setw(3) << m_Payload.size() << " bytes payload:";
        for (auto it = m_Payload.begin(); it != m_Payload.end(); ++it) {
            l_Output << " " << std::hex << std::setw(2) << std::setfill('0') << int(*it);
        } // for

        return l_Output.str();
    }
    
private:
    std::vector<unsigned char> m_Payload;
};

#endif // SNET_SERVICE_MESSAGE_H
