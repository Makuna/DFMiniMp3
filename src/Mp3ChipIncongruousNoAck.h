/*-------------------------------------------------------------------------
Mp3ChipIncongruousNoAck - chip class for T_CHIP_VARIANT template features

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


class Mp3ChipIncongruousNoAck : public Mp3ChipBase
{
public:
    static const bool SendCheckSum = true;

    typedef Mp3_Packet_WithCheckSum SendPacket;
    typedef Mp3_Packet_WithCheckSum ReceptionPacket;

    static const SendPacket generatePacket(uint8_t command, uint16_t arg, bool requestAck = false)
    {
        SendPacket packet = {
                Mp3_PacketStartCode,
                Mp3_PacketVersion,
                6, // size, remaining bytes not including end code
                command,
                requestAck,
                static_cast<uint8_t>(arg >> 8),
                static_cast<uint8_t>(arg & 0x00ff),
                0, // checksum calculated below
                0, // checksum calculated below
                Mp3_PacketEndCode };
        setChecksum(&packet);
        return packet;
    }

    static bool commandSupportsAck(uint8_t command)
    {
        return (command > Mp3_Commands_Requests);
    }
};
