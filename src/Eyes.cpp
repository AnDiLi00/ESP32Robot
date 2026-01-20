#include "Eyes.h"

const uint16_t Eyes::DISPLAY_WIDTH = 128;
const uint16_t Eyes::DISPLAY_HEIGHT = 64;
const uint16_t Eyes::DISPLAY_RESET = 4;

Eyes::Eyes(void) :
  data() {
}

Eyes::Eyes(const Eyes &copy) :
  data() {

  data.display = copy.data.display;
  data.position = copy.data.position;

  for (uint8_t i = 0; i < Types::EYES; i++) {
    data.eyes[i] = copy.data.eyes[i];
    data.eyes_new[i] = copy.data.eyes_new[i];
  }
}

Eyes::~Eyes(void) {
}

Eyes &Eyes::operator=(const Eyes &other) {
  if (&other != this) {
    data.display = other.data.display;
    data.position = other.data.position;

    for (uint8_t i = 0; i < Types::EYES; i++) {
      data.eyes[i] = other.data.eyes[i];
      data.eyes_new[i] = other.data.eyes_new[i];
    }
  }

  return (*this);
}

bool Eyes::AreTransitioned(void) {
  bool ret = true;

  for (uint8_t i = 0; i < Types::EYES; i++) {
    if (data.eyes[i] != data.eyes_new[i]) {
      ret = false;
      break;
    }
  }

  return (ret);
}

void Eyes::SetDisplay(Adafruit_SSD1306 *display) {
  data.display = display;
}

void Eyes::SetPosition(const Types::EyePosition &position) {
  if (position != data.position) {
    data.position = position;

    switch (data.position) {
      case Types::POS_CENTER:
        Serial.println("position=center");
        break;
      case Types::POS_CENTER_LEFT:
        Serial.println("position=center left");
        break;
      case Types::POS_CENTER_RIGHT:
        Serial.println("position=center right");
        break;
      case Types::POS_TOP:
        Serial.println("position=top center");
        break;
      case Types::POS_TOP_LEFT:
        Serial.println("position=top left");
        break;
      case Types::POS_TOP_RIGHT:
        Serial.println("position=top right");
        break;
      case Types::POS_BOTTOM:
        Serial.println("position=bottom center");
        break;
      case Types::POS_BOTTOM_LEFT:
        Serial.println("position=bottom left");
        break;
      case Types::POS_BOTTOM_RIGHT:
        Serial.println("position=bottom right");
        break;
      case Types::POSITIONS:
        Serial.println("position=positions?!");
        break;
    }
  }
}

void Eyes::OnSetup(const Types::Mood &mood) {
  if (data.display == NULL) {
    return;
  }

  data.display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  data.display->clearDisplay();
  data.display->display();

  Eye::GetEyes(data.display->width(), data.display->height(), mood, data.position, data.eyes);
  for (uint8_t i = 0; i < Types::EYES; i++) {
    data.eyes_new[i] = data.eyes[i];
  }
}

void Eyes::OnLoop(const Types::Mood &mood) {
  if (data.display == NULL) {
    return;
  }

  data.display->clearDisplay();

  DrawBackground();
  DrawEyes(mood);

  data.display->display();
}

void Eyes::OnEnd(void) {
}

void Eyes::OnMoodChange(const Types::Mood &mood) {
  if (data.display == NULL) {
    return;
  }

  Eye::GetEyes(data.display->width(), data.display->height(), mood, data.position, data.eyes_new);
}

void Eyes::DrawBackground(void) {
  data.display->fillScreen(SSD1306_BLACK);
}

void Eyes::DrawEyes(const Types::Mood &mood) {
  for (uint8_t i = 0; i < Types::EYES; i++) {
    data.eyes[i].Transition(data.eyes_new[i]);

    data.eyes[i].Draw(data.display, mood, (Types::EyeType)i);
  }
}
