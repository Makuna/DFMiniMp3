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

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

enum DfMp3_Error
{
    DfMp3_Error_Busy = 1,
    DfMp3_Error_Sleeping,
    DfMp3_Error_SerialWrongStack,
    DfMp3_Error_CheckSumNotMatch,
    DfMp3_Error_FileIndexOut,
    DfMp3_Error_FileMismatch,
    DfMp3_Error_Advertise,
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


enum DfMp3_PlaySource
{
    DfMp3_PlaySource_U,
    DfMp3_PlaySource_Sd,
    DfMp3_PlaySource_Aux,
    DfMp3_PlaySource_Sleep,
    DfMp3_PlaySource_Flash
};


template<class T_SERIAL_METHOD, class T_NOTIFICATION_METHOD> class DFMiniMp3
{
public:
    DFMiniMp3(T_SERIAL_METHOD& serial) :
        _serial(serial),
        _lastSendSpace(c_msSendSpace),
        _isOnline(true)
    {
    }

    void begin()
    {
        _serial.begin(9600);
        _serial.setTimeout(600);
        _lastSend = millis();
    }

    void loop()
    {
        while (_serial.available() >= DfMp3_Packet_SIZE)
        {
            listenForReply(0x00);
        }
    }

    // the track as enumerated across all folders
    void playGlobalTrack(uint16_t track = 0)
    {
        sendPacket(0x03, track);
    }

    // sd:/mp3/####track name
    void playMp3FolderTrack(uint16_t track)
    {
        sendPacket(0x12, track);
    }

    // sd:/###/###track name
    void playFolderTrack(uint8_t folder, uint8_t track)
    {
        uint16_t arg = (folder << 8) | track;
        sendPacket(0x0f, arg);
    }

    // sd:/##/####track name
    // track number must be four digits, zero padded
    void playFolderTrack16(uint8_t folder, uint16_t track)
    {
        uint16_t arg = (((uint16_t)folder) << 12) | track;
        sendPacket(0x14, arg);
    }

    void playRandomTrackFromAll()
    {
        sendPacket(0x18);
    }

    void nextTrack()
    {
        sendPacket(0x01);
    }

    void prevTrack()
    {
        sendPacket(0x02);
    }

    uint16_t getCurrentTrack()
    {
        sendPacket(0x4c);
        return listenForReply(0x4c);
    }

    // 0- 30
    void setVolume(uint8_t volume)
    {
        sendPacket(0x06, volume);
    }

    uint8_t getVolume()
    {
        sendPacket(0x43);
        return listenForReply(0x43);
    }

    void increaseVolume()
    {
        sendPacket(0x04);
    }

    void decreaseVolume()
    {
        sendPacket(0x05);
    }

    // useless, removed
    // 0-31
    /*
    void setVolume(bool mute, uint8_t volume)
    {
        uint16_t arg = (!mute << 8) | volume;
        sendPacket(0x10, arg);
    }
    */

    void loopGlobalTrack(uint16_t globalTrack)
    {
        sendPacket(0x08, globalTrack);
    }

    DfMp3_PlaybackMode getPlaybackMode()
    {
        sendPacket(0x45);
        return listenForReply(0x45);
    }

    void setRepeatPlay(bool repeat)
    {
        sendPacket(0x11, !!repeat);
    }


    void setEq(DfMp3_Eq eq)
    {
        sendPacket(0x07, eq);
    }

    DfMp3_Eq getEq()
    {
        sendPacket(0x44);
        return listenForReply(0x44);
    }


    void setPlaybackSource(DfMp3_PlaySource source)
    {
        sendPacket(0x09, source, 200);
    }

    void sleep()
    {
        sendPacket(0x0a);
    }

    void reset()
    {
        sendPacket(0x0c, 0, 600);
        _isOnline = false;
    }

    void start()
    {
        sendPacket(0x0d);
    }

    void pause()
    {
        sendPacket(0x0e);
    }

    void stop()
    {
        sendPacket(0x16);
    }

    uint16_t getStatus()
    {
        sendPacket(0x42);
        return listenForReply(0x42);
    }

    uint16_t getFolderTrackCount(uint16_t folder)
    {
        sendPacket(0x4e, folder);
        return listenForReply(0x4e);
    }

    uint16_t getTotalTrackCount()
    {
        sendPacket(0x48);
        return listenForReply(0x48);
    }

    // sd:/advert/####track name
    void playAdvertisement(uint16_t track)
    {
        sendPacket(0x13, track);
    }

    void stopAdvertisement()
    {
        sendPacket(0x15);
    }

private:
    static const uint16_t c_msSendSpace = 50;

    // 7E FF 06 0F 00 01 01 xx xx EF
    // 0	->	7E is start code
    // 1	->	FF is version
    // 2	->	06 is length
    // 3	->	0F is command
    // 4	->	00 is no receive
    // 5~6	->	01 01 is argument
    // 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
    // 9	->	EF is end code
    enum DfMp3_Packet
    {
        DfMp3_Packet_StartCode,
        DfMp3_Packet_Version,
        DfMp3_Packet_Length,
        DfMp3_Packet_Command,
        DfMp3_Packet_RequestAck,
        DfMp3_Packet_HiByteArgument,
        DfMp3_Packet_LowByteArgument,
        DfMp3_Packet_HiByteCheckSum,
        DfMp3_Packet_LowByteCheckSum,
        DfMp3_Packet_EndCode,
        DfMp3_Packet_SIZE
    };


    T_SERIAL_METHOD& _serial;
    uint32_t _lastSend;
    uint16_t _lastSendSpace;
    bool _isOnline;

    void sendPacket(uint8_t command, uint16_t arg = 0, uint16_t sendSpaceNeeded = c_msSendSpace)
    {
        uint8_t out[DfMp3_Packet_SIZE] = { 0x7E, 0xFF, 06, command, 00, (arg >> 8), (arg & 0x00ff), 00, 00, 0xEF };

        setChecksum(out);

        // wait for spacing since last send
        while (((millis() - _lastSend) < _lastSendSpace) || !_isOnline)
        {
            // check for event messages from the device while
            // we wait
            loop();
            delay(1);
        }

        _lastSendSpace = sendSpaceNeeded;
        _serial.write(out, DfMp3_Packet_SIZE);

        _lastSend = millis();
    }

    bool readPacket(uint8_t* command, uint16_t* argument)
    {
        uint8_t in[DfMp3_Packet_SIZE] = { 0 };
        uint8_t read;

        // init our out args always
        *command = 0;
        *argument = 0;
        
        // try to sync our reads to the packet start
        do
        {
            // we use readBytes as it gives us the standard timeout
            read = _serial.readBytes(&(in[DfMp3_Packet_StartCode]), 1);
            if (read != 1)
            {
                // nothing read
                return false;
            }
        } while (in[DfMp3_Packet_StartCode] != 0x7e);

        read += _serial.readBytes(in + 1, DfMp3_Packet_SIZE - 1);
        if (read < DfMp3_Packet_SIZE)
        {
            // not enough bytes, corrupted packet
            return false;
        }

        if (in[DfMp3_Packet_Version] != 0xFF ||
            in[DfMp3_Packet_Length] != 0x06 ||
            in[DfMp3_Packet_EndCode] != 0xef )
        {
            // invalid version or corrupted packet
            return false;
        }

        if (!validateChecksum(in))
        {
            // checksum failed, corrupted paket
            return false;
        }

        *command = in[DfMp3_Packet_Command];
        *argument = ((in[DfMp3_Packet_HiByteArgument] << 8) | in[DfMp3_Packet_LowByteArgument]);

        return true;
    }

    uint16_t listenForReply(uint8_t command)
    {
        uint8_t replyCommand = 0;
        uint16_t replyArg = 0;

        do
        {
            if (readPacket(&replyCommand, &replyArg))
            {
                if (command != 0 && command == replyCommand)
                {
                    return replyArg;
                }
                else
                {
                    switch (replyCommand)
                    {
                    case 0x3d:
                        T_NOTIFICATION_METHOD::OnPlayFinished(replyArg);
                        break;

                    case 0x3F:
                        if (replyArg & 0x02)
                        {
                            _isOnline = true;
                            T_NOTIFICATION_METHOD::OnCardOnline(replyArg);
                        }
                        break;

                    case 0x3A:
                        if (replyArg & 0x02)
                        {
                            T_NOTIFICATION_METHOD::OnCardInserted(replyArg);
                        }
                        break;

                    case 0x3B:
                        if (replyArg & 0x02)
                        {
                            T_NOTIFICATION_METHOD::OnCardRemoved(replyArg);
                        }
                        break;

                    case 0x40:
                        T_NOTIFICATION_METHOD::OnError(replyArg);
                        break;

                    default:
                        // unknown/unsupported command reply
                        break;
                    }
                }
            }
            else
            {
                if (command != 0)
                {
                    T_NOTIFICATION_METHOD::OnError(DfMp3_Error_General);
                    if (_serial.available() == 0)
                    {
                        return 0;
                    }
                }
            }
        } while (command != 0);

        return 0;
    }

    uint16_t calcChecksum(uint8_t* packet)
    {
        uint16_t sum = 0;
        for (int i = DfMp3_Packet_Version; i < DfMp3_Packet_HiByteCheckSum; i++)
        {
            sum += packet[i];
        }
        return -sum;
    }

    void setChecksum(uint8_t* out)
    {
        uint16_t sum = calcChecksum(out);

        out[DfMp3_Packet_HiByteCheckSum] = (sum >> 8);
        out[DfMp3_Packet_LowByteCheckSum] = (sum & 0xff);
    }

    bool validateChecksum(uint8_t* in)
    {
        uint16_t sum = calcChecksum(in);
        return (sum == ((in[DfMp3_Packet_HiByteCheckSum] << 8) | in[DfMp3_Packet_LowByteCheckSum]));
    }
};
