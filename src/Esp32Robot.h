#ifndef __esp32robot_h__
#define __esp32robot_h__

#include <stdint.h>

#include <Arduino.h>
#include <BLEDevice.h>
#include <ESP32Servo.h>

class Esp32Robot {
public:
  static const int8_t PIN_NOPIN;
  static const int8_t OFFSET_DEFAULT;
  static const int8_t POSITION_DEFAULT;

  static const int8_t DRIVE_DRIVE_DEFAULT;
  static const int8_t DRIVE_ANKLE_LEFT_DEFAULT;
  static const int8_t DRIVE_ANKLE_RIGHT_DEFAULT;
  static const int8_t DRIVE_ARM_DEFAULT;

  static const int8_t WALK_DRIVE_DEFAULT;
  static const int8_t WALK_ANKLE_LEFT_DEFAULT;
  static const int8_t WALK_ANKLE_RIGHT_DEFAULT;
  static const int8_t WALK_ARM_DEFAULT;

  enum MovementMode {
    MOVE_UNDEFINED,
    MOVE_DRIVE,
    MOVE_WALK
  };

  enum BodyParts {
    PART_LEFT_DRIVE,
    PART_LEFT_ANKLE,
    PART_LEFT_ARM,
    PART_RIGHT_DRIVE,
    PART_RIGHT_ANKLE,
    PART_RIGHT_ARM,
    PART_PARTS
  };

  Esp32Robot(void);
  Esp32Robot(const Esp32Robot &copy);
  virtual ~Esp32Robot(void);
  Esp32Robot &operator=(const Esp32Robot &other);

  virtual int8_t GetPin(const BodyParts &part) const;
  virtual int8_t GetOffset(const BodyParts &part, const MovementMode &mode) const;

  virtual void SetPin(const BodyParts &part, const int8_t &pin);
  virtual void SetOffset(const BodyParts &part, const MovementMode &mode, const int8_t &offset);

  virtual void Update(const BodyParts &part);
  virtual void Move(const BodyParts &part, const int8_t &position);

  virtual MovementMode GetMode(void) const;
  virtual void SetMode(const MovementMode &mode);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

  virtual void OnModeChange(const uint8_t &mode);
  virtual void OnSpeedChange(const int8_t &speed);
  virtual void OnSteeringChange(const int8_t &steering);
  virtual void OnOffsetLeftChange(const int8_t &left_offset);
  virtual void OnOffsetRightChange(const int8_t &right_offset);

protected:
  struct Data {
    int8_t pins[PART_PARTS];
    int8_t offsets_drive[PART_PARTS];
    int8_t offsets_walk[PART_PARTS];
    int8_t positions[PART_PARTS];
    Servo servos[PART_PARTS];

    MovementMode mode;

    Data(void) :
      mode(MOVE_UNDEFINED) {

      for (uint8_t i = 0; i < PART_PARTS; i++) {
        pins[i] = PIN_NOPIN;
        offsets_drive[i] = OFFSET_DEFAULT;
        offsets_walk[i] = OFFSET_DEFAULT;
        positions[i] = POSITION_DEFAULT;
      }
    }
  };

  Data data;
};

#endif
