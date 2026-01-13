#include "Esp32Robot.h"

const int8_t Esp32Robot::PIN_NOPIN = -1;
const int8_t Esp32Robot::OFFSET_DEFAULT = 0;

const int8_t Esp32Robot::DRIVE_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::DRIVE_ANKLE_DEFAULT = 45;
const int8_t Esp32Robot::DRIVE_ARM_DEFAULT = 45;

const int8_t Esp32Robot::WALK_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::WALK_ANKLE_DEFAULT = 125;
const int8_t Esp32Robot::WALK_ARM_DEFAULT = 45;

Esp32Robot::Esp32Robot(void) :
  data() {
}

Esp32Robot::Esp32Robot(const Esp32Robot &copy) :
  data() {

  for (int8_t i = 0; i < PART_PARTS; i++) {
    data.pins[i] = copy.data.pins[i];
    data.offsets[i] = copy.data.offsets[i];

    data.servos[i].detach();
    if (data.pins[i] != PIN_NOPIN) {
      data.servos[i].attach(data.pins[i]);
    }
  }
}

Esp32Robot::~Esp32Robot(void) {
}

Esp32Robot &Esp32Robot::operator=(const Esp32Robot &other) {
  if (&other != this) {
    for (int8_t i = 0; i < PART_PARTS; i++) {
      data.pins[i] = other.data.pins[i];
      data.offsets[i] = other.data.offsets[i];

      data.servos[i].detach();
      if (data.pins[i] != PIN_NOPIN) {
        data.servos[i].attach(data.pins[i]);
      }
    }
  }

  return (*this);
}

int8_t Esp32Robot::GetPin(const BodyParts &part) const {
  return (data.pins[part]);
}

int8_t Esp32Robot::GetOffset(const BodyParts &part) const {
  return (data.offsets[part]);
}

void Esp32Robot::SetPin(const BodyParts &part, const int8_t &pin) {
  if (pin != data.pins[part]) {
    data.pins[part] = pin;

    data.servos[part].detach();
    if (data.pins[part] != PIN_NOPIN) {
      data.servos[part].attach(data.pins[part]);
    }
  }
}

void Esp32Robot::SetOffset(const BodyParts &part, const int8_t &offset) {
  data.offsets[part] = offset;
}

void Esp32Robot::Move(const BodyParts &part, const int8_t &position) {
  data.servos[part].write(position);
}

Esp32Robot::MovementMode Esp32Robot::GetMode(void) const {
  return (data.mode);
}

void Esp32Robot::SetMode(const MovementMode &mode) {
  if (mode != data.mode) {
    data.mode = mode;

    switch (data.mode) {
      case MOVE_DRIVE:
        break;
      case MOVE_WALK:
        break;
    }
  }
}

void Esp32Robot::OnSetup(void) {
}

void Esp32Robot::OnLoop(void) {
}

void Esp32Robot::OnEnd(void) {
  for (int8_t i = 0; i < PART_PARTS; i++) {
    data.servos[i].detach();
  }
}

void Esp32Robot::OnModeChange(const uint8_t &mode) {
  Serial << "mode=" << mode << "\n";
}

void Esp32Robot::OnSpeedChange(const int8_t &speed) {
  Serial << "speed=" << speed << "\n";
}

void Esp32Robot::OnSteeringChange(const int8_t &steering) {
  Serial << "steering=" << steering << "\n";
}

void Esp32Robot::OnOffsetLeftChange(const int8_t &left_offset) {
  Serial << "offset left=" << left_offset << "\n";
}
void Esp32Robot::OnOffsetRightChange(const int8_t &right_offset) {
  Serial << "offset right=" << right_offset << "\n";
}
