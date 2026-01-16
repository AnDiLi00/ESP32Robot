#include "Esp32RobotEyes.h"

const uint16_t Esp32RobotEyes::DISPLAY_WIDTH = 128;
const uint16_t Esp32RobotEyes::DISPLAY_HEIGHT = 64;
const uint16_t Esp32RobotEyes::DISPLAY_RESET = 4;

const uint16_t Esp32RobotEyes::EYE_WIDTH = 30;
const uint16_t Esp32RobotEyes::EYE_HEIGHT = 30;
const uint16_t Esp32RobotEyes::EYE_DISTANCE = 16;
const uint16_t Esp32RobotEyes::EYE_CORNER = 8;
const uint16_t Esp32RobotEyes::EYE_BORDER = 10;

const unsigned long Esp32RobotEyes::TIME_LAST_DEFAULT = 0;
const unsigned long Esp32RobotEyes::TIME_UPDATE = 20;

Esp32RobotEyes::Esp32RobotEyes(void) :
  data() {
}

Esp32RobotEyes::Esp32RobotEyes(const Esp32RobotEyes &copy) :
  data() {

  data.display = copy.data.display;

  data.mood = copy.data.mood;
  data.animation = copy.data.animation;
  data.position = copy.data.position;

  data.last = copy.data.last;

  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    data.eyes[i] = copy.data.eyes[i];
    data.eyes_new[i] = copy.data.eyes_new[i];
  }
}

Esp32RobotEyes::~Esp32RobotEyes(void) {
}

Esp32RobotEyes &Esp32RobotEyes::operator=(const Esp32RobotEyes &other) {
  if (&other != this) {
    data.display = other.data.display;

    data.mood = other.data.mood;
    data.animation = other.data.animation;
    data.position = other.data.position;

    data.last = other.data.last;

    for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
      data.eyes[i] = other.data.eyes[i];
      data.eyes_new[i] = other.data.eyes_new[i];
    }
  }

  return (*this);
}

void Esp32RobotEyes::SetDisplay(Adafruit_SSD1306 *display) {
  data.display = display;
}

void Esp32RobotEyes::SetMood(const Esp32RobotEye::Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;
  }
}

void Esp32RobotEyes::SetPosition(const Esp32RobotEye::Position &position) {
  if (position != data.position) {
    data.position = position;
  }
}

void Esp32RobotEyes::OnSetup(void) {
  if (data.display == NULL) {
    return;
  }

  data.display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  data.display->clearDisplay();
  data.display->display();

  Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), data.mood, data.position, data.eyes);
  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    data.eyes_new[i] = data.eyes[i];
  }
}

void Esp32RobotEyes::OnLoop(void) {
  if (data.display == NULL) {
    return;
  }

  unsigned long now = millis();
  unsigned long difference = now - data.last;

  if (difference >= TIME_UPDATE) {
    data.last = now;

    data.display->clearDisplay();

    DrawBackground();
    DrawEyes();

    data.display->display();
  }
}

void Esp32RobotEyes::OnEnd(void) {
}

void Esp32RobotEyes::DrawBackground(void) {
  data.display->fillScreen(SSD1306_BLACK);
}

void Esp32RobotEyes::DrawEyes(void) {
  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    Esp32RobotEye::UpdateEye(data.eyes[i], data.eyes_new[i]);

    data.eyes[i].Draw(data.display, i, data.mood);
  }
}
