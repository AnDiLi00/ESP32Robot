#include "Esp32RobotEyes.h"

const int8_t Esp32RobotEyes::WIDTH_DEFAULT = 128;
const int8_t Esp32RobotEyes::HEIGHT_DEFAULT = 64;

const uint8_t Esp32RobotEyes::EYE_WIDTH = 30;
const uint8_t Esp32RobotEyes::EYE_HEIGHT = 30;
const uint8_t Esp32RobotEyes::EYE_DISTANCE = 14;
const uint8_t Esp32RobotEyes::EYE_CORNER = 8;
const uint8_t Esp32RobotEyes::EYE_BORDER = 10;

const uint16_t Esp32RobotEyes::COLOR_DEFAULT = 0x371c;
const uint16_t Esp32RobotEyes::COLOR_BACKGROUND_DEFAULT = 0x632c;

const unsigned long Esp32RobotEyes::UPDATE_LAST_DEFAULT = 0;
const unsigned long Esp32RobotEyes::UPDATE_MS_DEFAULT = 20;

Esp32RobotEyes::Esp32RobotEyes(void) :
  data() {
}

Esp32RobotEyes::Esp32RobotEyes(const Esp32RobotEyes &copy) :
  data() {

  data.display = copy.data.display;
  data.width = copy.data.width;
  data.height = copy.data.height;

  data.color = copy.data.color;
  data.color_background = copy.data.color_background;

  data.update_last = copy.data.update_last;
  data.update_ms = copy.data.update_ms;

  data.mood = copy.data.mood;
  data.position = copy.data.position;
}

Esp32RobotEyes::~Esp32RobotEyes(void) {
}

Esp32RobotEyes &Esp32RobotEyes::operator=(const Esp32RobotEyes &other) {
  if (&other != this) {
    data.display = other.data.display;
    data.width = other.data.width;
    data.height = other.data.height;

    data.color = other.data.color;
    data.color_background = other.data.color_background;

    data.update_last = other.data.update_last;
    data.update_ms = other.data.update_ms;

    data.mood = other.data.mood;
    data.position = other.data.position;
  }

  return (*this);
}

void Esp32RobotEyes::SetDisplay(Adafruit_SSD1306 *display) {
  data.display = display;
}

void Esp32RobotEyes::SetDisplayDimensions(uint8_t width, uint8_t height) {
  data.width = width;
  data.height = height;
}

void Esp32RobotEyes::SetEyeColor(uint8_t red, uint8_t green, uint8_t blue) {
  data.color = GetColor(red, green, blue);
}

void Esp32RobotEyes::SetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  data.color_background = GetColor(red, green, blue);
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
  data.display->begin(SSD1306_SWITCHCAPVCC, 0x3D);
  data.display->clearDisplay();
}

void Esp32RobotEyes::OnLoop(void) {
  data.display->clearDisplay();

  DrawBackground();
  DrawEyes();
}

void Esp32RobotEyes::OnEnd(void) {
}

uint16_t Esp32RobotEyes::GetColor(uint8_t red, uint8_t green, uint8_t blue) const {
  return (((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3));
}

void Esp32RobotEyes::DrawBackground(void) {
  data.display->fillScreen(data.color_background);
}

void Esp32RobotEyes::DrawEyes(void) {
  uint8_t position_x = WIDTH_DEFAULT / 2;
  uint8_t position_y = HEIGHT_DEFAULT / 2;

  int8_t eyes_width = 2 * EYE_WIDTH + EYE_DISTANCE;
  int8_t eyes_height = EYE_HEIGHT;

  switch (data.position) {
    case POS_CENTER:
      break;
    case POS_CENTER_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      break;
    case POS_CENTER_RIGHT:
      position_x = WIDTH_DEFAULT - EYE_BORDER - eyes_width / 2;
      break;
    case POS_TOP:
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_TOP_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_TOP_RIGHT:
      position_x = WIDTH_DEFAULT - EYE_BORDER - eyes_width / 2;
      position_y = EYE_BORDER + eyes_height / 2;
      break;
    case POS_BOTTOM:
      position_y = HEIGHT_DEFAULT - EYE_BORDER - eyes_height / 2;
      break;
    case POS_BOTTOM_LEFT:
      position_x = EYE_BORDER + eyes_width / 2;
      position_y = HEIGHT_DEFAULT - EYE_BORDER - eyes_height / 2;
      break;
    case POS_BOTTOM_RIGHT:
      position_x = WIDTH_DEFAULT - EYE_BORDER - eyes_width / 2;
      position_y = HEIGHT_DEFAULT - EYE_BORDER - eyes_height / 2;
      break;
  }

  uint8_t eye_left_x = position_x - eyes_width / 2 - EYE_DISTANCE / 2;
  uint8_t eye_left_y = position_y - eyes_height / 2;
  uint8_t eye_right_x = position_x + EYE_DISTANCE / 2;
  uint8_t eye_right_y = position_y - eyes_height / 2;

  display->fillRoundRect(eye_left_x, eye_left_y, EYE_WIDTH, EYE_HEIGHT, EYE_CORNER, data.color);
  display->fillRoundRect(eye_right_x, eye_right_y, EYE_WIDTH, EYE_HEIGHT, EYE_CORNER, data.color);
}
