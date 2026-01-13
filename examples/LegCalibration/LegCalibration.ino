#include <Esp32Robot.h>
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = {
  255, 2, 0, 2, 0, 74, 0, 19, 0, 0, 0, 82, 111, 98, 111, 0, 31, 1, 200, 84,
  1, 1, 6, 0, 4, 8, 46, 81, 11, 160, 2, 26, 4, 103, 46, 81, 11, 160, 2, 26,
  67, 29, 24, 40, 10, 93, 2, 26, 67, 123, 24, 40, 10, 93, 2, 26, 129, 39, 15, 21,
  9, 64, 17, 76, 69, 70, 84, 0, 129, 129, 15, 26, 9, 64, 17, 82, 73, 71, 72, 84, 0
};

struct {
  // input variables
  int8_t offset_left; // from -100 to 100
  int8_t offset_right; // from -100 to 100

  // output variables
  int8_t value_left; // -128 .. 127
  int8_t value_right; // -128 .. 127

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

Esp32Robot Esp32Robot;

void RemoteXY_offset_left_event() {
  Esp32Robot.OnOffsetLeftChange(RemoteXY.offset_left);
  RemoteXY.value_left = RemoteXY.offset_left;
}

void RemoteXY_offset_right_event() {
  Esp32Robot.OnOffsetRightChange(RemoteXY.offset_right);
  RemoteXY.value_right = RemoteXY.offset_right;
}

void setup() {
  Serial.begin(115200);

  RemoteXYGui *gui = RemoteXYEngine.addGui(RemoteXY_CONF_PROGMEM, &RemoteXY);
  gui->addConnection(new CRemoteXYStream_BLEDevice("ESP32Robot"));

  RemoteXYEngine.addVariableEvent(RemoteXY.offset_left, RemoteXY_offset_left_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.offset_right, RemoteXY_offset_right_event);

  Esp32Robot.OnSetup();
}

void loop() {
  RemoteXYEngine.handler();

  Esp32Robot.OnLoop();
}
