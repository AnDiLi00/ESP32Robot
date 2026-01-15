#include "Esp32Robot.h"

const int8_t Esp32Robot::PIN_NOPIN = -1;
const int8_t Esp32Robot::OFFSET_DEFAULT = 0;
const int8_t Esp32Robot::POSITION_DEFAULT = 0;

const int8_t Esp32Robot::DRIVE_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::DRIVE_ANKLE_LEFT_DEFAULT = 125;
const int8_t Esp32Robot::DRIVE_ANKLE_RIGHT_DEFAULT = 45;
const int8_t Esp32Robot::DRIVE_ARM_DEFAULT = 45;

const int8_t Esp32Robot::WALK_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::WALK_ANKLE_LEFT_DEFAULT = 45;
const int8_t Esp32Robot::WALK_ANKLE_RIGHT_DEFAULT = 125;
const int8_t Esp32Robot::WALK_ARM_DEFAULT = 45;

Esp32Robot::Esp32Robot(void) :
  Esp32RobotEyes(),
  data() {
}

Esp32Robot::Esp32Robot(const Esp32Robot &copy) :
  Esp32RobotEyes(copy),
  data() {

  for (int8_t i = 0; i < PART_PARTS; i++) {
    data.pins[i] = copy.data.pins[i];
    data.offsets_drive[i] = copy.data.offsets_drive[i];
    data.offsets_walk[i] = copy.data.offsets_walk[i];
    data.positions[i] = copy.data.positions[i];

    data.servos[i].detach();
    if (data.pins[i] != PIN_NOPIN) {
      data.servos[i].attach(data.pins[i]);
    }
  }

  data.mode = copy.data.mode;
}

Esp32Robot::~Esp32Robot(void) {
}

Esp32Robot &Esp32Robot::operator=(const Esp32Robot &other) {
  if (&other != this) {
    Esp32RobotEyes::operator=(other);

    for (int8_t i = 0; i < PART_PARTS; i++) {
      data.pins[i] = other.data.pins[i];
      data.offsets_drive[i] = other.data.offsets_drive[i];
      data.offsets_walk[i] = other.data.offsets_walk[i];
      data.positions[i] = other.data.positions[i];

      data.servos[i].detach();
      if (data.pins[i] != PIN_NOPIN) {
        data.servos[i].attach(data.pins[i]);
      }
    }

    data.mode = other.data.mode;
  }

  return (*this);
}

int8_t Esp32Robot::GetPin(const BodyParts &part) const {
  return (data.pins[part]);
}

int8_t Esp32Robot::GetOffset(const BodyParts &part, const MovementMode &mode) const {
  int8_t ret = OFFSET_DEFAULT;

  switch (mode) {
    case MOVE_UNDEFINED:
      break;
    case MOVE_DRIVE:
      ret = data.offsets_drive[part];
      break;
    case MOVE_WALK:
      ret = data.offsets_walk[part];
      break;
  }

  return (ret);
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

void Esp32Robot::SetOffset(const BodyParts &part, const MovementMode &mode, const int8_t &offset) {
  switch (mode) {
    case MOVE_UNDEFINED:
      break;
    case MOVE_DRIVE:
      data.offsets_drive[part] = offset;
      break;
    case MOVE_WALK:
      data.offsets_walk[part] = offset;
      break;
  }
}

void Esp32Robot::Update(const BodyParts &part) {
  Move(part, data.positions[part]);
}

void Esp32Robot::Move(const BodyParts &part, const int8_t &position) {
  data.positions[part] = position;
  data.servos[part].write(position + GetOffset(part, data.mode));
}

Esp32Robot::MovementMode Esp32Robot::GetMode(void) const {
  return (data.mode);
}

void Esp32Robot::SetMode(const MovementMode &mode) {
  if ((mode != data.mode) && (mode != MOVE_UNDEFINED)) {
    data.mode = mode;

    switch (data.mode) {
      case MOVE_UNDEFINED:
        Serial.println("mode=undefined");
        break;
      case MOVE_DRIVE:
        Serial.println("mode=drive");
        Move(PART_LEFT_DRIVE, DRIVE_DRIVE_DEFAULT);
        Move(PART_RIGHT_DRIVE, DRIVE_DRIVE_DEFAULT);
        Move(PART_LEFT_ANKLE, DRIVE_ANKLE_LEFT_DEFAULT);
        Move(PART_RIGHT_ANKLE, DRIVE_ANKLE_RIGHT_DEFAULT);
        break;
      case MOVE_WALK:
        Serial.println("mode=walk");
        Move(PART_LEFT_DRIVE, WALK_DRIVE_DEFAULT);
        Move(PART_RIGHT_DRIVE, WALK_DRIVE_DEFAULT);
        Move(PART_LEFT_ANKLE, WALK_ANKLE_LEFT_DEFAULT);
        Move(PART_RIGHT_ANKLE, WALK_ANKLE_RIGHT_DEFAULT);
        break;
    }
  }
}

void Esp32Robot::OnSetup(void) {
  Esp32RobotEyes::OnSetup();
}

void Esp32Robot::OnLoop(void) {
  Esp32RobotEyes::OnLoop();
}

void Esp32Robot::OnEnd(void) {
  for (int8_t i = 0; i < PART_PARTS; i++) {
    data.servos[i].detach();
  }

  Esp32RobotEyes::OnEnd();
}

void Esp32Robot::OnModeChange(const uint8_t &mode) {
  Serial.print("OnModeChange::mode=");
  Serial.println(mode);
}

void Esp32Robot::OnSpeedChange(const int8_t &speed) {
  Serial.print("OnSpeedChange::speed=");
  Serial.println(speed);
}

void Esp32Robot::OnSteeringChange(const int8_t &steering) {
  Serial.print("OnSteeringChange::steering=");
  Serial.println(steering);
}

void Esp32Robot::OnOffsetLeftChange(const int8_t &left_offset) {
  Serial.print("OnOffsetLeftChange::offset=");
  Serial.println(left_offset);

  SetOffset(PART_LEFT_ANKLE, data.mode, left_offset);
  Update(PART_LEFT_ANKLE);
}

void Esp32Robot::OnOffsetRightChange(const int8_t &right_offset) {
  Serial.print("OnOffsetRightChange::offset=");
  Serial.println(right_offset);

  SetOffset(PART_RIGHT_ANKLE, data.mode, right_offset);
  Update(PART_RIGHT_ANKLE);
}
