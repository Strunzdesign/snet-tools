/**
 * \file SnetPlatformRequest.h
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

#ifndef SNET_PLATFORM_REQUEST_H
#define SNET_PLATFORM_REQUEST_H

#include <vector>
#include "SnetAppMessage.h"

class SnetPlatformRequest: public SnetAppMessage {
public:
    SnetPlatformRequest(uint16_t a_UnicastSSA): SnetAppMessage(0x10, 0x10, 0x04, 0x4000, a_UnicastSSA, false) {}
    
    // Serializer
    std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer(SnetAppMessage::Serialize());
        l_Buffer.emplace_back(0x06);
        l_Buffer.emplace_back(0x02);
        l_Buffer.emplace_back(0x00);
        l_Buffer.emplace_back(0x80);
        l_Buffer.emplace_back(0x02);
        l_Buffer.emplace_back(0x01);
        return l_Buffer;
    }
};

#endif // SNET_PLATFORM_REQUEST_H
