

#define MP3_RX_PIN 15
#define MP3_TX_PIN 2

//#include <SoftwareSerial.h>
//SoftwareSerial Mp3Serial(MP3_RX_PIN, MP3_TX_PIN); // RX, TX
//#define Mp3SerialType SoftwareSerial

#define Mp3SerialType HardwareSerial
#define Mp3Serial Serial1

#define DebugOut Serial
#define DfMiniMp3Debug DebugOut

#include <DFMiniMp3.h>

// forward declare the notify class, just the name
//
class Mp3Notify; 

// define a handy type using serial and our notify class
//
typedef DFMiniMp3<Mp3SerialType, Mp3Notify> DfMp3;

class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        DebugOut.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        DebugOut.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        DebugOut.print("Flash, ");
    }
    DebugOut.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    DebugOut.println();
    DebugOut.print("Com Error ");
    DebugOut.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
    DebugOut.print("Play finished for #");
    DebugOut.println(track);  
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};


DfMp3 mp3(Mp3Serial);

uint32_t setupStart;

void setup() 
{
  DebugOut.begin(115200);
  
  //mp3.begin(); // when you can't set the pins OR software serial is used
  mp3.begin(MP3_RX_PIN, MP3_TX_PIN); // rx, tx
  
  // use hardware serial Serial for MP3 playback on ESP8266
  // move serial to GPIO15(TX) and GPIO13 (RX) for
  // the mp3 module, we loose the debug monitor though
  //Serial.swap();

 // mp3.awake();
  mp3.reset(); 


  DebugOut.println("initializing...");


  setupStart = millis();
}

const uint16_t c_playDelay = 1000;

void loop() 
{
  mp3.loop();
  DebugOut.println("starting...");
  

  uint16_t version = mp3.getSoftwareVersion();
  DebugOut.print("version ");
  DebugOut.println(version, HEX);

  DebugOut.println("playGlobalTrack 6");
  mp3.playGlobalTrack(6);
  delay(c_playDelay);

  DebugOut.println("playMp3FolderTrack 2");
  mp3.playMp3FolderTrack(2);
  delay(c_playDelay);

  DebugOut.println("playFolderTrack 20, 2");
  mp3.playFolderTrack(20, 2);
  delay(c_playDelay);

  DebugOut.println("playFolderTrack16 10, 2000");
  mp3.playFolderTrack16(10, 2000);
  delay(c_playDelay);

  DebugOut.println("playRandomTrackFromAll");
  mp3.playRandomTrackFromAll();
  delay(c_playDelay);

  DebugOut.println("nextTrack");
  mp3.nextTrack();
  delay(c_playDelay);

  DebugOut.println("prevTrack");
  mp3.prevTrack();
  delay(c_playDelay);

  uint16_t track = mp3.getCurrentTrack();
  DebugOut.print("current track ");
  DebugOut.println(track);

  DebugOut.println("setVolume 16");
  mp3.setVolume(16);

  uint16_t volume = mp3.getVolume();
  DebugOut.print("volume ");
  DebugOut.println(volume);

  DebugOut.println("increaseVolume");
  mp3.increaseVolume();

  DebugOut.println("decreaseVolume");
  mp3.decreaseVolume();

  DebugOut.println("loopGlobalTrack 6");
  mp3.loopGlobalTrack(6);
  delay(c_playDelay);

  DebugOut.println("loopFolder 20");
  mp3.loopFolder(20);
  delay(c_playDelay);

  DebugOut.println("setPlaybackMode 2 (single repeat)");
  mp3.setPlaybackMode(DfMp3_PlaybackMode_SingleRepeat);

  DfMp3_PlaybackMode mode = mp3.getPlaybackMode();
  DebugOut.print("playback mode ");
  DebugOut.println(mode);

  DebugOut.println("setRepeatPlayAllInRoot false");
  mp3.setRepeatPlayAllInRoot(false);
  delay(c_playDelay);

  DebugOut.println("setRepeatPlayCurrentTrack true");
  mp3.setRepeatPlayCurrentTrack(true);
  delay(c_playDelay);

  DebugOut.println("setEq 3 (jazz)");
  mp3.setEq(DfMp3_Eq_Jazz);

  DfMp3_Eq eqMode = mp3.getEq();
  DebugOut.print("eq mode ");
  DebugOut.println(eqMode);

  DebugOut.println("setPlaybackSource 2 (SD)");
  mp3.setPlaybackSource(DfMp3_PlaySource_Sd);
  delay(c_playDelay);

  DebugOut.println("start");
  mp3.start();
  delay(c_playDelay);

  DebugOut.println("pause");
  mp3.pause();
  delay(c_playDelay);

  DebugOut.println("stop");
  mp3.stop();
  delay(c_playDelay);

  DfMp3_Status status = mp3.getStatus();
  DebugOut.print("status source is ");
  DebugOut.print(status.source);
  DebugOut.print(" and state is ");
  DebugOut.println(status.state);

  uint16_t count = mp3.getFolderTrackCount(10);
  DebugOut.print("folder 10 files ");
  DebugOut.println(count);

  count = mp3.getTotalTrackCount();
  DebugOut.print("total files ");
  DebugOut.println(count);

  count = mp3.getTotalFolderCount();
  DebugOut.print("total folders ");
  DebugOut.println(count);

  DebugOut.println("playAdvertisement 2");
  mp3.playAdvertisement(2);
  delay(c_playDelay);

  DebugOut.println("stopAdvertisement");
  mp3.stopAdvertisement();
  delay(c_playDelay);

  DebugOut.println("enableDac");
  mp3.enableDac();

  DebugOut.println("disableDac");
  mp3.disableDac();

  mp3.loop();
  DebugOut.println("...stopping");
  DebugOut.println();
  mp3.loop();
  delay(2000);
}
