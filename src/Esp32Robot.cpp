#include "Esp32Robot.h"

Esp32Robot::Esp32Robot(void) :
  Animation(),
  Movement() {
}

Esp32Robot::Esp32Robot(const Esp32Robot &copy) :
  Animation(copy),
  Movement(copy) {
}

Esp32Robot::~Esp32Robot(void) {
}

Esp32Robot &Esp32Robot::operator=(const Esp32Robot &other) {
  if (&other != this) {
    Animation::operator=(other);
    Movement::operator=(other);
  }

  return (*this);
}

void Esp32Robot::OnSetup(void) {
  Animation::OnSetup();
  Movement::OnSetup();
}

void Esp32Robot::OnLoop(void) {
  Animation::OnLoop();
  Movement::OnLoop();
}

void Esp32Robot::OnEnd(void) {
  Animation::OnEnd();
  Movement::OnEnd();
}
