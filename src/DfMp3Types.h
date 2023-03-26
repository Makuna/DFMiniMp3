/*-------------------------------------------------------------------------
DfMp3Types - API exposed support enums and structs

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


enum DfMp3_Error
{
    //                              alternative meanings depending on chip
    // from device                
    DfMp3_Error_Busy = 1,         //  busy                  busy
    DfMp3_Error_Sleeping,         //  frame not received    sleep
    DfMp3_Error_SerialWrongStack, //  verification error    frame not received
    DfMp3_Error_CheckSumNotMatch, //                        checksum
    DfMp3_Error_FileIndexOut,     //  folder out of scope   track out of scope
    DfMp3_Error_FileMismatch,     //  folder not found      track not found
    DfMp3_Error_Advertise,        //                        only allowed while playing     advertisement not allowed
    DfMp3_Error_SdReadFail,       //                        SD card failed
    DfMp3_Error_FlashReadFail,    //                        Flash mem failed
    DfMp3_Error_EnteredSleep = 10, //                        entered sleep    
    // from library
    DfMp3_Error_RxTimeout = 0x81,
    DfMp3_Error_PacketSize,
    DfMp3_Error_PacketHeader,
    DfMp3_Error_PacketChecksum,
    DfMp3_Error_General = 0xff
};

enum DfMp3_PlaybackMode
{
    DfMp3_PlaybackMode_Repeat,
    DfMp3_PlaybackMode_FolderRepeat,
    DfMp3_PlaybackMode_SingleRepeat,
    DfMp3_PlaybackMode_Random
};

enum DfMp3_Eq
{
    DfMp3_Eq_Normal,
    DfMp3_Eq_Pop,
    DfMp3_Eq_Rock,
    DfMp3_Eq_Jazz,
    DfMp3_Eq_Classic,
    DfMp3_Eq_Bass
};

enum DfMp3_PlaySource // value - only one can be set
{
    DfMp3_PlaySource_Usb = 1,
    DfMp3_PlaySource_Sd,
    DfMp3_PlaySource_Aux,
    DfMp3_PlaySource_Sleep,
    DfMp3_PlaySource_Flash
};

enum DfMp3_PlaySources // bitfield - more than one can be set
{
    DfMp3_PlaySources_Usb = 0x01,
    DfMp3_PlaySources_Sd = 0x02,
    DfMp3_PlaySources_Pc = 0x04,
    DfMp3_PlaySources_Flash = 0x08,
};

enum DfMp3_StatusState
{
    DfMp3_StatusState_Idle = 0x00,
    DfMp3_StatusState_Playing = 0x01,
    DfMp3_StatusState_Paused = 0x02,
    DfMp3_StatusState_Sleep = 0x08, // note, some chips use DfMp3_StatusSource_Sleep
    DfMp3_StatusState_Shuffling = 0x11, // not documented, but discovered
};

enum DfMp3_StatusSource
{
    DfMp3_StatusSource_General = 0x00,
    DfMp3_StatusSource_Usb = 0x01,
    DfMp3_StatusSource_Sd = 0x02,
    DfMp3_StatusSource_Sleep = 0x10,
};

struct DfMp3_Status
{
    DfMp3_StatusSource source;
    DfMp3_StatusState state;
};

