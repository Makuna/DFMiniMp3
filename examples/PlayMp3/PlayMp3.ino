// this example will play a track and then 
// every five seconds play another track
//
// it expects the sd card to contain these three mp3 files
// but doesn't care whats in them
//
// sd:/mp3/0001.mp3
// sd:/mp3/0002.mp3
// sd:/mp3/0003.mp3

#include <DFMiniMp3.h>

// forward declare the sketches managing class, just the name
//
class Mp3Manager;

// define a handy type using hardware serial and our notify class wrapper
//
typedef DFMiniMp3<HardwareSerial, DfMp3Callback<Mp3Manager>> DfMp3;

// Some arduino boards only have one hardware serial port, 
// so a software serial port is needed instead.
// use this instead to define the DFMiniMp3
//SoftwareSerial secondarySerial(10, 11); // RX, TX
//typedef DFMiniMp3<SoftwareSerial, DfMp3Callback<Mp3Manager>> DfMp3;

// implement the sketches managing class,
// this is the authors class doing what funtionality the author wants
// it must implement the methods as defined below and these member
// methods will get called by the Mp3 class for ayncrounous notifications
//
class Mp3Manager
{
public:
    Mp3Manager() :
        _dfmp3(Serial1, this) // NOTE: passing this object as the notification target
//      _dfmp3(secondarySerial, this); // for software serial
    {
    }

    ~Mp3Manager()
    {
    }

    // not required but common practice 
    // as _dfmp3.begin() must get called
    //
    void begin()
    {
        _dfmp3.begin();
        // for boards that support hardware arbitrary pins
        // _dfmp3.begin(10, 11); // RX, TX

        // during development, it's a good practice to put the module
        // into a known state by calling reset().  
        // You may hear popping when starting and you can remove this 
        // call to reset() once your project is finalized
        _dfmp3.reset();

        // some example things you might want to do
        uint16_t version = _dfmp3.getSoftwareVersion();
        Serial.print("version ");
        Serial.println(version);

        uint16_t volume = _dfmp3.getVolume();
        Serial.print("volume ");
        Serial.println(volume);
        _dfmp3.setVolume(24);

        uint16_t count = _dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
        Serial.print("files ");
        Serial.println(count);

        Serial.println("starting...");

        // start the first track playing
        _dfmp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
    }

    // not required but common practice 
    // as _dfmp3.loop() must get called
    //
    void loop()
    {
        _dfmp3.loop();
    }

    // required method
    void OnError(uint16_t errorCode)
    {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    // required method
    void OnPlayFinished([[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
    {
        Serial.print("Play finished for #");
        Serial.println(track);

        // start next track
        track += 1;
        // this example will just start back over with 1 after track 3
        if (track > 3)
        {
            track = 1;
        }
        _dfmp3.playMp3FolderTrack(track);  // sd:/mp3/0001.mp3, sd:/mp3/0002.mp3, sd:/mp3/0003.mp3
    }

    // required method
    void OnPlaySourceOnline(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "online");
    }

    // required method
    void OnPlaySourceInserted(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "inserted");
    }

    // required method
    void OnPlaySourceRemoved(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "removed");
    }

protected:
    // sketch specific example member, not required
    void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
    {
        if (source & DfMp3_PlaySources_Sd)
        {
            Serial.print("SD Card, ");
        }
        if (source & DfMp3_PlaySources_Usb)
        {
            Serial.print("USB Disk, ");
        }
        if (source & DfMp3_PlaySources_Flash)
        {
            Serial.print("Flash, ");
        }
        Serial.println(action);
    }

    DfMp3 _dfmp3;
};

Mp3Manager mp3; // instance your class per your design

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    mp3.begin();
}

void waitMilliseconds(uint16_t msWait)
{
    uint32_t start = millis();

    while ((millis() - start) < msWait)
    {
        // if you have loops with delays, its important to 
        // call dfmp3.loop() periodically so it allows for notifications 
        // to be handled without interrupts
        mp3.loop();
        delay(1);
    }
}

void loop()
{
    waitMilliseconds(100);
}
