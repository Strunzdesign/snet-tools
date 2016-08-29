/**
 * \file Statistic.h
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

#ifndef STATISTIC_H
#define STATISTIC_H

#include <bitset>

class Statistic {
public:
    Statistic(std::string a_Identifier): m_Identifier(a_Identifier)  {
        Reset();
    }
    
    void Reset() {
        m_SeqNbrsSeen[0].reset();
        m_SeqNbrsSeen[1].reset();
        m_Generation = 0;
        m_BankIndex = 0;
    }
    
    void Update(uint32_t a_SeqNbr) {
        uint32_t l_Generation = ((a_SeqNbr & 0xFFFFFF00) >> 8);
        uint8_t  l_Nbr        =  (a_SeqNbr & 0x000000FF);
        if (((l_Generation + 1) & 0x00FFFFFFF) == m_Generation) {
            // Ignore
        } else if (l_Generation == m_Generation) {
            // Copy
            m_SeqNbrsSeen[m_BankIndex].set(l_Nbr);
        } else if (l_Generation == ((m_Generation + 1) & 0x00FFFFFF)) {
            // Copy
            uint8_t l_BankIndex = ((m_BankIndex + 1) & 0x01);
            m_SeqNbrsSeen[l_BankIndex].set(l_Nbr);
        } else if (l_Generation == ((m_Generation + 2) & 0x00FFFFFF)) {
            // One generation is complete now
            PrintReport(m_Generation, m_SeqNbrsSeen[m_BankIndex].count());
            ++m_Generation;
            m_SeqNbrsSeen[m_BankIndex].reset();
            m_SeqNbrsSeen[m_BankIndex].set(l_Nbr);
            m_BankIndex = ((m_BankIndex + 1) & 0x01);
        } else if (l_Generation == ((m_Generation + 3) & 0x00FFFFFF)) {
            // Both generations are complete now
            PrintReport(m_Generation, m_SeqNbrsSeen[m_BankIndex].count());
            ++m_Generation;
            uint8_t l_BankIndex = ((m_BankIndex + 1) & 0x01);
            PrintReport(m_Generation, m_SeqNbrsSeen[m_BankIndex].count());
            ++m_Generation;
            m_SeqNbrsSeen[0].reset();
            m_SeqNbrsSeen[1].reset();
            m_SeqNbrsSeen[l_BankIndex].set(l_Nbr);
        } else {
            // Start from scratch
            m_SeqNbrsSeen[0].reset();
            m_SeqNbrsSeen[1].reset();
            m_BankIndex = 0;
            m_Generation = l_Generation;
            m_SeqNbrsSeen[0].set(l_Nbr);
        } // else
    }

private:
    // Helpers
    void PrintReport(uint32_t a_Generation, size_t a_AmountOfPackets) {
        std::cout << "Report " << std::dec << m_Identifier << ": Gen = " << a_Generation << ", Amount = " << a_AmountOfPackets;
        if (a_AmountOfPackets == 256) {
            std::cout << std::endl;
        } else {
            std::cout << " (!!!)" << std::endl;
        } // else
    }
    
    // Members
    std::string m_Identifier;
    uint32_t m_Generation;
    uint8_t m_BankIndex;
    std::bitset<256> m_SeqNbrsSeen[2];
};

#endif // STATISTIC_H
