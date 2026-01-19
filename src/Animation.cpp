#include "Animation.h"

const unsigned long Animation::TIME_DEFAULT = 0;
const unsigned long Animation::TIME_UPDATE = 20;
const unsigned long Animation::TIME_IDLE = 5000;
const unsigned long Animation::TIME_IDLE_VARIANCE = 1600;

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
  data.duration = copy.data.duration;

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
    data.duration = other.data.duration;

    data.eyes = Eyes(other.data.eyes);

    data.anim = other.data.anim;
  }

  return (*this);
}

void Animation::SetMood(const Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;

    switch (data.mood) {
      case MOOD_NORMAL:
        Serial.println("mood=normal");
        break;
      case MOOD_CONFUSED:
        Serial.println("mood=confused");
        break;
      case MOOD_ANGRY:
        Serial.println("mood=angry");
        break;
      case MOOD_TIRED:
        Serial.println("mood=tired");
        break;
      case MOOD_CLOSED:
        Serial.println("mood=closed");
        break;
      case MOODS:
        Serial.println("mood=moods?!");
        break;
    }
  }
}

void Animation::OnSetup(void) {
  data.eyes.OnSetup();
}

void Animation::OnLoop(void) {
  unsigned long now = millis();
  unsigned long difference_update = now - data.last_update;

  if (difference_update >= TIME_UPDATE) {
    data.last_update = now;

    DoUpdate(now);

    data.eyes.OnLoop();
  }
}

void Animation::OnEnd(void) {
  data.eyes.OnEnd();
}

void Animation::DoUpdate(const unsigned long &now) {
  unsigned long difference_idle = now - data.last_idle;

  switch (data.animation) {
    case ANIM_IDLE:
      if (difference_idle >= data.duration) {
        if (data.anim % 2 == 0) {
          // blink
          data.animation = ANIM_BLINK;
          data.animation_sub = SUB_CLOSING;

          data.eyes.MoodChange(MOOD_CLOSED);
        } else {
          // switch mood
          data.last_idle = now;
          data.duration = GetIdleDuration();

          int8_t new_mood = ((int8_t)data.mood + 1) % (int8_t)MOODS;
          SetMood((Mood)new_mood);

          data.eyes.OnMoodChange(data.mood);
        }
        data.anim++;
      }
      break;
    case ANIM_BLINK:
      switch (data.animation_sub) {
        case SUB_NONE:
          break;
        case SUB_CLOSING:
          if (data.eyes.AreTransitioned() == true) {
            data.animation_sub = SUB_OPENING;

            data.eyes.OnMoodChange(data.mood);
          }
          break;
        case SUB_OPENING:
          if (data.eyes.AreTransitioned() == true) {
            data.last_idle = now;
            data.duration = GetIdleDuration();

            data.animation = ANIM_IDLE;
            data.animation_sub = SUB_NONE;

            data.eyes.OnMoodChange(data.mood);
          }
          break;
      }
      break;
    case ANIM_SHAKE:
      break;
  }
}

unsigned long Animation::GetIdleDuration(void) {
  unsigned long random_idle = (unsigned long)random(TIME_IDLE_VARIANCE);
  return (TIME_IDLE - TIME_IDLE_VARIANCE / 2 + random_idle);
}
