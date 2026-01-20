#include <Adafruit_SSD1306.h>
#include <Esp32Robot.h>
#include <RemoteXY.h>

// Display setup
Adafruit_SSD1306 display(Eyes::DISPLAY_WIDTH, Eyes::DISPLAY_HEIGHT, &Wire, Eyes::DISPLAY_RESET);

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = {
  255, 5, 0, 0, 0, 68, 0, 19, 0, 0, 0, 82, 111, 98, 111, 0, 31, 1, 200, 84,
  1, 1, 5, 0, 3, 184, 5, 11, 20, 2, 2, 26, 5, 14, 15, 60, 60, 32, 2, 26,
  31, 5, 100, 16, 60, 60, 52, 2, 26, 31, 129, 165, 7, 18, 8, 64, 17, 100, 114, 105,
  118, 101, 0, 129, 166, 16, 17, 8, 64, 17, 119, 97, 108, 107, 0
};

struct {
  // input variables
  uint8_t mode; // from 0 to 2
  int8_t unused01; // from -100 to 100
  int8_t speed; // from -100 to 100
  int8_t steering; // from -100 to 100
  int8_t unused02; // from -100 to 100

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

Esp32Robot Esp32Robot;

void RemoteXY_mode_event() {
  Esp32Robot.OnModeChange(RemoteXY.mode);
}

void RemoteXY_speed_event() {
  Esp32Robot.OnSpeedChange(RemoteXY.speed);
}

void RemoteXY_steering_event() {
  Esp32Robot.OnSteeringChange(RemoteXY.steering);
}

void setup() {
  randomSeed(12345674890);

  Serial.begin(115200);

  RemoteXYGui *gui = RemoteXYEngine.addGui(RemoteXY_CONF_PROGMEM, &RemoteXY);
  gui->addConnection(new CRemoteXYStream_BLEDevice("ESP32Robot"));

  RemoteXYEngine.addVariableEvent(RemoteXY.mode, RemoteXY_mode_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.speed, RemoteXY_speed_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.steering, RemoteXY_steering_event);
  // set remotexy mode to 'walk' since robot is set to walk
  RemoteXY.mode = 1;

  // Esp32Robot Setup
  Esp32Robot.SetDisplay(&display);
  Esp32Robot.SetPin(Types::PART_LEFT_DRIVE, 5);
  Esp32Robot.SetPin(Types::PART_RIGHT_DRIVE, 23);
  Esp32Robot.SetPin(Types::PART_LEFT_ANKLE, 19);
  Esp32Robot.SetPin(Types::PART_RIGHT_ANKLE, 18);
  Esp32Robot.SetOffset(Types::PART_LEFT_ANKLE, Types::MOVE_WALK, 0);
  Esp32Robot.SetOffset(Types::PART_RIGHT_ANKLE, Types::MOVE_WALK, 2);
  Esp32Robot.SetOffset(Types::PART_LEFT_ANKLE, Types::MOVE_DRIVE, 0);
  Esp32Robot.SetOffset(Types::PART_RIGHT_ANKLE, Types::MOVE_DRIVE, 0);
  Esp32Robot.SetMode(Types::MOVE_WALK);

  Esp32Robot.OnSetup();
}

void loop() {
  RemoteXYEngine.handler();

  Esp32Robot.OnLoop();
}
