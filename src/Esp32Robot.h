#ifndef __esp32robot_h__
#define __esp32robot_h__

#include <stdint.h>

#include <ESP32Servo.h>

#include "Animation.h"
#include "Types.h"

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

  Esp32Robot(void);
  Esp32Robot(const Esp32Robot &copy);
  virtual ~Esp32Robot(void);
  Esp32Robot &operator=(const Esp32Robot &other);

  virtual int8_t GetOffset(const Types::BodyParts &part, const Types::MovementMode &mode) const;

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetPin(const Types::BodyParts &part, const int8_t &pin);
  virtual void SetOffset(const Types::BodyParts &part, const Types::MovementMode &mode, const int8_t &offset);

  virtual void Update(const Types::BodyParts &part);
  virtual void Move(const Types::BodyParts &part, const int8_t &position);

  virtual Types::MovementMode GetMode(void) const;
  virtual void SetMode(const Types::MovementMode &mode);

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
    int8_t pins[Types::PART_PARTS];
    int8_t offsets_drive[Types::PART_PARTS];
    int8_t offsets_walk[Types::PART_PARTS];
    int8_t positions[Types::PART_PARTS];
    Servo servos[Types::PART_PARTS];

    Types::MovementMode mode;

    Animation animation;

    Data(void) :
      mode(Types::MOVE_DRIVE),
      animation(Animation()) {

      for (uint8_t i = 0; i < Types::PART_PARTS; i++) {
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
