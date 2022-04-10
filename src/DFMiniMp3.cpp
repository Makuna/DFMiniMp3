/*-------------------------------------------------------------------------
DFMiniMp3 library

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/DFMiniMp3)

-------------------------------------------------------------------------
This file is part of the Makuna/DFMiniMp3 library.

DFMiniMp3 is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

DFMiniMp3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with DFMiniMp3.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

#include "DFMiniMp3.h"

uint16_t calcChecksum(const DfMp3_Packet_WithCheckSum& packet)
{
    uint16_t sum = 0xFFFF;
    for (
        const uint8_t* packetByte = &(packet.version);
        packetByte != &(packet.hiByteCheckSum);
        packetByte++
    )
    {
        sum -= *packetByte;
    }
    return sum + 1;
}

void setChecksum(DfMp3_Packet_WithCheckSum* out)
{
    uint16_t sum = calcChecksum(*out);

    out->hiByteCheckSum = (sum >> 8);
    out->lowByteCheckSum = (sum & 0xff);
}

bool validateChecksum(const DfMp3_Packet_WithCheckSum& in)
{
    uint16_t sum = calcChecksum(in);
    return (sum == static_cast<uint16_t>((in.hiByteCheckSum << 8) | in.lowByteCheckSum));
}
