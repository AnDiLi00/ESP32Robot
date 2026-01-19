#ifndef __animation_h__
#define __animation_h__

#include "Eyes.h"

class Animation {
public:
  static const unsigned long TIME_DEFAULT;
  static const unsigned long TIME_UPDATE;
  static const unsigned long TIME_IDLE;
  static const unsigned long TIME_IDLE_VARIANCE;

  enum Mood {
    MOOD_NORMAL,
    MOOD_CONFUSED,
    MOOD_ANGRY,
    MOOD_TIRED,
    MOOD_CLOSED,
    MOODS
  };

  enum Type {
    ANIM_IDLE,
    ANIM_BLINK,
    ANIM_SHAKE
  };

  enum SubType {
    SUB_NONE,
    SUB_CLOSING,
    SUB_OPENING,
  };

  Animation(void);
  Animation(const Animation &copy);
  virtual ~Animation(void);
  Animation &operator=(const Animation &other);

  virtual void SetMood(const Mood &mood);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual void DoUpdate(const unsigned long &now);

  virtual unsigned long GetIdleDuration(void);

  struct Data {
    Mood mood;

    Type animation;
    SubType animation_sub;

    unsigned long last_update;
    unsigned long last_idle;
    unsigned long duration;

    Eyes eyes;

    int8_t anim;

    Data(void) :
      mood(MOOD_NORMAL),
      animation(ANIM_IDLE),
      animation_sub(SUB_NONE),
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
