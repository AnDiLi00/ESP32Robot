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
  data.position = copy.data.position;

  data.last = copy.data.last;
}

Esp32RobotEyes::~Esp32RobotEyes(void) {
}

Esp32RobotEyes &Esp32RobotEyes::operator=(const Esp32RobotEyes &other) {
  if (&other != this) {
    data.display = other.data.display;

    data.mood = other.data.mood;
    data.position = other.data.position;

    data.last = other.data.last;
  }

  return (*this);
}

void Esp32RobotEyes::SetDisplay(Adafruit_SSD1306 *display) {
  data.display = display;
}

void Esp32RobotEyes::SetMood(const Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;
  }
}

void Esp32RobotEyes::SetPosition(const Position &position) {
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
  uint16_t position_x = data.display->width() / 2;
  uint16_t position_y = data.display->height() / 2;

  uint16_t eyes_width = 2 * EYE_WIDTH + EYE_DISTANCE;
  uint16_t eyes_height = EYE_HEIGHT;

  switch (data.position) {
    case POS_CENTER:
      break;
    case POS_CENTER_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      break;
    case POS_CENTER_RIGHT:
      position_x = data.display->width() - EYE_BORDER - eyes_width / 2;
      break;
    case POS_TOP:
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_TOP_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_TOP_RIGHT:
      position_x = data.display->width() - EYE_BORDER - eyes_width / 2;
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_BOTTOM:
      position_y = data.display->height() - EYE_BORDER - eyes_height / 2;
      break;
    case POS_BOTTOM_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      position_y = data.display->height() - EYE_BORDER - eyes_height / 2;
      break;
    case POS_BOTTOM_RIGHT:
      position_x = data.display->width() - EYE_BORDER - eyes_width / 2;
      position_y = data.display->height() - EYE_BORDER - eyes_height / 2;
      break;
  }

  uint16_t eye_left_x = position_x - eyes_width / 2 - EYE_DISTANCE / 2;
  uint16_t eye_left_y = position_y - eyes_height / 2;
  uint16_t eye_right_x = position_x + EYE_DISTANCE / 2;
  uint16_t eye_right_y = position_y - eyes_height / 2;

  data.display->fillRoundRect(eye_left_x, eye_left_y, EYE_WIDTH, EYE_HEIGHT, EYE_CORNER, SSD1306_WHITE);
  data.display->fillRoundRect(eye_right_x, eye_right_y, EYE_WIDTH, EYE_HEIGHT, EYE_CORNER, SSD1306_WHITE);
}
