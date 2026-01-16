#ifndef __esp32roboteyes_h__
#define __esp32roboteyes_h__

#include <Adafruit_SSD1306.h>

#include "Esp32RobotEye.h"

class Esp32RobotEyes {
public:
  static const uint16_t DISPLAY_WIDTH;
  static const uint16_t DISPLAY_HEIGHT;
  static const uint16_t DISPLAY_RESET;

  static const unsigned long TIME_LAST_DEFAULT;
  static const unsigned long TIME_UPDATE;

  Esp32RobotEyes(void);
  Esp32RobotEyes(const Esp32RobotEyes &copy);
  virtual ~Esp32RobotEyes(void);
  Esp32RobotEyes &operator=(const Esp32RobotEyes &other);

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetMood(const Esp32RobotEye::Mood &mood);
  virtual void SetPosition(const Esp32RobotEye::Position &position);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual void DrawBackground(void);
  virtual void DrawEyes(void);

  struct Data {
    Adafruit_SSD1306 *display;

    Esp32RobotEye::Mood mood;
    Esp32RobotEye::Animation animation;
    Esp32RobotEye::Position position;

    unsigned long last;

    Esp32RobotEye eyes[Esp32RobotEye::EYES];

    Data(void) :
      display(NULL),
      mood(MOOD_NORMAL),
      animation(ANIM_IDLE),
      position(POS_CENTER),
      last(TIME_LAST_DEFAULT) {
    }
  };

  Data data;
};

#endif
