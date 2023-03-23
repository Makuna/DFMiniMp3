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

enum DfMp3_Commands
{
    DfMp3_Commands_None = 0x00,
    DfMp3_Commands_PlayNextTrack = 0x01,
    DfMp3_Commands_PlayPrevTrack = 0x02,
    DfMp3_Commands_PlayGlobalTrack = 0x03,
    DfMp3_Commands_IncVolume = 0x04,
    DfMp3_Commands_DecVolume = 0x05,
    DfMp3_Commands_SetVolume = 0x06,
    DfMp3_Commands_SetEq = 0x07,
    DfMp3_Commands_LoopGlobalTrack = 0x08,
    DfMp3_Commands_SetPlaybackMode = 0x08,
    DfMp3_Commands_SetPlaybackSource = 0x09,
    DfMp3_Commands_Sleep = 0x0a,
    DfMp3_Commands_Awake = 0x0b,
    DfMp3_Commands_Reset = 0x0c,
    DfMp3_Commands_Start = 0x0d,
    DfMp3_Commands_Pause = 0x0e,
    DfMp3_Commands_PlayFolderTrack = 0x0f,
    DfMp3_Commands_RepeatPlayInRoot = 0x11,
    DfMp3_Commands_PlayMp3FolderTrack = 0x12,
    DfMp3_Commands_PlayAdvertTrack = 0x13,
    DfMp3_Commands_PlayFolderTrack16 = 0x14,
    DfMp3_Commands_StopAdvert = 0x15,
    DfMp3_Commands_Stop = 0x16,
    DfMp3_Commands_LoopInFolder = 0x17,
    DfMp3_Commands_PlayRandmomGlobalTrack = 0x18,
    DfMp3_Commands_RepeatPlayCurrentTrack = 0x19,
    DfMp3_Commands_SetDacInactive = 0x1a,
    DfMp3_Commands_GetPlaySources = 0x3f,
    DfMp3_Commands_Error = 0x40,
    DfMp3_Commands_Ack = 0x41,
    DfMp3_Commands_GetStatus = 0x42,
    DfMp3_Commands_GetVolume = 0x43,
    DfMp3_Commands_GetEq = 0x44,
    DfMp3_Commands_GetPlaybackMode = 0x45,
    DfMp3_Commands_GetUsbTrackount = 0x47,
    DfMp3_Commands_GetSdTrackount = 0x48,
    DfMp3_Commands_GetFlashTrackount = 0x49,
    DfMp3_Commands_GetUsbCurrentTrack = 0x4b,
    DfMp3_Commands_GetSdCurrentTrack = 0x4c,
    DfMp3_Commands_GetFlashCurrentTrack = 0x4d,
    DfMp3_Commands_GetFolderTrackCount = 0x4e,
    DfMp3_Commands_GetTotalFolderCount = 0x4f,
};

enum DfMp3_Error
{
    //                              alternative meanings depending on chip
    // from device                
    DfMp3_Error_Busy = 1,         //  busy                  busy
    DfMp3_Error_Sleeping,         //  frame not received    sleep
    DfMp3_Error_SerialWrongStack, //  verification error    frame not received
    DfMp3_Error_CheckSumNotMatch, //                        checksum
    DfMp3_Error_FileIndexOut,     //                        track out of scope
    DfMp3_Error_FileMismatch,     //                        track not found
    DfMp3_Error_Advertise,        //                        only allowed while playing     advertisement not allowed
    DfMp3_Error_SdReadFail,        //                        SD card failed
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

// 7E FF 06 0F 00 01 01 xx xx EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
// 9	->	EF is end code
struct DfMp3_Packet_WithCheckSum
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
struct DfMp3_Packet_WithoutCheckSum
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

class Mp3ChipBase
{
private:
    static uint16_t calcChecksum(const DfMp3_Packet_WithCheckSum& packet)
    {
        uint16_t sum = 0xFFFF;
        for (const uint8_t* packetByte = &(packet.version); packetByte != &(packet.hiByteCheckSum); packetByte++)
        {
            sum -= *packetByte;
        }
        return sum + 1;
    }

public:
    static void setChecksum(DfMp3_Packet_WithCheckSum* out)
    {
        uint16_t sum = calcChecksum(*out);

        out->hiByteCheckSum = (sum >> 8);
        out->lowByteCheckSum = (sum & 0xff);
    }

    static bool validateChecksum(const DfMp3_Packet_WithCheckSum& in)
    {
        uint16_t sum = calcChecksum(in);
        return (sum == ((static_cast<uint16_t>(in.hiByteCheckSum) << 8) | in.lowByteCheckSum));
    }
};

class Mp3ChipMH2024K16SS : public Mp3ChipBase 
{
public:
    static const bool SendCheckSum = false;

    typedef DfMp3_Packet_WithoutCheckSum SendPacket;
    typedef DfMp3_Packet_WithCheckSum ReceptionPacket;

    static const SendPacket generatePacket(uint8_t command, uint16_t arg, bool requestAck = false) 
    {
        return {
            0x7E,
            0xFF,
            6,
            command,
            requestAck,
            static_cast<uint8_t>(arg >> 8),
            static_cast<uint8_t>(arg & 0x00ff),
            0xEF };
    }
};

class Mp3ChipOriginal : public Mp3ChipBase 
{
public:
    static const bool SendCheckSum = true;

    typedef DfMp3_Packet_WithCheckSum SendPacket;
    typedef DfMp3_Packet_WithCheckSum ReceptionPacket;

    static const SendPacket generatePacket(uint8_t command, uint16_t arg, bool requestAck = false)
    {
        SendPacket packet = {
                0x7E,
                0xFF,
                6,
                command,
                requestAck,
                static_cast<uint8_t>(arg >> 8),
                static_cast<uint8_t>(arg & 0x00ff),
                0,
                0,
                0xEF };
        setChecksum(&packet);
        return packet;
    }
};

template <class T_SERIAL_METHOD, class T_NOTIFICATION_METHOD, class T_CHIP_VARIANT = Mp3ChipOriginal>
class DFMiniMp3
{
public:
    explicit DFMiniMp3(T_SERIAL_METHOD& serial) :
        _serial(serial),
        _comRetries(3), // default to three retries
        _isOnline(false)
    {
    }

    void begin(unsigned long baud = 9600)
    {
        _serial.begin(baud);
        _serial.setTimeout(900); // long enough to allow Mp3 to think
    }

    void setComRetries(uint8_t retries)
    {
        _comRetries = retries;
    }

    void loop()
    {
        while (_serial.available() >= static_cast<int>(sizeof(typename T_CHIP_VARIANT::ReceptionPacket)))
        {
            listenForReply(DfMp3_Commands_None);
        }
    }

    // Does not work with all models.
    // YX5200-24SS - sends reply
    // MH2024K-24SS - sends NO reply --> results in error notification
    DfMp3_PlaySources getPlaySources()
    {
        return getCommand(DfMp3_Commands_GetPlaySources).arg;
    }

    // the track as enumerated across all folders
    void playGlobalTrack(uint16_t track = 0)
    {
        setCommand(DfMp3_Commands_PlayGlobalTrack, track);
    }

    // sd:/mp3/####track name
    void playMp3FolderTrack(uint16_t track)
    {
        setCommand(DfMp3_Commands_PlayMp3FolderTrack, track);
    }

    // older devices: sd:/###/###track name
    // newer devices: sd:/##/###track name
    // folder and track numbers are zero padded
    void playFolderTrack(uint8_t folder, uint8_t track)
    {
        uint16_t arg = (folder << 8) | track;
        setCommand(DfMp3_Commands_PlayFolderTrack, arg);
    }

    // sd:/##/####track name
    // track number must be four digits, zero padded
    void playFolderTrack16(uint8_t folder, uint16_t track)
    {
        uint16_t arg = (static_cast<uint16_t>(folder) << 12) | track;
        setCommand(DfMp3_Commands_PlayFolderTrack16, arg);
    }

    void playRandomTrackFromAll()
    {
        setCommand(DfMp3_Commands_PlayRandmomGlobalTrack);
    }

    void nextTrack()
    {
        setCommand(DfMp3_Commands_PlayNextTrack);
    }

    void prevTrack()
    {
        setCommand(DfMp3_Commands_PlayPrevTrack);
    }

    uint16_t getCurrentTrack(DfMp3_PlaySource source = DfMp3_PlaySource_Sd)
    {
        uint8_t command;

        switch (source)
        {
        case DfMp3_PlaySource_Usb:
            command = DfMp3_Commands_GetUsbCurrentTrack;
            break;
        case DfMp3_PlaySource_Sd:
            command = DfMp3_Commands_GetSdCurrentTrack;
            break;
        case DfMp3_PlaySource_Flash:
            command = DfMp3_Commands_GetFlashCurrentTrack;
            break;
        default:
            command = DfMp3_Commands_GetSdCurrentTrack;
            break;
        }

        return getCommand(command).arg;
    }

    // 0- 30
    void setVolume(uint8_t volume)
    {
        setCommand(DfMp3_Commands_SetVolume, volume);
    }

    uint8_t getVolume()
    {
        return getCommand(DfMp3_Commands_GetVolume).arg;
    }

    void increaseVolume()
    {
        setCommand(DfMp3_Commands_IncVolume);
    }

    void decreaseVolume()
    {
        setCommand(DfMp3_Commands_DecVolume);
    }

    // useless, removed
    // 0-31
    /*
    void setVolume(bool mute, uint8_t volume)
    {
        uint16_t arg = (!mute << 8) | volume;
        setCommand(0x10, arg);
    }
    */

    void loopGlobalTrack(uint16_t globalTrack)
    {
        setCommand(DfMp3_Commands_LoopGlobalTrack, globalTrack);
    }

    // sd:/##/*
    // 0-99
    void loopFolder(uint8_t folder)
    {
        setCommand(DfMp3_Commands_LoopInFolder, folder);
    }

    // not well supported, use at your own risk
    void setPlaybackMode(DfMp3_PlaybackMode mode)
    {
        setCommand(DfMp3_Commands_SetPlaybackMode, mode);
    }

    DfMp3_PlaybackMode getPlaybackMode()
    {
        return static_cast<DfMp3_PlaybackMode>(getCommand(DfMp3_Commands_GetPlaybackMode).arg);
    }

    void setRepeatPlayAllInRoot(bool repeat)
    {
        setCommand(DfMp3_Commands_RepeatPlayInRoot, !!repeat);
    }

    void setRepeatPlayCurrentTrack(bool repeat)
    {
        setCommand(DfMp3_Commands_RepeatPlayCurrentTrack, !repeat);
    }

    void setEq(DfMp3_Eq eq)
    {
        setCommand(DfMp3_Commands_SetEq, eq);
    }

    DfMp3_Eq getEq()
    {
        return static_cast<DfMp3_Eq>(getCommand(DfMp3_Commands_GetEq).arg);
    }

    void setPlaybackSource(DfMp3_PlaySource source)
    {
        setCommand(DfMp3_Commands_SetPlaybackSource, source);
    }

    void sleep()
    {
        setCommand(DfMp3_Commands_Sleep);
    }

    void awake()
    {
        setCommand(DfMp3_Commands_Awake);
    }

    void reset()
    {
        setCommand(DfMp3_Commands_Reset);
        _isOnline = false;
    }

    void start()
    {
        setCommand(DfMp3_Commands_Start);
    }

    void pause()
    {
        setCommand(DfMp3_Commands_Pause);
    }

    void stop()
    {
        setCommand(DfMp3_Commands_Stop);
    }

    DfMp3_Status getStatus()
    {
        uint16_t reply = getCommand(DfMp3_Commands_GetStatus).arg;

        DfMp3_Status status;
        status.source = static_cast<DfMp3_StatusSource>(reply >> 8);
        status.state = static_cast<DfMp3_StatusState>(reply & 0xff);

        return status;
    }

    uint16_t getFolderTrackCount(uint16_t folder)
    {
        return getCommand(DfMp3_Commands_GetFolderTrackCount).arg;
    }

    uint16_t getTotalTrackCount(DfMp3_PlaySource source = DfMp3_PlaySource_Sd)
    {
        uint8_t command;

        switch (source)
        {
        case DfMp3_PlaySource_Usb:
            command = DfMp3_Commands_GetUsbTrackount;
            break;
        case DfMp3_PlaySource_Sd:
            command = DfMp3_Commands_GetSdTrackount;
            break;
        case DfMp3_PlaySource_Flash:
            command = DfMp3_Commands_GetFlashTrackount;
            break;
        default:
            command = DfMp3_Commands_GetSdTrackount;
            break;
        }

        return getCommand(command).arg;
    }

    uint16_t getTotalFolderCount()
    {
        return getCommand(DfMp3_Commands_GetTotalFolderCount).arg;
    }

    // sd:/advert/####track name
    void playAdvertisement(uint16_t track)
    {
        setCommand(DfMp3_Commands_PlayAdvertTrack, track);
    }

    void stopAdvertisement()
    {
        setCommand(DfMp3_Commands_StopAdvert);
    }

    void enableDac()
    {
        setCommand(DfMp3_Commands_SetDacInactive, 0x00);
    }

    void disableDac()
    {
        setCommand(DfMp3_Commands_SetDacInactive, 0x01);
    }

    bool isOnline() const
    {
        return _isOnline;
    }

private:
    struct reply_t
    {
        uint8_t command;
        uint16_t arg;
    };

    T_SERIAL_METHOD& _serial;
    uint8_t _comRetries;
    bool _isOnline;

    void drainResponses()
    {
        while (_serial.available() > 0)
        {
            listenForReply(DfMp3_Commands_None);
        }
    }

    void sendPacket(uint8_t command, uint16_t arg = 0, bool requestAck = false)
    {
        typename T_CHIP_VARIANT::SendPacket packet = T_CHIP_VARIANT::generatePacket(command, arg, requestAck);

#ifdef DfMiniMp3Debug
        DfMiniMp3Debug.print("OUT ");
        printRawPacket(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));
        DfMiniMp3Debug.println();
#endif

        _serial.write(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    }

    bool readPacket(reply_t* reply)
    {
        typename T_CHIP_VARIANT::ReceptionPacket in;
        uint8_t read;

        // init our out args always
        *reply = { 0 };

        // try to sync our reads to the packet start
        do
        {
            // we use readBytes as it gives us the standard timeout
            read = _serial.readBytes(&in.startCode, 1);

            if (read != 1)
            {
                // nothing read
                reply->arg = DfMp3_Error_RxTimeout;

                return false;
            }
        } while (in.startCode != 0x7e);

        read += _serial.readBytes(&in.version, sizeof(in) - 1);
        if (read < sizeof(in))
        {
            // not enough bytes, corrupted packet
            reply->arg = DfMp3_Error_PacketSize;
            return false;
        }

        if (in.version != 0xFF ||
            in.length != 0x06 ||
            in.endCode != 0xef)
        {
            // invalid version or corrupted packet
            reply->arg = DfMp3_Error_PacketHeader;
            return false;
        }

        if (!T_CHIP_VARIANT::validateChecksum(in))
        {
            // checksum failed, corrupted packet
            reply->arg = DfMp3_Error_PacketChecksum;
            return false;
        }

#ifdef DfMiniMp3Debug
        DfMiniMp3Debug.print("IN ");
        printRawPacket(reinterpret_cast<const uint8_t*>(&in), sizeof(in));
        DfMiniMp3Debug.println();
#endif

        reply->command = in.command;
        reply->arg = ((static_cast<uint16_t>(in.hiByteArgument) << 8) | in.lowByteArgument);

        return true;
    }

    reply_t retryCommand(uint8_t command, 
            uint8_t expectedCommand, 
            uint16_t arg = 0, 
            bool requestAck = false)
    {
        reply_t reply;
        uint8_t retries = _comRetries;

        drainResponses();

        do
        {
            sendPacket(command, arg, requestAck); 
            reply = listenForReply(expectedCommand);
            retries--;
        } while (reply.command != expectedCommand && retries);

        if (reply.command == DfMp3_Commands_Error)
        {
            T_NOTIFICATION_METHOD::OnError(*this, reply.arg);
            reply = { 0 };
        }

        return reply;
    }

    reply_t getCommand(uint8_t command)
    {
        return retryCommand(command, command);
    }

    void setCommand(uint8_t command, uint16_t arg = 0)
    {
        retryCommand(command, DfMp3_Commands_Ack, arg, true);
    }

    reply_t listenForReply(uint8_t command)
    {
        reply_t reply;

        while (readPacket(&reply))
        {
            switch (reply.command)
            {
            case 0x3c: // usb
            case 0x4b: // usb on MH2024K-16SS
                T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Usb, reply.arg);
                break;

            case 0x3d: // micro sd
            case 0x4c: // micro sd on MH2024K-16SS
                T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Sd, reply.arg);
                break;

            case 0x3e: // flash
                T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Flash, reply.arg);
                break;

            case 0x3F:
                _isOnline = true;
                T_NOTIFICATION_METHOD::OnPlaySourceOnline(*this, static_cast<DfMp3_PlaySources>(reply.arg));
                break;

            case 0x3A:
                _isOnline = true;
                T_NOTIFICATION_METHOD::OnPlaySourceInserted(*this, static_cast<DfMp3_PlaySources>(reply.arg));
                break;

            case 0x3B:
                _isOnline = true;
                T_NOTIFICATION_METHOD::OnPlaySourceRemoved(*this, static_cast<DfMp3_PlaySources>(reply.arg));
                break;

            case DfMp3_Commands_Error: // error
                if (command == DfMp3_Commands_None)
                {
                    T_NOTIFICATION_METHOD::OnError(*this, reply.arg);
                }
                // fall through
            case DfMp3_Commands_Ack: // ack
                // fall through
            default:
                if (command != DfMp3_Commands_None)
                {
                    return reply;
                }
                break;
            }
        }


        return { 0 };
    }

#ifdef DfMiniMp3Debug
    void printRawPacket(const uint8_t* data, size_t dataSize)
    {
        char formated[8];
        const uint8_t* end = data + dataSize;

        while (data < end)
        {
            sprintf(formated, " %02x", *data);
            DfMiniMp3Debug.print(formated);
            data++;
        }
    }
#endif
};
