#ifndef __esp32roboteyes_h__
#define __esp32roboteyes_h__

#include <stdint.h>

#include <Adafruit_SSD1306.h>

class Esp32RobotEyes {
public:
  static const uint16_t EYE_WIDTH;
  static const uint16_t EYE_HEIGHT;
  static const uint16_t EYE_DISTANCE;
  static const uint16_t EYE_CORNER;
  static const uint16_t EYE_BORDER;

  static const unsigned long TIME_LAST_DEFAULT;
  static const unsigned long TIME_UPDATE;

  enum Mood {
    MOOD_NORMAL,
    MOOD_CONFUSED,
    MOOD_ANGRY
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

  Esp32RobotEyes(void);
  Esp32RobotEyes(const Esp32RobotEyes &copy);
  virtual ~Esp32RobotEyes(void);
  Esp32RobotEyes &operator=(const Esp32RobotEyes &other);

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetMood(const Mood &mood);
  virtual void SetPosition(const Position &position);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual void DrawBackground(void);
  virtual void DrawEyes(void);

  struct Data {
    Adafruit_SSD1306 *display;

    Mood mood;
    Position position;

    unsigned long last;

    Data(void) :
      display(NULL),
      mood(MOOD_NORMAL),
      position(POS_CENTER),
      last(TIME_LAST_DEFAULT) {
    }
  };

  Data data;
};

#endif
