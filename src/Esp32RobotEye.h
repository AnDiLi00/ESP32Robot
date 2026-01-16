#ifndef __esp32roboteye_h__
#define __esp32roboteye_h__

#include <Adafruit_SSD1306.h>

class Esp32RobotEye {
public:
  static const uint16_t EYE_WIDTH;
  static const uint16_t EYE_HEIGHT;
  static const uint16_t EYE_HEIGHT_EXTENDED;
  static const uint16_t EYE_DISTANCE;
  static const uint16_t EYE_CORNER;
  static const uint16_t EYE_BORDER;

  enum Eye {
    EYE_LEFT,
    EYE_RIGHT
    EYES
  };

  enum Mood {
    MOOD_NORMAL,
    MOOD_CONFUSED,
    MOOD_ANGRY,
    MOOD_TIRED,
    MOOD_CLOSED
  };

  enum Animation {
    ANIM_IDLE,
    ANIM_BLINK,
    ANIM_SHAKE
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
    POS_BOTTOM_RIGHT
  };

  Esp32RobotEye(void);
  Esp32RobotEye(const Esp32RobotEye &copy);
  virtual ~Esp32RobotEye(void);
  Esp32RobotEye &operator=(const Esp32RobotEye &other);

  virtual void Draw(Adafruit_SSD1306 *display, const Eye &eye, const Mood &mood) const;

  static void GetEyes(const uint16_t &width, const uint16_t &height, const Mood &mood, const Position &position, Esp32RobotEye (&eyes)[EYES]);
  static void UpdateEye(Esp32RobotEye &eye, Esp32RobotEye &eye_new);

  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
};

#endif
