#ifndef __esp32robot_h__
#define __esp32robot_h__

#include <vector>

#include <Arduino.h>
#include <Servo.h>

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "ESP32Robot"

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = // 193 bytes V19
    {255, 7, 0, 2, 0, 186, 0, 19, 0, 0, 0, 0, 31, 1, 200, 84, 2, 3, 2, 8,
     0, 3, 143, 2, 19, 34, 2, 2, 26, 129, 163, 5, 26, 12, 64, 17, 100, 114, 105, 118,
     101, 0, 129, 163, 21, 25, 12, 64, 17, 119, 97, 108, 107, 0, 4, 17, 9, 11, 74, 48,
     2, 26, 4, 57, 41, 74, 11, 176, 2, 26, 129, 6, 1, 33, 12, 64, 17, 115, 112, 101,
     101, 100, 0, 129, 71, 1, 44, 12, 64, 17, 115, 116, 101, 101, 114, 105, 110, 103, 0, 131,
     156, 67, 40, 14, 2, 17, 2, 31, 99, 111, 110, 102, 105, 103, 117, 114, 101, 0, 6, 7,
     0, 131, 156, 67, 40, 14, 2, 17, 2, 31, 98, 97, 99, 107, 0, 9, 4, 4, 43, 74,
     11, 176, 2, 26, 129, 30, 3, 18, 12, 64, 17, 108, 101, 102, 116, 0, 4, 76, 43, 74,
     11, 176, 2, 26, 129, 99, 3, 25, 12, 64, 17, 114, 105, 103, 104, 116, 0, 67, 20, 20,
     40, 10, 125, 2, 26, 67, 93, 20, 40, 10, 125, 2, 26};

// this structure defines all the variables and events of your control interface
struct
{
  // input variables
  uint8_t main;               // =1 if page is visible, else =0, from 0 to 1
  uint8_t configure;          // =1 if page is visible, else =0, from 0 to 1
  uint8_t mode;               // from 0 to 2
  int8_t speed;               // from -100 to 100
  int8_t steering;            // from -100 to 100
  int8_t left_offset_slider;  // from -100 to 100
  int8_t right_offset_slider; // from -100 to 100

  // output variables
  int8_t left_offset;  // -128 .. 127
  int8_t right_offset; // -128 .. 127

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

class Esp32Robot
{
public:
  static const int8_t PIN_NOPIN;
  static const int8_t OFFSET_DEFAULT;

  static const int8_t DRIVE_DRIVE_DEFAULT;
  static const int8_t DRIVE_ANKLE_DEFAULT;
  static const int8_t DRIVE_ARM_DEFAULT;

  static const int8_t WALK_DRIVE_DEFAULT;
  static const int8_t WALK_ANKLE_DEFAULT;
  static const int8_t WALK_ARM_DEFAULT;

  enum MovementMode
  {
    MOVE_DRIVE,
    MOVE_WALK
  };

  enum BodyParts
  {
    PART_LEFT_DRIVE,
    PART_LEFT_ANKLE,
    PART_LEFT_ARM,
    PART_RIGHT_DRIVE,
    PART_RIGHT_ANKLE,
    PART_RIGHT_ARM,
    PART_PARTS
  };

  Esp32Robot(void);
  // Esp32Robot(const Esp32Robot &copy);
  virtual ~Esp32Robot(void);
  // Esp32Robot &operator=(const Esp32Robot &other);

  virtual int8_t GetPin(const BodyParts &part) const;
  virtual int8_t GetOffset(const BodyParts &part) const;

  virtual void SetPin(const BodyParts &part, const int8_t &pin);
  virtual void SetOffset(const BodyParts &part, const int8_t &offset);

  virtual void Move(const BodyParts &part, const int8_t &position);

  virtual MovementMode GetMode(void) const;
  virtual void SetMode(const MovementMode &mode);

  virtual void OnSetup(void) const;
  virtual void OnLoop(void) const;
  virtual void OnEnd(void) const;

protected:
  struct Data
  {
    std::vector<int8_t> pins;
    std::vector<int8_t> offsets;
    std::vector<Servo> servos;

    MovementMode mode;

    Data(void) : pins(PART_PARTS, PIN_NOPIN),
                 offsets(PART_PARTS, OFFSET_DEFAULT),
                 servos(PART_PARTS, Servo()),
                 mode(MOVE_DRIVE)
    {
    }
  };

  Data data;
}

#endif
