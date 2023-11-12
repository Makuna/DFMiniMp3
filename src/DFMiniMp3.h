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

#include "internal/queueSimple.h"
#include "DfMp3Types.h"
#include "internal/Mp3Packet.h"
#include "Mp3ChipBase.h"
#include "Mp3ChipOriginal.h"
#include "Mp3ChipMH2024K16SS.h"
#include "Mp3ChipIncongruousNoAck.h"


template <class T_SERIAL_METHOD, class T_NOTIFICATION_METHOD, class T_CHIP_VARIANT = Mp3ChipOriginal, uint32_t C_ACK_TIMEOUT = 900>
class DFMiniMp3
{
public:
    explicit DFMiniMp3(T_SERIAL_METHOD& serial) :
        _serial(serial),
        _comRetries(3), // default to three retries
        _isOnline(false),
#ifdef DfMiniMp3Debug
        _inTransaction(0),
#endif
        _queueNotifications(4) // default to 4 notifications in queue
    {
    }

    void begin(unsigned long baud = 9600)
    {
        _serial.begin(baud);
    }

    void begin(int8_t rxPin, int8_t txPin, unsigned long baud = 9600)
    {
        _serial.begin(baud, SERIAL_8N1, rxPin, txPin);
    }

    void setComRetries(uint8_t retries)
    {
        _comRetries = retries;
    }

    void loop()
    {
        // call all outstanding notifications
        while (abateNotification());

        // check for any new notifications in comms
        uint8_t maxDrains = 6;

        while (maxDrains &&
            _serial.available() >= static_cast<int>(sizeof(typename T_CHIP_VARIANT::ReceptionPacket)))
        {
            listenForReply(Mp3_Commands_None);
            maxDrains--;
        }
    }

    // Does not work with all models.
    // 0x3f reply overlaps the play source online notification
    // so this is why I suspect this has been deprecated and should
    // be avoided as there is not a way to tell a notification from a value return
    // YX5200-24SS - sends reply
    // MH2024K-24SS - sends NO reply --> results in error notification
    [[deprecated("Command in conflict with notification with no valid solution.")]]
    DfMp3_PlaySources getPlaySources()
    {
        return getCommand(Mp3_Commands_GetPlaySources).arg;
    }

    uint16_t getSoftwareVersion()
    {
        return getCommand(Mp3_Commands_GetSoftwareVersion).arg;
    }

    // the track as enumerated across all folders
    void playGlobalTrack(uint16_t track = 0)
    {
        setCommand(Mp3_Commands_PlayGlobalTrack, track);
    }

    // sd:/mp3/####track name
    void playMp3FolderTrack(uint16_t track)
    {
        setCommand(Mp3_Commands_PlayMp3FolderTrack, track);
    }

    // older devices: sd:/###/###track name
    // newer devices: sd:/##/###track name
    // folder and track numbers are zero padded
    void playFolderTrack(uint8_t folder, uint8_t track)
    {
        uint16_t arg = (folder << 8) | track;
        setCommand(Mp3_Commands_PlayFolderTrack, arg);
    }

    // sd:/##/####track name
    // track number must be four digits, zero padded
    void playFolderTrack16(uint8_t folder, uint16_t track)
    {
        uint16_t arg = (static_cast<uint16_t>(folder) << 12) | track;
        setCommand(Mp3_Commands_PlayFolderTrack16, arg);
    }

    void playRandomTrackFromAll()
    {
        setCommand(Mp3_Commands_PlayRandmomGlobalTrack);
    }

    void nextTrack()
    {
        setCommand(Mp3_Commands_PlayNextTrack);
    }

    void prevTrack()
    {
        setCommand(Mp3_Commands_PlayPrevTrack);
    }

    uint16_t getCurrentTrack(DfMp3_PlaySource source = DfMp3_PlaySource_Sd)
    {
        uint8_t command;

        switch (source)
        {
        case DfMp3_PlaySource_Usb:
            command = Mp3_Commands_GetUsbCurrentTrack;
            break;
        case DfMp3_PlaySource_Sd:
            command = Mp3_Commands_GetSdCurrentTrack;
            break;
        case DfMp3_PlaySource_Flash:
            command = Mp3_Commands_GetFlashCurrentTrack;
            break;
        default:
            command = Mp3_Commands_GetSdCurrentTrack;
            break;
        }

        return getCommand(command).arg;
    }

    // 0- 30
    void setVolume(uint8_t volume)
    {
        setCommand(Mp3_Commands_SetVolume, volume);
    }

    uint8_t getVolume()
    {
        return getCommand(Mp3_Commands_GetVolume).arg;
    }

    void increaseVolume()
    {
        setCommand(Mp3_Commands_IncVolume);
    }

    void decreaseVolume()
    {
        setCommand(Mp3_Commands_DecVolume);
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
        setCommand(Mp3_Commands_LoopGlobalTrack, globalTrack);
    }

    // sd:/##/*
    // 0-99
    void loopFolder(uint8_t folder)
    {
        setCommand(Mp3_Commands_LoopInFolder, folder);
    }

    // not well supported, use at your own risk
    void setPlaybackMode(DfMp3_PlaybackMode mode)
    {
        setCommand(Mp3_Commands_SetPlaybackMode, mode);
    }

    DfMp3_PlaybackMode getPlaybackMode()
    {
        return static_cast<DfMp3_PlaybackMode>(getCommand(Mp3_Commands_GetPlaybackMode).arg);
    }

    void setRepeatPlayAllInRoot(bool repeat)
    {
        setCommand(Mp3_Commands_RepeatPlayInRoot, !!repeat);
    }

    void setRepeatPlayCurrentTrack(bool repeat)
    {
        setCommand(Mp3_Commands_RepeatPlayCurrentTrack, !repeat);
    }

    void setEq(DfMp3_Eq eq)
    {
        setCommand(Mp3_Commands_SetEq, eq);
    }

    DfMp3_Eq getEq()
    {
        return static_cast<DfMp3_Eq>(getCommand(Mp3_Commands_GetEq).arg);
    }

    void setPlaybackSource(DfMp3_PlaySource source)
    {
        setCommand(Mp3_Commands_SetPlaybackSource, source);
    }

    void sleep()
    {
        setCommand(Mp3_Commands_Sleep);
    }

    void awake()
    {
        setCommand(Mp3_Commands_Awake);
    }

    void reset(bool waitForOnline = true)
    {
        setCommand(Mp3_Commands_Reset);

        _isOnline = false;
        while (waitForOnline && !_isOnline)
        {
            delay(1);
            loop();
        }
    }

    void start()
    {
        setCommand(Mp3_Commands_Start);
    }

    void pause()
    {
        setCommand(Mp3_Commands_Pause);
    }

    void stop()
    {
        setCommand(Mp3_Commands_Stop);
    }

    DfMp3_Status getStatus()
    {
        uint16_t reply = getCommand(Mp3_Commands_GetStatus).arg;

        DfMp3_Status status;
        status.source = static_cast<DfMp3_StatusSource>(reply >> 8);
        status.state = static_cast<DfMp3_StatusState>(reply & 0xff);

        return status;
    }

    uint16_t getFolderTrackCount(uint16_t folder)
    {
        return getCommand(Mp3_Commands_GetFolderTrackCount, folder).arg;
    }

    uint16_t getTotalTrackCount(DfMp3_PlaySource source = DfMp3_PlaySource_Sd)
    {
        uint8_t command;

        switch (source)
        {
        case DfMp3_PlaySource_Usb:
            command = Mp3_Commands_GetUsbTrackCount;
            break;
        case DfMp3_PlaySource_Sd:
            command = Mp3_Commands_GetSdTrackCount;
            break;
        case DfMp3_PlaySource_Flash:
            command = Mp3_Commands_GetFlashTrackCount;
            break;
        default:
            command = Mp3_Commands_GetSdTrackCount;
            break;
        }

        return getCommand(command).arg;
    }

    uint16_t getTotalFolderCount()
    {
        return getCommand(Mp3_Commands_GetTotalFolderCount).arg;
    }

    // sd:/advert/####track name
    void playAdvertisement(uint16_t track)
    {
        setCommand(Mp3_Commands_PlayAdvertTrack, track);
    }

    void stopAdvertisement()
    {
        setCommand(Mp3_Commands_StopAdvert);
    }

    void enableDac()
    {
        setCommand(Mp3_Commands_SetDacInactive, 0x00);
    }

    void disableDac()
    {
        setCommand(Mp3_Commands_SetDacInactive, 0x01);
    }

    bool isOnline() const
    {
        return _isOnline;
    }

private:
    struct reply_t
    {
        uint8_t command = 0;
        uint16_t arg = 0;

        bool isUndefined()
        {
            return (command == Mp3_Commands_None);
        }

#ifdef DfMiniMp3Debug
        void printReply() const
        {
            char formated[8];

            sprintf(formated, " %02x", command);
            DfMiniMp3Debug.print(formated);
            sprintf(formated, " %04x", arg);
            DfMiniMp3Debug.print(formated);
        }
#endif
    };

    const uint32_t c_AckTimeout = C_ACK_TIMEOUT;
    const uint32_t c_NoAckTimeout = 50; // 30ms observerd, added a little overhead

    T_SERIAL_METHOD& _serial;
    uint8_t _comRetries;
    volatile bool _isOnline;
#ifdef DfMiniMp3Debug
    int8_t _inTransaction;
#endif
    queueSimple_t<reply_t> _queueNotifications;

    void appendNotification(reply_t reply)
    {
        // store the notification for later calling so
        // current comms transactions can be finished
        // without interruption
        _queueNotifications.Enqueue(reply);
    }

    bool abateNotification()
    {
        // remove the first notification and call it
        reply_t reply;
        bool wasAbated = false;
        if (_queueNotifications.Dequeue(&reply))
        {
            callNotification(reply);
            wasAbated = true;
        }
        return wasAbated;
    }

    void callNotification(reply_t reply)
    {
        switch (reply.command)
        {
        case Mp3_Replies_TrackFinished_Usb: // usb
            T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Usb, reply.arg);
            break;

        case Mp3_Replies_TrackFinished_Sd: // micro sd
            T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Sd, reply.arg);
            break;

        case Mp3_Replies_TrackFinished_Flash: // flash
            T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Flash, reply.arg);
            break;

        case Mp3_Replies_PlaySource_Online:
            T_NOTIFICATION_METHOD::OnPlaySourceOnline(*this, static_cast<DfMp3_PlaySources>(reply.arg));
            break;

        case Mp3_Replies_PlaySource_Inserted:
            T_NOTIFICATION_METHOD::OnPlaySourceInserted(*this, static_cast<DfMp3_PlaySources>(reply.arg));
            break;

        case Mp3_Replies_PlaySource_Removed:
            T_NOTIFICATION_METHOD::OnPlaySourceRemoved(*this, static_cast<DfMp3_PlaySources>(reply.arg));
            break;

        case Mp3_Replies_Error: // error
            T_NOTIFICATION_METHOD::OnError(*this, reply.arg);
            break;

        default:
#ifdef DfMiniMp3Debug
            DfMiniMp3Debug.print("INVALID NOTIFICATION: ");
            reply.printReply();
            DfMiniMp3Debug.println();
#endif
            break;
        }
    }

    void drainResponses()
    {
        loop();
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
        *reply = {};

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

#ifdef DfMiniMp3Debug
        DfMiniMp3Debug.print("IN ");
        printRawPacket(reinterpret_cast<const uint8_t*>(&in), read);
        DfMiniMp3Debug.println();
#endif

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


#ifdef DfMiniMp3Debug
        if (_inTransaction != 0)
        {
            DfMiniMp3Debug.print("Rentrant? _inTransaction ");
            DfMiniMp3Debug.print(_inTransaction);
        }
        else
#endif
        {
            drainResponses();
        }

#ifdef DfMiniMp3Debug
        _inTransaction++;
#endif
        if (T_CHIP_VARIANT::commandSupportsAck(command))
        {
            // with ack support, 
            // we may retry if we don't get what we expected
            //
            _serial.setTimeout(c_AckTimeout);             
            do
            {
                sendPacket(command, arg, requestAck);
                reply = listenForReply(expectedCommand);
                retries--;
            } while (reply.command != expectedCommand && retries);
        }
        else
        {
            // without ack support, 
            // we may retry only if we get an error
            //
            _serial.setTimeout(c_NoAckTimeout);
            do
            {
                sendPacket(command, arg, requestAck);
                reply = listenForReply(expectedCommand);
                retries--;
            } while (reply.command == Mp3_Replies_Error && retries);
        }
#ifdef DfMiniMp3Debug
        _inTransaction--;
#endif

        if (reply.command == Mp3_Replies_Error)
        {
            T_NOTIFICATION_METHOD::OnError(*this, reply.arg);
            reply = {};
        }
        
        return reply;
    }

    reply_t getCommand(uint8_t command, uint16_t arg = 0)
    {
        return retryCommand(command, command, arg);
    }

    void setCommand(uint8_t command, uint16_t arg = 0)
    {
        retryCommand(command, Mp3_Replies_Ack, arg, true);
    }

    reply_t listenForReply(uint8_t command)
    {
        reply_t reply;

        while (readPacket(&reply))
        {
            switch (reply.command)
            {
            case Mp3_Replies_PlaySource_Online: // play source online
            case Mp3_Replies_PlaySource_Inserted: // play source inserted
            case Mp3_Replies_PlaySource_Removed: // play source removed
                _isOnline = true;
                appendNotification(reply);
                break;

            case Mp3_Replies_TrackFinished_Usb: // usb
            case Mp3_Replies_TrackFinished_Sd: // micro sd
            case Mp3_Replies_TrackFinished_Flash: // flash
                appendNotification(reply);
                break;

            case Mp3_Replies_Error: // error
                if (command == Mp3_Commands_None)
                {
                    appendNotification(reply);
                }
                else
                {
                    return reply;
                }
                break;

            case Mp3_Replies_Ack: // ack
            default:
                if (command != Mp3_Commands_None)
                {
                    return reply;
                }
                break;
            }

            // for not specific listen, only drain
            // one message at a time
            if (command == Mp3_Commands_None)
            {
                break;
            }
        }


        return {};
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
