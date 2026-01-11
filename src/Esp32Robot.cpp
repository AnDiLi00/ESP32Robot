#include <vector>

#include <Arduino.h>
#include <BLEDevice.h>
#include <Servo.h>

#include "Esp32Robot.h"

const int8_t Esp32Robot::PIN_NOPIN = -1;
const int8_t Esp32Robot::OFFSET_DEFAULT = 0;

const int8_t Esp32Robot::DRIVE_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::DRIVE_ANKLE_DEFAULT = 45;
const int8_t Esp32Robot::DRIVE_ARM_DEFAULT = 45;

const int8_t Esp32Robot::WALK_DRIVE_DEFAULT = 90;
const int8_t Esp32Robot::WALK_ANKLE_DEFAULT = 125;
const int8_t Esp32Robot::WALK_ARM_DEFAULT = 45;

Esp32Robot::Esp32Robot(void) : data()
{
}

// Esp32Robot::Esp32Robot(const Esp32Robot &copy) {
// }

Esp32Robot::~Esp32Robot(void)
{
}

// Esp32Robot::Esp32Robot &operator=(const Esp32Robot &other) {
// }

int8_t Esp32Robot::GetPin(const BodyParts &part) const
{
  return (data.pins[part]);
}

int8_t Esp32Robot::GetOffset(const BodyParts &part) const
{
  return (data.offsets[part]);
}

void Esp32Robot::SetPin(const BodyParts &part, const int8_t &pin)
{
  if (pin != data.pins[part])
  {
    data.pins[part] = pin;
    data.servos[part].detach();
    if (data.pins[part] != PIN_NOPIN)
    {
      data.servos[part].attach(data.pins[part]);
    }
  }
}

void Esp32Robot::SetOffset(const BodyParts &part, const int8_t &offset)
{
  data.offsets[part] = offset;
}

void Esp32Robot::Move(const BodyParts &part, const int8_t &position)
{
  data.servos[part].write(position);
}

MovementMode Esp32Robot::GetMode(void) const
{
  return (data.mode);
}

void Esp32Robot::SetMode(const MovementMode &mode)
{
  if (mode != data.mode)
  {
    data.mode = mode;

    switch (data.mode)
    {
    case MOVE_DRIVE:
      break;
    case MOVE_WALK:
      break;
    }
  }
}

void Esp32Robot::OnSetup(void) const
{
  RemoteXY_Init(); // initialization by macros

  RemoteXYEngine.addVariableEvent(RemoteXY.main, RemoteXY_main_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.configure, RemoteXY_configure_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.mode, RemoteXY_mode_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.speed, RemoteXY_speed_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.steering, RemoteXY_steering_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.left_offset_slider, RemoteXY_left_offset_slider_event);
  RemoteXYEngine.addVariableEvent(RemoteXY.right_offset_slider, RemoteXY_right_offset_slider_event);
}

void Esp32Robot::OnLoop(void) const
{
}

void Esp32Robot::OnEnd(void) const
{
  for (size_t i = 0; i < data.servos.size(); i++)
  {
    data.servos[i].detach();
  }
}
