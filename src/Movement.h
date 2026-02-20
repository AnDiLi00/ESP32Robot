#ifndef __movement_h__
#define __movement_h__

#include <ESP32Servo.h>

#include "Types.h"

class Movement {
public:
  static const int8_t PIN_NOPIN;
  static const int8_t OFFSET_DEFAULT;
  static const uint8_t POSITION_DEFAULT;

  static const uint8_t SERVO_CENTER;
  static const uint8_t SERVO_RANGE_STEPS;
  static const uint8_t SERVO_RANGE_SPEED_DRIVE;
  static const uint8_t SERVO_RANGE_SPEED_WALK;
  static const uint8_t SERVO_RANGE_STEERING;

  static const uint8_t DRIVE_DRIVE_DEFAULT;
  static const uint8_t DRIVE_ANKLE_LEFT_DEFAULT;
  static const uint8_t DRIVE_ANKLE_RIGHT_DEFAULT;
  static const uint8_t DRIVE_ARM_DEFAULT;

  static const uint8_t WALK_DRIVE_DEFAULT;
  static const uint8_t WALK_ANKLE_LEFT_DEFAULT;
  static const uint8_t WALK_ANKLE_RIGHT_DEFAULT;
  static const uint8_t WALK_ANKLE_LEFT_WALK;
  static const uint8_t WALK_ANKLE_LEFT_WALKRIGHT;
  static const uint8_t WALK_ANKLE_RIGHT_WALK;
  static const uint8_t WALK_ANKLE_RIGHT_WALKLEFT;
  static const uint8_t WALK_ARM_DEFAULT;

  static const unsigned long TIME_MOVE;

  Movement(void);
  Movement(const Movement &copy);
  virtual ~Movement(void);
  Movement &operator=(const Movement &other);

  virtual void SetPin(const Types::BodyParts &part, const int8_t &pin);
  virtual void SetOffset(const Types::BodyParts &part, const Types::MovementMode &mode, const int8_t &offset);
  virtual void SetMode(const Types::MovementMode &mode);

  virtual void OnSetup(void);
  virtual void OnLoop(const unsigned long &now);
  virtual void OnEnd(void);

  virtual void OnModeChange(const uint8_t &mode);
  virtual void OnSpeedChange(const int8_t &speed);
  virtual void OnSteeringChange(const int8_t &steering);
  virtual void OnOffsetLeftChange(const int8_t &left_offset);
  virtual void OnOffsetRightChange(const int8_t &right_offset);

protected:
  virtual void UpdateDriving(const unsigned long &now);
  virtual void UpdateWalking(const unsigned long &now);

  virtual void Update(const Types::BodyParts &part);
  virtual void Move(const Types::BodyParts &part, const uint8_t &position);

  virtual int8_t GetOffset(const Types::BodyParts &part, const Types::MovementMode &mode) const;

  virtual void PrintMode(void) const;

  struct Data {
    int8_t pins[Types::PART_PARTS];
    int8_t offsets_drive[Types::PART_PARTS];
    int8_t offsets_walk[Types::PART_PARTS];
    uint8_t positions[Types::PART_PARTS];
    Servo servos[Types::PART_PARTS];

    Types::MovementMode mode;
    Types::MovementSubMode mode_sub;

    unsigned long last_move;

    int8_t steering;
    int8_t speed;

    Data(void) :
      mode(Types::MOVE_DRIVE),
      mode_sub(Types::MSUB_DRIVE),
      last_move(Types::TIME_DEFAULT),
      steering(0),
      speed(0) {
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
