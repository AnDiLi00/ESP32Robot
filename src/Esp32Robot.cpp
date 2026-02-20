#include "Esp32Robot.h"

Esp32Robot::Esp32Robot(void) :
  Animation(),
  Movement(),
  data() {
}

Esp32Robot::Esp32Robot(const Esp32Robot &copy) :
  Animation(copy),
  Movement(copy),
  data() {
  data.last_update = copy.data.last_update;
}

Esp32Robot::~Esp32Robot(void) {
}

Esp32Robot &Esp32Robot::operator=(const Esp32Robot &other) {
  if (&other != this) {
    Animation::operator=(other);
    Movement::operator=(other);

    data.last_update = other.data.last_update;
  }

  return (*this);
}

void Esp32Robot::OnSetup(void) {
  Animation::OnSetup();
  Movement::OnSetup();
}

void Esp32Robot::OnLoop(void) {
  unsigned long now = millis();
  unsigned long difference_update = now - data.last_update;

  if (difference_update >= Types::TIME_UPDATE) {
    data.last_update = now;

    Animation::OnLoop(now);
    Movement::OnLoop(now);
  }
}

void Esp32Robot::OnEnd(void) {
  Animation::OnEnd();
  Movement::OnEnd();
}
