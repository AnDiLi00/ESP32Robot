#ifndef __animation_h__
#define __animation_h__

#include "Eyes.h"
#include "Matrix.h"
#include "Types.h"

class Animation {
public:
  static const unsigned long TIME_DEFAULT;
  static const unsigned long TIME_UPDATE;
  static const unsigned long TIME_IDLE_MIN;
  static const unsigned long TIME_IDLE_VARIANCE;
  static const unsigned long TIME_BLINK_MIN;
  static const unsigned long TIME_TRANSITION_MIN;

  static const String TEST;

  Animation(void);
  Animation(const Animation &copy);
  virtual ~Animation(void);
  Animation &operator=(const Animation &other);

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetMatrix(Adafruit_LEDBackpack *matrix);

  virtual void SetMood(const Types::Mood &mood);
  virtual void SetSubMood(const Types::MoodSub &mood_sub);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual void DoUpdate(const unsigned long &now);
  virtual void DoAnimationTest(const unsigned long &now);

  virtual unsigned long GetIdleDuration(void) const;
  virtual unsigned long GetAnimationSteps(void) const;

  virtual void PrintMood(void) const;
  virtual void PrintSubMood(void) const;

  struct Data {
    Types::Mood mood;
    Types::MoodSub mood_sub;

    Types::AnimType animation;
    Types::AnimSubType animation_sub;

    unsigned long last_update;
    unsigned long last_idle;
    unsigned long duration_idle;

    Eyes eyes;
    Matrix matrix;

    bool test;
    int8_t test_anim1;
    int8_t test_anim2;

    Data(void) :
      mood(Types::MOOD_NORMAL),
      mood_sub(Types::MSUB_NORMAL),
      animation(Types::ANIM_IDLE),
      animation_sub(Types::SUB_NONE),
      last_update(TIME_DEFAULT),
      last_idle(TIME_DEFAULT),
      duration_idle(TIME_DEFAULT),
      eyes(Eyes()),
      test(true),
      test_anim1(0),
      test_anim2(0) {
    }
  };

  Data data;
};

#endif
