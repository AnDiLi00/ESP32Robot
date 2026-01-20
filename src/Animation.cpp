#include "Animation.h"

const unsigned long Animation::TIME_DEFAULT = 0;
const unsigned long Animation::TIME_UPDATE = 20;
const unsigned long Animation::TIME_IDLE_MIN = 4200;
const unsigned long Animation::TIME_IDLE_VARIANCE = 1600;
const unsigned long Animation::TIME_BLINK_MIN = 100;
const unsigned long Animation::TIME_TRANSITION_MIN = 100;

Animation::Animation(void) :
  data() {
}

Animation::Animation(const Animation &copy) :
  data() {

  data.mood = copy.data.mood;

  data.animation = copy.data.animation;
  data.animation_sub = copy.data.animation_sub;

  data.last_update = copy.data.last_update;
  data.last_idle = copy.data.last_idle;
  data.duration_idle = copy.data.duration_idle;

  data.eyes = Eyes(copy.data.eyes);

  data.anim = copy.data.anim;
}

Animation::~Animation(void) {
}

Animation &Animation::operator=(const Animation &other) {
  if (&other != this) {
    data.mood = other.data.mood;

    data.animation = other.data.animation;
    data.animation_sub = other.data.animation_sub;

    data.last_update = other.data.last_update;
    data.last_idle = other.data.last_idle;
    data.duration_idle = other.data.duration_idle;

    data.eyes = Eyes(other.data.eyes);

    data.anim = other.data.anim;
  }

  return (*this);
}

void Animation::SetDisplay(Adafruit_SSD1306 *display) {
  data.eyes.SetDisplay(display);
}

void Animation::SetMood(const Types::Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;

    switch (data.mood) {
      case Types::MOOD_NORMAL:
        Serial.println("mood=normal");
        break;
      case Types::MOOD_CONFUSED:
        Serial.println("mood=confused");
        break;
      case Types::MOOD_ANGRY:
        Serial.println("mood=angry");
        break;
      case Types::MOOD_TIRED:
        Serial.println("mood=tired");
        break;
      case Types::MOOD_CLOSED:
        Serial.println("mood=closed");
        break;
      case Types::MOODS:
        Serial.println("mood=moods?!");
        break;
    }
  }
}

void Animation::OnSetup(void) {
  data.eyes.OnSetup(data.mood);
}

void Animation::OnLoop(void) {
  unsigned long now = millis();
  unsigned long difference_update = now - data.last_update;

  if (difference_update >= TIME_UPDATE) {
    data.last_update = now;

    DoUpdate(now);

    data.eyes.OnLoop(data.mood);
  }
}

void Animation::OnEnd(void) {
  data.eyes.OnEnd();
}

void Animation::DoUpdate(const unsigned long &now) {
  unsigned long difference_idle = now - data.last_idle;

  switch (data.animation) {
    case Types::ANIM_IDLE:
      if (difference_idle >= data.duration_idle) {
        if (data.anim % 2 == 0) {
          // blink
          data.animation = Types::ANIM_BLINK;
          data.animation_sub = Types::SUB_CLOSING;

          data.eyes.OnMoodChange(Types::MOOD_CLOSED, GetAnimationSteps());
        } else {
          // switch mood
          data.last_idle = now;
          data.duration_idle = GetIdleDuration();

          int8_t new_mood = ((int8_t)data.mood + 1) % (int8_t)Types::MOODS;
          SetMood((Types::Mood)new_mood);

          data.eyes.OnMoodChange(data.mood, GetAnimationSteps());
        }
        data.anim++;
      }
      break;
    case Types::ANIM_BLINK:
      switch (data.animation_sub) {
        case Types::SUB_NONE:
          break;
        case Types::SUB_CLOSING:
          if (data.eyes.AreTransitioned() == true) {
            data.animation_sub = Types::SUB_OPENING;

            data.eyes.OnMoodChange(data.mood, GetAnimationSteps());
          }
          break;
        case Types::SUB_OPENING:
          if (data.eyes.AreTransitioned() == true) {
            data.last_idle = now;
            data.duration_idle = GetIdleDuration();

            data.animation = Types::ANIM_IDLE;
            data.animation_sub = Types::SUB_NONE;

            data.eyes.OnMoodChange(data.mood, GetAnimationSteps());
          }
          break;
      }
      break;
    case Types::ANIM_SHAKE:
      break;
  }
}

unsigned long Animation::GetIdleDuration(void) {
  unsigned long random_idle = ((TIME_IDLE_MIN + (unsigned long)random(TIME_IDLE_VARIANCE)) / TIME_UPDATE) * TIME_UPDATE;

  return (random_idle);
}

unsigned long Animation::GetAnimationSteps(void) {
  unsigned long steps = 0;

  switch (data.animation) {
    case Types::ANIM_IDLE:
      steps = TIME_TRANSITION_MIN / TIME_UPDATE;
      break;
    case Types::ANIM_BLINK:
      switch (data.animation_sub) {
        case Types::SUB_NONE:
          break;
        case Types::SUB_CLOSING:
        case Types::SUB_OPENING:
          steps = TIME_BLINK_MIN / TIME_UPDATE;
          break;
      }
      break;
    case Types::ANIM_SHAKE:
      break;
  }

  return (steps);
}
