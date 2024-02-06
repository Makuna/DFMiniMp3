// this example will play a random track from all on the sd 
//
// it expects the sd card to contain some mp3 files

#include <DFMiniMp3.h>

// define a handy type using hardware serial with no notifications
//
typedef DFMiniMp3<HardwareSerial> DfMp3; 

// instance a DfMp3 object, 
//
DfMp3 dfmp3(Serial1);

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
//SoftwareSerial secondarySerial(10, 11); // RX, TX
//typedef DFMiniMp3<SoftwareSerial> DfMp3;
// DfMp3 dfmp3(secondarySerial);

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

  // show some properties and set the volume
  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  dfmp3.setVolume(24);
  
  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);

  uint16_t mode = dfmp3.getPlaybackMode();
  Serial.print("playback mode ");
  Serial.println(mode);
  
  Serial.println("starting...");
  
  dfmp3.playRandomTrackFromAll(); // random of all folders on sd
}

void loop() 
{
  // calling dfmp3.loop() periodically allows for notifications 
  // to be handled without interrupts
  dfmp3.loop();
}
