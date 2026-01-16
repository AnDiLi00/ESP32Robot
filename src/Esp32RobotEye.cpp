#include "Esp32RobotEye.h"

const int16_t Esp32RobotEye::EYE_VALUE_DEFAULT = 0;

const uint16_t Esp32RobotEye::EYE_WIDTH = 30;
const uint16_t Esp32RobotEye::EYE_HEIGHT = 30;
const uint16_t Esp32RobotEye::EYE_HEIGHT_EXTENDED = 50;
const uint16_t Esp32RobotEye::EYE_DISTANCE = 16;
const uint16_t Esp32RobotEye::EYE_CORNER = 10;
const uint16_t Esp32RobotEye::EYE_BORDER = 10;

const int16_t Esp32RobotEye::FACTOR = 100;

Esp32RobotEye::Esp32RobotEye(void) :
  x(EYE_VALUE_DEFAULT),
  y(EYE_VALUE_DEFAULT),
  w(EYE_WIDTH),
  h(EYE_HEIGHT),
  increment_x(FACTOR),
  increment_y(FACTOR),
  increment_w(FACTOR),
  increment_h(FACTOR) {
}

Esp32RobotEye::Esp32RobotEye(const Esp32RobotEye &copy) :
  x(copy.x),
  y(copy.y),
  w(copy.w),
  h(copy.h),
  increment_x(copy.increment_x),
  increment_y(copy.increment_y),
  increment_w(copy.increment_w),
  increment_h(copy.increment_h) {
}

Esp32RobotEye::~Esp32RobotEye(void) {
}

Esp32RobotEye &Esp32RobotEye::operator=(const Esp32RobotEye &other) {
  if (&other != this) {
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;

    increment_x = other.increment_x;
    increment_y = other.increment_y;
    increment_w = other.increment_w;
    increment_h = other.increment_h;
  }

  return (*this);
}

bool Esp32RobotEye::operator==(const Esp32RobotEye &comp) {
  bool ret = false;

  if ((x == comp.x) && (y == comp.y) && (w == comp.w) && (h == comp.h)) {
    ret = true;
  }

  return (ret);
}

bool Esp32RobotEye::operator!=(const Esp32RobotEye &comp) {
   return (!operator==(comp));
}

void Esp32RobotEye::Draw(Adafruit_SSD1306 *display, const Eye &eye, const Mood &mood) const {
  if (display == NULL) {
    return;
  }

  int16_t x1 = (x - w / 2) / FACTOR;
  int16_t y1 = (y - h / 2) / FACTOR;
  int16_t w1 = w / FACTOR;
  int16_t h1 = h / FACTOR;
  display->fillRoundRect(x1, y1, w1, h1, EYE_CORNER, SSD1306_WHITE);

  switch (mood) {
    case MOOD_NORMAL:
    case MOOD_CONFUSED:
    case MOOD_CLOSED:
    case MOODS:
      break;
    case MOOD_ANGRY:
      {
        int16_t x1 = (x - w / 2) / FACTOR;
        int16_t y1 = (y - h / 2) / FACTOR;
        int16_t x2 = (x + w / 2) / FACTOR;
        int16_t y2 = y1 + EYE_HEIGHT / 3;

        if (eye == EYE_LEFT)  {
          display->fillTriangle(x1, y1, x2, y1, x2, y2, SSD1306_BLACK);
        } else {
          display->fillTriangle(x1, y1, x2, y1, x1, y2, SSD1306_BLACK);
        }
      }
      break;
    case MOOD_TIRED:
      {
        int16_t h2 = y / (2 * FACTOR);

        display->fillRect(x1, y1, w1, h2, SSD1306_BLACK);
      }
      break;
  }
}

void Esp32RobotEye::GetEyes(const uint16_t &width, const uint16_t &height, const Mood &mood, const Position &position, Esp32RobotEye (&eyes)[EYES]) {
  switch (mood) {
    case MOOD_NORMAL:
    case MOOD_ANGRY:
    case MOOD_TIRED:
    case MOODS:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = EYE_HEIGHT;
      eyes[EYE_RIGHT].w = eyes[EYE_LEFT].w;
      eyes[EYE_RIGHT].h = eyes[EYE_LEFT].h;
      break;
    case MOOD_CONFUSED:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = EYE_HEIGHT_EXTENDED;
      eyes[EYE_RIGHT].w = eyes[EYE_LEFT].w;
      eyes[EYE_RIGHT].h = EYE_HEIGHT;
      break;
    case MOOD_CLOSED:
      eyes[EYE_LEFT].w = EYE_WIDTH;
      eyes[EYE_LEFT].h = 1;
      eyes[EYE_RIGHT].w = eyes[EYE_LEFT].w;
      eyes[EYE_RIGHT].h = eyes[EYE_LEFT].h;
      break;
  }

  uint16_t eyes_max_width = 2 * EYE_WIDTH + EYE_DISTANCE;
  uint16_t eyes_max_height = EYE_HEIGHT_EXTENDED;
  uint16_t position_x = width / 2;
  uint16_t position_y = height / 2;

  switch (position) {
    case POS_CENTER:
    case POSITIONS:
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

  eyes[EYE_LEFT].x = position_x - (EYE_DISTANCE + eyes[EYE_LEFT].w) / 2;
  eyes[EYE_LEFT].y = position_y;
  eyes[EYE_RIGHT].x = position_x + (EYE_DISTANCE + eyes[EYE_RIGHT].w) / 2;
  eyes[EYE_RIGHT].y = position_y;

  eyes[EYE_LEFT].x *= FACTOR;
  eyes[EYE_LEFT].y *= FACTOR;
  eyes[EYE_LEFT].w *= FACTOR;
  eyes[EYE_LEFT].h *= FACTOR;
  eyes[EYE_RIGHT].x *= FACTOR;
  eyes[EYE_RIGHT].y *= FACTOR;
  eyes[EYE_RIGHT].w *= FACTOR;
  eyes[EYE_RIGHT].h *= FACTOR;
}

void Esp32RobotEye::UpdateEye(Esp32RobotEye &eye, Esp32RobotEye &eye_new) {
  if (eye_new.x < eye.x) {
    eye.x -= eye_new.increment_x;
    if (eye.x < eye_new.x) {
      eye.x = eye_new.x;
    }
  } else if (eye_new.x > eye.x) {
    eye.x += eye_new.increment_x;
    if (eye.x > eye_new.x) {
      eye.x = eye_new.x;
    }
  }

  if (eye_new.y < eye.y) {
    eye.y -= eye_new.increment_y;
    if (eye.y < eye_new.y) {
      eye.y = eye_new.y;
    }
  } else if (eye_new.y > eye.y) {
    eye.y += eye_new.increment_y;
    if (eye.y > eye_new.y) {
      eye.y = eye_new.y;
    }
  }

  if (eye_new.w < eye.w) {
    eye.w -= eye_new.increment_w;
    if (eye.w < eye_new.w) {
      eye.w = eye_new.w;
    }
  } else if (eye_new.w > eye.w) {
    eye.w += eye_new.increment_w;
    if (eye.w > eye_new.w) {
      eye.w = eye_new.w;
    }
  }

  if (eye_new.h < eye.h) {
    eye.h -= eye_new.increment_h;
    if (eye.h < eye_new.h) {
      eye.h = eye_new.h;
    }
  } else if (eye_new.h > eye.h) {
    eye.h += eye_new.increment_h;
    if (eye.h > eye_new.h) {
      eye.h = eye_new.h;
    }
  }
}
