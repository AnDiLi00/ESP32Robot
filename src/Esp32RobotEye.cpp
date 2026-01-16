#include "Esp32RobotEye.h"

const uint16_t Esp32RobotEye::EYE_WIDTH = 30;
const uint16_t Esp32RobotEye::EYE_HEIGHT = 30;
const uint16_t Esp32RobotEye::EYE_HEIGHT_EXTENDED = 50;
const uint16_t Esp32RobotEye::EYE_DISTANCE = 16;
const uint16_t Esp32RobotEye::EYE_CORNER = 10;
const uint16_t Esp32RobotEye::EYE_BORDER = 10;

Esp32RobotEye::Esp32RobotEye(void) :
  x(0),
  y(0),
  w(EYE_WIDTH),
  h(EYE_HEIGHT) {
}

Esp32RobotEye::Esp32RobotEye(const Esp32RobotEye &copy) :
  x(copy.x),
  y(copy.y),
  w(copy.w),
  h(copy.h) {
}

Esp32RobotEye::~Esp32RobotEye(void) {
}

Esp32RobotEye &Esp32RobotEye::operator=(const Esp32RobotEye &other) {
  if (&other != this) {
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;
  }

  return (*this);
}

void Esp32RobotEye::Draw(Adafruit_SSD1306 *display, const Eye &eye, const Mood &mood) const {
  if (display == NULL) {
    return;
  }

  display->fillRoundRect(x, y, w, h, EYE_CORNER, SSD1306_WHITE);

  switch (mood) {
    case MOOD_NORMAL:
    case MOOD_CONFUSED:
    case MOOD_CLOSED:
      break;
    case MOOD_ANGRY:
      if (eye == EYE_LEFT)  {
        display->fillTriangle(x, y, x + w, y, x + w, y + EYE_HEIGHT / 3, SSD1306_BLACK);
      } else {
        display->fillTriangle(x, y, x + w, y, x, y + EYE_HEIGHT / 3, SSD1306_BLACK);
      }
      break;
    case MOOD_TIRED:
      display->fillRect(x, y, x + w, y + h / 2, SSD1306_BLACK);
      break;
  }
}

void Esp32RobotEye::GetEyes(const uint16_t &width, const uint16_t &height, const Mood &mood, const Position &position, Esp32RobotEye (&eyes)[EYES]) {
  switch (mood) {
    case MOOD_NORMAL:
    case MOOD_ANGRY:
    case MOOD_TIRED:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = EYE_HEIGHT;
      eyes[EYE_RIGHT].w = EYE_WIDTH;
      eyes[EYE_RIGHT].h = EYE_HEIGHT;
      break;
    case MOOD_CONFUSED:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = EYE_HEIGHT_EXTENDED;
      eyes[EYE_RIGHT].w = EYE_WIDTH;
      eyes[EYE_RIGHT].h = EYE_HEIGHT;
      break;
    case MOOD_CLOSED:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = 1;
      eyes[EYE_RIGHT].w = EYE_WIDTH;
      eyes[EYE_RIGHT].h = 1;
      break;
  }

  uint16_t eyes_max_width = 2 * EYE_WIDTH + EYE_DISTANCE;
  uint16_t eyes_max_height = EYE_HEIGHT_EXTENDED;
  uint16_t position_x = width / 2;
  uint16_t position_y = height / 2;

  switch (position) {
    case POS_CENTER:
      break;
    case POS_CENTER_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      break;
    case POS_CENTER_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      break;
    case POS_TOP:
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case POS_TOP_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case POS_TOP_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case POS_BOTTOM:
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
    case POS_BOTTOM_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
    case POS_BOTTOM_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
  }

  eyes[EYE_LEFT].x = position_x - EYE_WIDTH / 2 - EYE_DISTANCE / 2 - (EYE_WIDTH - eyes[EYE_LEFT].w) / 2;
  eyes[EYE_LEFT].y = position_y - eyes[EYE_LEFT].h / 2;
  eyes[EYE_RIGHT].x = position_x + EYE_DISTANCE / 2 + (EYE_WIDTH - eyes[EYE_RIGHT].w) / 2;
  eyes[EYE_RIGHT].y = position_y - eyes[EYE_RIGHT].h / 2;
}

void Esp32RobotEye::UpdateEye(Esp32RobotEye &eye, Esp32RobotEye &eye_new) {
  if (data.eyes_new[i].x < data.eyes[i].x) {
    data.eyes[i].x--;
  } else if (data.eyes_new[i].x > data.eyes[i].x) {
    data.eyes[i].x++;
  }

  if (data.eyes_new[i].y < data.eyes[i].y) {
    data.eyes[i].y--;
  } else if (data.eyes_new[i].y > data.eyes[i].y) {
    data.eyes[i].y++;
  }

  if (data.eyes_new[i].w < data.eyes[i].w) {
    data.eyes[i].w--;
  } else if (data.eyes_new[i].w > data.eyes[i].w) {
    data.eyes[i].w++;
  }

  if (data.eyes_new[i].h < data.eyes[i].h) {
    data.eyes[i].h--;
  } else if (data.eyes_new[i].h > data.eyes[i].h) {
    data.eyes[i].h++;
  }
}
