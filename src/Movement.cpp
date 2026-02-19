#include "Movement.h"

const int8_t Movement::PIN_NOPIN = -1;
const int8_t Movement::OFFSET_DEFAULT = 0;
const uint8_t Movement::POSITION_DEFAULT = 0;

const uint8_t Movement::SERVO_CENTER = 90;
const uint8_t Movement::SERVO_RANGE_SPEED = 50;
const uint8_t Movement::SERVO_RANGE_STEERING = 30;

const uint8_t Movement::DRIVE_DRIVE_DEFAULT = Movement::SERVO_CENTER;
const uint8_t Movement::DRIVE_ANKLE_LEFT_DEFAULT = 45;
const uint8_t Movement::DRIVE_ANKLE_RIGHT_DEFAULT = 125;
const uint8_t Movement::DRIVE_ARM_DEFAULT = Movement::SERVO_CENTER;

const uint8_t Movement::WALK_DRIVE_DEFAULT = Movement::SERVO_CENTER;
const uint8_t Movement::WALK_ANKLE_LEFT_DEFAULT = 125;
const uint8_t Movement::WALK_ANKLE_RIGHT_DEFAULT = 45;
const uint8_t Movement::WALK_ANKLE_LEFT_WALK = 135;
const uint8_t Movement::WALK_ANKLE_RIGHT_WALK = 35;
const uint8_t Movement::WALK_ARM_DEFAULT = Movement::SERVO_CENTER;

const unsigned long Movement::TIME_MOVE = 360;

Movement::Movement(void) :
  data() {
}

Movement::Movement(const Movement &copy) :
  data() {

  for (uint8_t i = 0; i < Types::PART_PARTS; i++) {
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
  data.mode_sub = copy.data.mode_sub;

  data.last_move = copy.data.last_move;

  data.steering = copy.data.steering;
  data.speed = copy.data.speed;
}

Movement::~Movement(void) {
}

Movement &Movement::operator=(const Movement &other) {
  if (&other != this) {
    for (uint8_t i = 0; i < Types::PART_PARTS; i++) {
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
    data.mode_sub = other.data.mode_sub;

    data.last_move = other.data.last_move;

    data.steering = other.data.steering;
    data.speed = other.data.speed;
  }

  return (*this);
}

void Movement::SetPin(const Types::BodyParts &part, const int8_t &pin) {
  if (pin != data.pins[part]) {
    data.pins[part] = pin;

    data.servos[part].detach();
    if (data.pins[part] != PIN_NOPIN) {
      data.servos[part].attach(data.pins[part]);
    }
  }
}

void Movement::SetOffset(const Types::BodyParts &part, const Types::MovementMode &mode, const int8_t &offset) {
  switch (mode) {
    case Types::MOVE_DRIVE:
      data.offsets_drive[part] = offset;
      break;
    case Types::MOVE_WALK:
      data.offsets_walk[part] = offset;
      break;
  }
}

void Movement::SetMode(const Types::MovementMode &mode) {
  if (mode != data.mode) {
    data.mode = mode;

    switch (data.mode) {
      case Types::MOVE_DRIVE:
        Move(Types::PART_LEFT_DRIVE, DRIVE_DRIVE_DEFAULT);
        Move(Types::PART_RIGHT_DRIVE, DRIVE_DRIVE_DEFAULT);
        Move(Types::PART_LEFT_ANKLE, DRIVE_ANKLE_LEFT_DEFAULT);
        Move(Types::PART_RIGHT_ANKLE, DRIVE_ANKLE_RIGHT_DEFAULT);
        break;
      case Types::MOVE_WALK:
        Move(Types::PART_LEFT_DRIVE, WALK_DRIVE_DEFAULT);
        Move(Types::PART_RIGHT_DRIVE, WALK_DRIVE_DEFAULT);
        Move(Types::PART_LEFT_ANKLE, WALK_ANKLE_LEFT_DEFAULT);
        Move(Types::PART_RIGHT_ANKLE, WALK_ANKLE_RIGHT_DEFAULT);
        break;
    }
  }
}

void Movement::OnSetup(void) {
}

void Movement::OnLoop(const unsigned long &now) {
  switch (data.mode) {
    case Types::MOVE_DRIVE:
      UpdateDriving(now);
      break;
    case Types::MOVE_WALK:
      UpdateWalking(now);
      break;
  }
}

void Movement::OnEnd(void) {
  for (int8_t i = 0; i < Types::PART_PARTS; i++) {
    data.servos[i].detach();
  }
}

void Movement::OnModeChange(const uint8_t &mode) {
  if (mode == 1) {
    SetMode(Types::MOVE_WALK);
  } else {
    SetMode(Types::MOVE_DRIVE);
  }
}

void Movement::OnSpeedChange(const int8_t &speed) {
  if (speed != data.speed) {
    data.speed = speed;
  }
}

void Movement::OnSteeringChange(const int8_t &steering) {
  if (steering != data.steering) {
    data.steering = steering;
  }
}

void Movement::OnOffsetLeftChange(const int8_t &left_offset) {
  SetOffset(Types::PART_LEFT_ANKLE, data.mode, left_offset);
  Update(Types::PART_LEFT_ANKLE);
}

void Movement::OnOffsetRightChange(const int8_t &right_offset) {
  SetOffset(Types::PART_RIGHT_ANKLE, data.mode, right_offset);
  Update(Types::PART_RIGHT_ANKLE);
}

void Movement::UpdateDriving(const unsigned long &now) {
  int16_t speed_part = (SERVO_RANGE_SPEED * data.speed) / 100;
  int16_t steering_part = (SERVO_RANGE_STEERING * data.steering) / 100;

  int16_t position_right = SERVO_CENTER - (speed_part - steering_part);
  int16_t position_left = SERVO_CENTER + (speed_part + steering_part);

  if (position_right < 0) {
    position_right = 0;
  } else if (position_right > (2 * SERVO_CENTER)) {
    position_right = 2 * SERVO_CENTER;
  }

  if (position_left < 0) {
    position_left = 0;
  } else if (position_left > (2 * SERVO_CENTER)) {
    position_left = 2 * SERVO_CENTER;
  }

  Move(Types::PART_RIGHT_DRIVE, (uint8_t)position_right);
  Move(Types::PART_LEFT_DRIVE, (uint8_t)position_left);
}

void Movement::UpdateWalking(const unsigned long &now) {
  unsigned long difference_update = now - data.last_move;

  switch (data.mode_sub) {
    case Types::MSUB_DRIVE:
      if (data.speed != 0) {
        data.mode_sub = Types::MSUB_LEFT_LEAN;
        data.last_move = now;
        Move(Types::PART_LEFT_ANKLE, WALK_ANKLE_LEFT_WALK);
      }
      break;
    case Types::MSUB_LEFT_LEAN:
      if (difference_update >= TIME_MOVE) {
      }
      break;
    case Types::MSUB_LEFT_ROTATE:
      break;
    case Types::MSUB_LEFT_LEANBACK:
      if (data.speed != 0) {
        data.mode_sub = Types::MSUB_LEFT_LEAN;
        data.last_move = now;
        Move(Types::PART_LEFT_ANKLE, WALK_ANKLE_LEFT_WALK);
      }
      break;
    case Types::MSUB_RIGHT_LEAN:
      break;
    case Types::MSUB_RIGHT_ROTATE:
      break;
    case Types::MSUB_RIGHT_LEANBACK:
      break;
  }
}

void Movement::Update(const Types::BodyParts &part) {
  Move(part, data.positions[part]);
}

void Movement::Move(const Types::BodyParts &part, const uint8_t &position) {
  data.positions[part] = position;
  data.servos[part].write(position + GetOffset(part, data.mode));
}

int8_t Movement::GetOffset(const Types::BodyParts &part, const Types::MovementMode &mode) const {
  int8_t ret = OFFSET_DEFAULT;

  switch (mode) {
    case Types::MOVE_DRIVE:
      ret = data.offsets_drive[part];
      break;
    case Types::MOVE_WALK:
      ret = data.offsets_walk[part];
      break;
  }

  return (ret);
}

void Movement::PrintMode(void) const {
  switch (data.mode) {
    case Types::MOVE_DRIVE:
      Serial.println("mode=drive");
      break;
    case Types::MOVE_WALK:
      Serial.println("mode=walk");
      break;
  }
}
