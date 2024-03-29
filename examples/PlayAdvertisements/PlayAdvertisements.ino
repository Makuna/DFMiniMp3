// this example will play a track and then every 60 seconds
// it will play an advertisement
//
// it expects the sd card to contain the following mp3 files
// but doesn't care whats in them
//
// sd:/01/001.mp3 - the song to play, the longer the better
// sd:/advert/0001.mp3 - the advertisement to interrupt the song, keep it short

#include <DFMiniMp3.h>

// implement a static notification class,
// its member methods will get called 
//
class Mp3Notify
{
public:
    // required type
    typedef void TargetType;

    // required method even though it doesn't do anything
    static void SetTarget(TargetType*)
    {
    }

    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
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

    // required method
    static void OnError(uint16_t errorCode)
    {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    // required method
    static void OnPlayFinished([[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
    {
        Serial.print("Play finished for #");
        Serial.println(track);
    }

    // required method
    static void OnPlaySourceOnline(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "online");
    }

    // required method
    static void OnPlaySourceInserted(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "inserted");
    }

    // required method
    static void OnPlaySourceRemoved(DfMp3_PlaySources source)
    {
        PrintlnSourceAction(source, "removed");
    }
};

// define a handy type using serial and our notify class
//
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 

// instance a DfMp3 object, 
//
DfMp3 dfmp3(Serial1);

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
//SoftwareSerial secondarySerial(10, 11); // RX, TX
//typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
// DfMp3 dfmp3(secondarySerial);


uint32_t lastAdvert; // track time for last advertisement

void setup() 
{
  Serial.begin(115200);

  Serial.println("initializing...");
  
  dfmp3.begin();
  // for boards that support hardware arbitrary pins
  // dfmp3.begin(10, 11); // RX, TX

  // during development, it's a good practice to put the module
  // into a known state by calling reset().  
  // You may hear popping when starting and you can remove this 
  // call to reset() once your project is finalized
  dfmp3.reset();

  uint16_t version = dfmp3.getSoftwareVersion();
  Serial.print("version ");
  Serial.println(version);

  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume was ");
  Serial.println(volume);
  dfmp3.setVolume(24);
  volume = dfmp3.getVolume();
  Serial.print(" and changed to  ");
  Serial.println(volume);
  
  Serial.println("track 1 from folder 1"); 
  dfmp3.playFolderTrack(1, 1); // sd:/01/001.mp3

  lastAdvert = millis();
}

void loop() 
{
  uint32_t now = millis();
  if ((now - lastAdvert) > 60000)
  {
    // interrupt the song and play the advertisement, it will
    // return to the song when its done playing automatically
    dfmp3.playAdvertisement(1); // sd:/advert/0001.mp3
    lastAdvert = now;
  }
  dfmp3.loop();
}
