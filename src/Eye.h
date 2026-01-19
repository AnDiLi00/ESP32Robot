#ifndef __esp32roboteye_h__
#define __esp32roboteye_h__

#include <Adafruit_SSD1306.h>

#include "Animation.h"

class Eye {
public:
  static const int16_t EYE_VALUE_DEFAULT;

  static const uint16_t EYE_WIDTH;
  static const uint16_t EYE_HEIGHT;
  static const uint16_t EYE_HEIGHT_EXTENDED;
  static const uint16_t EYE_DISTANCE;
  static const uint16_t EYE_CORNER;
  static const uint16_t EYE_BORDER;

  static const int16_t FACTOR;

  enum Type {
    EYE_LEFT,
    EYE_RIGHT,
    EYES
  };

  enum Position {
    POS_CENTER,
    POS_CENTER_LEFT,
    POS_CENTER_RIGHT,
    POS_TOP,
    POS_TOP_LEFT,
    POS_TOP_RIGHT,
    POS_BOTTOM,
    POS_BOTTOM_LEFT,
    POS_BOTTOM_RIGHT,
    POSITIONS
  };

  Eye(void);
  Eye(const Eye &copy);
  virtual ~Eye(void);
  Eye &operator=(const Eye &other);
  bool operator==(const Eye &comp);
  bool operator!=(const Eye &comp);

  virtual void Transition(Eye &eye_new);
  virtual void Draw(Adafruit_SSD1306 *display, const Animation::Mood &mood, const Type &type) const;

  static void GetEyes(const uint16_t &width, const uint16_t &height, const Animation::Mood &mood, const Position &position, Eye (&eyes)[EYES]);

  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;

  int16_t increment_x;
  int16_t increment_y;
  int16_t increment_w;
  int16_t increment_h;
};

#endif
