#ifndef __esp32roboteyes_h__
#define __esp32roboteyes_h__

#include <Adafruit_SSD1306.h>

#include "Esp32RobotEye.h"

class Esp32RobotEyes {
public:
  static const uint16_t DISPLAY_WIDTH;
  static const uint16_t DISPLAY_HEIGHT;
  static const uint16_t DISPLAY_RESET;

  static const unsigned long TIME_DEFAULT;
  static const unsigned long TIME_UPDATE;
  static const unsigned long TIME_IDLE;

  enum Animation {
    ANIM_IDLE,
    ANIM_BLINK,
    ANIM_SHAKE
  };

  enum SubAnimation {
    SUB_NONE,
    SUB_CLOSING,
    SUB_OPENING,
  };

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

  virtual unsigned long GetRandomIdleDuration(void);

  virtual bool CheckEyes(void);

  struct Data {
    Adafruit_SSD1306 *display;

    Esp32RobotEye::Mood mood;
    Esp32RobotEye::Position position;

    Animation animation;
    SubAnimation animation_sub;

    unsigned long last_update;
    unsigned long last_idle;
    unsigned long duration;

    int8_t anim;

    Esp32RobotEye eyes[Esp32RobotEye::EYES];
    Esp32RobotEye eyes_new[Esp32RobotEye::EYES];

    Data(void) :
      display(NULL),
      mood(Esp32RobotEye::MOOD_NORMAL),
      position(Esp32RobotEye::POS_CENTER),
      animation(ANIM_IDLE),
      animation_sub(SUB_NONE),
      last_update(TIME_DEFAULT),
      last_idle(TIME_DEFAULT),
      duration(TIME_DEFAULT),
      anim(0) {
    }
  };

  Data data;
};

#endif
