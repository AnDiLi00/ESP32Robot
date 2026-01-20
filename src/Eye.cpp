#include "Eye.h"

const int16_t Eye::EYE_VALUE_DEFAULT = 0;

const uint16_t Eye::EYE_WIDTH = 26;
const uint16_t Eye::EYE_HEIGHT = 34;
const uint16_t Eye::EYE_HEIGHT_EXTENDED = 44;
const uint16_t Eye::EYE_DISTANCE = 18;
const uint16_t Eye::EYE_CORNER = 12;
const uint16_t Eye::EYE_BORDER = 10;

const int16_t Eye::FACTOR = 100;

Eye::Eye(void) :
  x(EYE_VALUE_DEFAULT),
  y(EYE_VALUE_DEFAULT),
  w(EYE_WIDTH),
  h(EYE_HEIGHT),
  increment_x(FACTOR),
  increment_y(FACTOR),
  increment_w(FACTOR),
  increment_h(FACTOR) {
}

Eye::Eye(const Eye &copy) :
  x(copy.x),
  y(copy.y),
  w(copy.w),
  h(copy.h),
  increment_x(copy.increment_x),
  increment_y(copy.increment_y),
  increment_w(copy.increment_w),
  increment_h(copy.increment_h) {
}

Eye::~Eye(void) {
}

Eye &Eye::operator=(const Eye &other) {
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

bool Eye::operator==(const Eye &comp) {
  bool ret = false;

  if ((x == comp.x) && (y == comp.y) && (w == comp.w) && (h == comp.h)) {
    ret = true;
  }

  return (ret);
}

bool Eye::operator!=(const Eye &comp) {
   return (!operator==(comp));
}



void Eye::Transition(Eye &eye_new) {
  if (eye_new.x < x) {
    x -= eye_new.increment_x;
    if (x < eye_new.x) {
      x = eye_new.x;
    }
  } else if (eye_new.x > x) {
    x += eye_new.increment_x;
    if (x > eye_new.x) {
      x = eye_new.x;
    }
  }

  if (eye_new.y < y) {
    y -= eye_new.increment_y;
    if (y < eye_new.y) {
      y = eye_new.y;
    }
  } else if (eye_new.y > y) {
    y += eye_new.increment_y;
    if (y > eye_new.y) {
      y = eye_new.y;
    }
  }

  if (eye_new.w < w) {
    w -= eye_new.increment_w;
    if (w < eye_new.w) {
      w = eye_new.w;
    }
  } else if (eye_new.w > w) {
    w += eye_new.increment_w;
    if (w > eye_new.w) {
      w = eye_new.w;
    }
  }

  if (eye_new.h < h) {
    h -= eye_new.increment_h;
    if (h < eye_new.h) {
      h = eye_new.h;
    }
  } else if (eye_new.h > h) {
    h += eye_new.increment_h;
    if (h > eye_new.h) {
      h = eye_new.h;
    }
  }
}

void Eye::Draw(Adafruit_SSD1306 *display, const Types::Mood &mood, const Types::EyeType &type) const {
  if (display == NULL) {
    return;
  }

  int16_t x1 = (x - w / 2) / FACTOR;
  int16_t y1 = (y - h / 2) / FACTOR;
  int16_t w1 = w / FACTOR;
  int16_t h1 = h / FACTOR;
  display->fillRoundRect(x1, y1, w1, h1, EYE_CORNER, SSD1306_WHITE);

  switch (mood) {
    case Types::MOOD_NORMAL:
    case Types::MOOD_CONFUSED:
    case Types::MOOD_CLOSED:
    case Types::MOODS:
      break;
    case Types::MOOD_ANGRY:
      {
        int16_t x1 = (x - w / 2) / FACTOR;
        int16_t y1 = (y - h / 2) / FACTOR;
        int16_t x2 = (x + w / 2) / FACTOR;
        int16_t y2 = y1 + EYE_HEIGHT / 3;

        if (type == Types::EYE_LEFT)  {
          display->fillTriangle(x1, y1, x2, y1, x2, y2, SSD1306_BLACK);
        } else {
          display->fillTriangle(x1, y1, x2, y1, x1, y2, SSD1306_BLACK);
        }
      }
      break;
    case Types::MOOD_TIRED:
      {
        int16_t h2 = y / (2 * FACTOR);

        display->fillRect(x1, y1, w1, h2, SSD1306_BLACK);
      }
      break;
  }
}

void Eye::GetEyes(const uint16_t &width, const uint16_t &height, const Types::Mood &mood, const Types::EyePosition &position, Eye (&eyes)[Types::EYES]) {
  switch (mood) {
    case Types::MOOD_NORMAL:
    case Types::MOOD_ANGRY:
    case Types::MOOD_TIRED:
      eyes[Types::EYE_LEFT].w = EYE_WIDTH;
      eyes[Types::EYE_LEFT].h = EYE_HEIGHT;
      eyes[Types::EYE_RIGHT].w = eyes[Types::EYE_LEFT].w;
      eyes[Types::EYE_RIGHT].h = eyes[Types::EYE_LEFT].h;
      break;
    case Types::MOOD_CONFUSED:
      eyes[Types::EYE_LEFT].w = EYE_WIDTH;
      eyes[Types::EYE_LEFT].h = EYE_HEIGHT_EXTENDED;
      eyes[Types::EYE_RIGHT].w = eyes[Types::EYE_LEFT].w;
      eyes[Types::EYE_RIGHT].h = EYE_HEIGHT;
      break;
    case Types::MOOD_CLOSED:
      eyes[Types::EYE_LEFT].w = EYE_WIDTH;
      eyes[Types::EYE_LEFT].h = 1;
      eyes[Types::EYE_RIGHT].w = eyes[Types::EYE_LEFT].w;
      eyes[Types::EYE_RIGHT].h = eyes[Types::EYE_LEFT].h;
      break;
    case Types::MOODS:
      break;
  }

  uint16_t eyes_max_width = 2 * EYE_WIDTH + EYE_DISTANCE;
  uint16_t eyes_max_height = EYE_HEIGHT_EXTENDED;
  uint16_t position_x = width / 2;
  uint16_t position_y = height / 2;

  switch (position) {
    case Types::POS_CENTER:
    case Types::POSITIONS:
      break;
    case Types::POS_CENTER_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      break;
    case Types::POS_CENTER_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      break;
    case Types::POS_TOP:
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case Types::POS_TOP_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case Types::POS_TOP_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      position_y = EYE_BORDER + eyes_max_height / 2;
      break;
    case Types::POS_BOTTOM:
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
    case Types::POS_BOTTOM_LEFT:
      position_x = EYE_BORDER + eyes_max_width / 2;
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
    case Types::POS_BOTTOM_RIGHT:
      position_x = width - EYE_BORDER - eyes_max_width / 2;
      position_y = height - EYE_BORDER - eyes_max_height / 2;
      break;
  }

  eyes[Types::EYE_LEFT].x = position_x - (EYE_DISTANCE + eyes[Types::EYE_LEFT].w) / 2;
  eyes[Types::EYE_LEFT].y = position_y;
  eyes[Types::EYE_RIGHT].x = position_x + (EYE_DISTANCE + eyes[Types::EYE_RIGHT].w) / 2;
  eyes[Types::EYE_RIGHT].y = position_y;

  eyes[Types::EYE_LEFT].x *= FACTOR;
  eyes[Types::EYE_LEFT].y *= FACTOR;
  eyes[Types::EYE_LEFT].w *= FACTOR;
  eyes[Types::EYE_LEFT].h *= FACTOR;
  eyes[Types::EYE_RIGHT].x *= FACTOR;
  eyes[Types::EYE_RIGHT].y *= FACTOR;
  eyes[Types::EYE_RIGHT].w *= FACTOR;
  eyes[Types::EYE_RIGHT].h *= FACTOR;
}
