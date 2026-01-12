#include <Esp32Robot.h>
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = {
  255, 7, 0, 2, 0, 186, 0, 19, 0, 0, 0, 0, 31, 1, 200, 84, 2, 3, 2, 8,
  0, 3, 143, 2, 19, 34, 2, 2, 26, 129, 163, 5, 26, 12, 64, 17, 100, 114, 105, 118,
  101, 0, 129, 163, 21, 25, 12, 64, 17, 119, 97, 108, 107, 0, 4, 17, 9, 11, 74, 48,
  2, 26, 4, 57, 41, 74, 11, 176, 2, 26, 129, 6, 1, 33, 12, 64, 17, 115, 112, 101,
  101, 100, 0, 129, 71, 1, 44, 12, 64, 17, 115, 116, 101, 101, 114, 105, 110, 103, 0, 131,
  156, 67, 40, 14, 2, 17, 2, 31, 99, 111, 110, 102, 105, 103, 117, 114, 101, 0, 6, 7,
  0, 131, 156, 67, 40, 14, 2, 17, 2, 31, 98, 97, 99, 107, 0, 9, 4, 4, 43, 74,
  11, 176, 2, 26, 129, 30, 3, 18, 12, 64, 17, 108, 101, 102, 116, 0, 4, 76, 43, 74,
  11, 176, 2, 26, 129, 99, 3, 25, 12, 64, 17, 114, 105, 103, 104, 116, 0, 67, 20, 20,
  40, 10, 125, 2, 26, 67, 93, 20, 40, 10, 125, 2, 26
};

struct {
  uint8_t main; // =1 if page is visible, else =0, from 0 to 1
  uint8_t configure; // =1 if page is visible, else =0, from 0 to 1
  uint8_t mode; // from 0 to 2
  int8_t speed; // from -100 to 100
  int8_t steering; // from -100 to 100
  int8_t left_offset_slider; // from -100 to 100
  int8_t right_offset_slider; // from -100 to 100

  // output variables
  int8_t left_offset; // -128 .. 127
  int8_t right_offset; // -128 .. 127

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

void RemoteXY_OnModeChange() {
}

void RemoteXY_OnSpeedChange() {
}

void RemoteXY_OnSteeringChange() {
}

void RemoteXY_OnOffsetLeftChange() {
}

void RemoteXY_OnOffsetRightChange() {
}

Esp32Robot Esp32Robot;

void setup() {
  Serial.begin(115200);

  RemoteXYGui *gui = RemoteXYEngine.addGui(RemoteXY_CONF_PROGMEM, &RemoteXY);
  gui->addConnection(new CRemoteXYStream_BLEDevice("ESP32Robot"));

  RemoteXYEngine.addVariableEvent(RemoteXY.mode, RemoteXY_OnModeChange);
  RemoteXYEngine.addVariableEvent(RemoteXY.speed, RemoteXY_OnSpeedChange);
  RemoteXYEngine.addVariableEvent(RemoteXY.steering, RemoteXY_OnSteeringChange);
  RemoteXYEngine.addVariableEvent(RemoteXY.left_offset_slider, RemoteXY_OnOffsetLeftChange);
  RemoteXYEngine.addVariableEvent(RemoteXY.right_offset_slider, RemoteXY_OnOffsetRightChange);
  
  Esp32Robot.OnSetup();
}

void loop() {
  RemoteXYEngine.handler();
  
  Esp32Robot.OnLoop();
}
