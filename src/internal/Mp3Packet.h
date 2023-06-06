/*-------------------------------------------------------------------------
Mp3Packet - packet specific enums and structures

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

enum Mp3_Commands
{
    Mp3_Commands_None = 0x00,
    Mp3_Commands_PlayNextTrack = 0x01,
    Mp3_Commands_PlayPrevTrack = 0x02,
    Mp3_Commands_PlayGlobalTrack = 0x03,
    Mp3_Commands_IncVolume = 0x04,
    Mp3_Commands_DecVolume = 0x05,
    Mp3_Commands_SetVolume = 0x06,
    Mp3_Commands_SetEq = 0x07,
    Mp3_Commands_LoopGlobalTrack = 0x08,
    Mp3_Commands_SetPlaybackMode = 0x08,
    Mp3_Commands_SetPlaybackSource = 0x09,
    Mp3_Commands_Sleep = 0x0a,
    Mp3_Commands_Awake = 0x0b,
    Mp3_Commands_Reset = 0x0c,
    Mp3_Commands_Start = 0x0d,
    Mp3_Commands_Pause = 0x0e,
    Mp3_Commands_PlayFolderTrack = 0x0f,
    Mp3_Commands_RepeatPlayInRoot = 0x11,
    Mp3_Commands_PlayMp3FolderTrack = 0x12,
    Mp3_Commands_PlayAdvertTrack = 0x13,
    Mp3_Commands_PlayFolderTrack16 = 0x14,
    Mp3_Commands_StopAdvert = 0x15,
    Mp3_Commands_Stop = 0x16,
    Mp3_Commands_LoopInFolder = 0x17,
    Mp3_Commands_PlayRandmomGlobalTrack = 0x18,
    Mp3_Commands_RepeatPlayCurrentTrack = 0x19,
    Mp3_Commands_SetDacInactive = 0x1a,
    Mp3_Commands_Requests = 0x30, // after this is all request, before all actions
    Mp3_Commands_GetPlaySources = 0x3f, // deprecated due to conflict with replies
    Mp3_Commands_GetStatus = 0x42,
    Mp3_Commands_GetVolume = 0x43,
    Mp3_Commands_GetEq = 0x44,
    Mp3_Commands_GetPlaybackMode = 0x45,
    Mp3_Commands_GetSoftwareVersion = 0x46,
    Mp3_Commands_GetUsbTrackCount = 0x47,
    Mp3_Commands_GetSdTrackCount = 0x48,
    Mp3_Commands_GetFlashTrackCount = 0x49,
    Mp3_Commands_GetUsbCurrentTrack = 0x4b,
    Mp3_Commands_GetSdCurrentTrack = 0x4c,
    Mp3_Commands_GetFlashCurrentTrack = 0x4d,
    Mp3_Commands_GetFolderTrackCount = 0x4e,
    Mp3_Commands_GetTotalFolderCount = 0x4f,
};


enum Mp3_Replies
{
    Mp3_Replies_PlaySource_Inserted = 0x3a,
    Mp3_Replies_PlaySource_Removed = 0x3b,
    Mp3_Replies_TrackFinished_Usb = 0x3c,
    Mp3_Replies_TrackFinished_Sd = 0x3d,
    Mp3_Replies_TrackFinished_Flash = 0x3e,
    Mp3_Replies_PlaySource_Online = 0x3f,
    Mp3_Replies_Error = 0x40,
    Mp3_Replies_Ack = 0x41,
};

const uint8_t Mp3_PacketStartCode = 0x7e;
const uint8_t Mp3_PacketVersion = 0xff;
const uint8_t Mp3_PacketEndCode = 0xef;

// 7E FF 06 0F 00 01 01 xx xx EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
// 9	->	EF is end code
struct Mp3_Packet_WithCheckSum
{
    uint8_t startCode;
    uint8_t version;
    uint8_t length;
    uint8_t command;
    uint8_t requestAck;
    uint8_t hiByteArgument;
    uint8_t lowByteArgument;
    uint8_t hiByteCheckSum;
    uint8_t lowByteCheckSum;
    uint8_t endCode;
};

// 7E FF 06 0F 00 01 01 EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7	->	EF is end code
struct Mp3_Packet_WithoutCheckSum
{
    uint8_t startCode;
    uint8_t version;
    uint8_t length;
    uint8_t command;
    uint8_t requestAck;
    uint8_t hiByteArgument;
    uint8_t lowByteArgument;
    uint8_t endCode;
};