#ifndef __animation_h__
#define __animation_h__

#include "Eyes.h"
#include "Types.h"

class Animation {
public:
  static const unsigned long TIME_DEFAULT;
  static const unsigned long TIME_UPDATE;
  static const unsigned long TIME_IDLE;
  static const unsigned long TIME_IDLE_VARIANCE;

  Animation(void);
  Animation(const Animation &copy);
  virtual ~Animation(void);
  Animation &operator=(const Animation &other);

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetMood(const Types::Mood &mood);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual void DoUpdate(const unsigned long &now);

  virtual unsigned long GetIdleDuration(void);

  struct Data {
    Types::Mood mood;

    Types::AnimType animation;
    Types::AnimSubType animation_sub;

    unsigned long last_update;
    unsigned long last_idle;
    unsigned long duration;

    Eyes eyes;

    int8_t anim;

    Data(void) :
      mood(Types::MOOD_NORMAL),
      animation(Types::ANIM_IDLE),
      animation_sub(Types::SUB_NONE),
      last_update(TIME_DEFAULT),
      last_idle(TIME_DEFAULT),
      duration(TIME_DEFAULT),
      eyes(Eyes()),
      anim(0) {
    }
  };

  Data data;
};

#endif
