/**
 * \file StreamTestEntity.h
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

#ifndef STREAM_TEST_ENTITY_H
#define STREAM_TEST_ENTITY_H

#include "HdlcdAccessClient.h"
#include "Statistic.h"
#include "SnetProbeRequest.h"
#include "SnetProbeReply.h"
#include "SnetPeerStatistic.h"

class StreamTestEntity {
public:
    StreamTestEntity(HdlcdAccessClient& a_AccessClient, uint16_t a_UnicastSSA): m_AccessClient(a_AccessClient), m_LocalSeqNbr(0), m_RemoteStatistic("remote      "), m_LocalStatistic("local       "), m_UnicastSSA(a_UnicastSSA) {
        srand(::time(NULL));
        m_LocalSeed = rand();
        m_RemoteSeed = 0;
        m_TotalBytesWritten = 0;
        
        // Trigger activity
        SendNextProbeRequest();
    }

    void PacketReceived(const HdlcdPacketData& a_PacketData) {
        SnetAppMessage l_AppMessage;
        if (l_AppMessage.Deserialize(a_PacketData.GetData())) {
            // Checks
            if ((l_AppMessage.GetSrcSSA() == m_UnicastSSA) && (l_AppMessage.GetSrcServiceId() == 0x22) && (l_AppMessage.GetDstServiceId() == 0x22)) {
                if (l_AppMessage.GetToken() == 0x01) {
                    HandleProbeReply(a_PacketData);
                } else if (l_AppMessage.GetToken() == 0x02) {
                    HandlePeerStatistics(a_PacketData);
                } // else if
            } // if
        } // if
    }
    
private:
    // Helpers
    void HandleProbeReply(const HdlcdPacketData& a_PacketData) {
        SnetProbeReply l_ProbeReply;
        if (l_ProbeReply.Deserialize(a_PacketData.GetData())) {
            UpdateStatistics(l_ProbeReply.GetRemoteSeed(), l_ProbeReply.GetLocalSeed(), l_ProbeReply.GetRemoteSeqNbr(), l_ProbeReply.GetLocalSeqNbr());
        } // if
    }
    
    void HandlePeerStatistics(const HdlcdPacketData& a_PacketData) {
        SnetPeerStatistic l_PeerStatistic;
        if (l_PeerStatistic.Deserialize(a_PacketData.GetData())) {        
            // Print
            std::cout << "New peer statistics, Gen = " << l_PeerStatistic.GetLocalGeneration() << ", Amount = " << l_PeerStatistic.GetAmountOfPackets();
            if (l_PeerStatistic.GetAmountOfPackets() == 256) {
                std::cout << std::endl;
            } else {
                std::cout << " (!!!)" << std::endl;
            } // else
        } // if
    }
    
    void UpdateStatistics(uint32_t a_RemoteSeed, uint32_t a_LocalSeed, uint32_t a_RemoteSeqNbr, uint32_t a_LocalSeqNbr) {
        // Update statistics
        if (m_LocalSeed != a_LocalSeed) {
            return;
        } // if
            
        if (m_RemoteSeed != a_RemoteSeed) {
            m_RemoteSeed = a_RemoteSeed;
            m_RemoteStatistic.Reset();
            m_LocalStatistic.Reset();
        } // if

        m_RemoteStatistic.Update(a_RemoteSeqNbr);
        m_LocalStatistic.Update(a_LocalSeqNbr);
    }

    void SendNextProbeRequest() {
        // Create probe request
        SnetProbeRequest l_ProbeRequest(m_UnicastSSA);
        l_ProbeRequest.SetLocalSeed  (m_LocalSeed);
        l_ProbeRequest.SetLocalSeqNbr(m_LocalSeqNbr);

        // Send one probe request, and if done, call this method again via a provided lambda-callback
        if (m_AccessClient.Send(std::move(HdlcdPacketData::CreatePacket(l_ProbeRequest.Serialize(), true)), [this](){ SendNextProbeRequest(); })) {
            // One packet is on its way
            m_TotalBytesWritten += l_ProbeRequest.GetSize();
            if (((++m_LocalSeqNbr) % 0xFF) == 0) {
                std::cout << "256 Packets written to the TCP socket (" << std::dec << m_TotalBytesWritten << " bytes total)" << std::endl;
            } // if
        } // if
    }
    
    // Members
    uint16_t m_UnicastSSA;
    uint32_t m_RemoteSeed;
    uint32_t m_LocalSeed;
    HdlcdAccessClient& m_AccessClient;
    uint32_t m_LocalSeqNbr;
    Statistic m_RemoteStatistic;
    Statistic m_LocalStatistic;
    unsigned long m_TotalBytesWritten;
};

#endif // STREAM_TEST_ENTITY_H
