#ifndef __eye_h__
#define __eye_h__

#include <Adafruit_SSD1306.h>

#include "Types.h"

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

  Eye(void);
  Eye(const Eye &copy);
  virtual ~Eye(void);
  Eye &operator=(const Eye &other);
  bool operator==(const Eye &comp);
  bool operator!=(const Eye &comp);

  virtual void Transition(Eye &eye_new);
  virtual void Draw(Adafruit_SSD1306 *display, const Types::Mood &mood, const Types::MoodSub &submood, const Types::EyeType &type) const;

  static void GetEyes(const uint16_t &width, const uint16_t &height, const Types::Mood &mood, const Types::MoodSub &submood, const Types::EyePosition &position, Eye (&eyes)[Types::EYES]);
  static void UpdateIncrement(const int16_t &steps, Eye (&eyes_from)[Types::EYES], Eye (&eyes_to)[Types::EYES]);

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
