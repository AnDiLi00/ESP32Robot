#include "Animation.h"

const unsigned long Animation::TIME_DEFAULT = 0;
const unsigned long Animation::TIME_UPDATE = 20;
const unsigned long Animation::TIME_IDLE_MIN = 5000;
const unsigned long Animation::TIME_IDLE_VARIANCE = 2000;
const unsigned long Animation::TIME_BLINK_MIN = 100;
const unsigned long Animation::TIME_TRANSITION_MIN = 60;

const String Animation::TEST = "Test / ";

Animation::Animation(void) :
  data() {
}

Animation::Animation(const Animation &copy) :
  data() {

  data.mood = copy.data.mood;
  data.mood_sub = copy.data.mood_sub;

  data.animation = copy.data.animation;
  data.animation_sub = copy.data.animation_sub;

  data.last_update = copy.data.last_update;
  data.last_idle = copy.data.last_idle;
  data.duration_idle = copy.data.duration_idle;

  data.eyes = Eyes(copy.data.eyes);
  data.matrix = Matrix(copy.data.matrix);

  data.test = copy.data.test;
  data.test_anim1 = copy.data.test_anim1;
  data.test_anim2 = copy.data.test_anim2;
}

Animation::~Animation(void) {
}

Animation &Animation::operator=(const Animation &other) {
  if (&other != this) {
    data.mood = other.data.mood;
    data.mood_sub = other.data.mood_sub;

    data.animation = other.data.animation;
    data.animation_sub = other.data.animation_sub;

    data.last_update = other.data.last_update;
    data.last_idle = other.data.last_idle;
    data.duration_idle = other.data.duration_idle;

    data.eyes = Eyes(other.data.eyes);
    data.matrix = Matrix(other.data.matrix);

    data.test = other.data.test;
    data.test_anim1 = other.data.test_anim1;
    data.test_anim2 = other.data.test_anim2;
  }

  return (*this);
}

void Animation::SetDisplay(Adafruit_SSD1306 *display) {
  data.eyes.SetDisplay(display);
}

void Animation::SetMatrix(Adafruit_LEDBackpack *matrix) {
  data.matrix.SetMatrix(matrix);
}

void Animation::SetMood(const Types::Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;
  }
}

void Animation::SetSubMood(const Types::MoodSub &mood_sub) {
  if (mood_sub != data.mood_sub) {
    data.mood_sub = mood_sub;
  }
}

void Animation::OnSetup(void) {
  data.eyes.OnSetup(data.mood, data.mood_sub);
  data.matrix.OnSetup(data.mood, data.mood_sub);

  if (data.test == true) {
    data.matrix.SetText(TEST.c_str());
  }
}

void Animation::OnLoop(void) {
  unsigned long now = millis();
  unsigned long difference_update = now - data.last_update;

  if (difference_update >= TIME_UPDATE) {
    data.last_update = now;

    DoUpdate(now);

    data.eyes.OnLoop(data.mood, data.mood_sub);
    data.matrix.OnLoop(data.mood, data.mood_sub);
  }
}

void Animation::OnEnd(void) {
  data.eyes.OnEnd();
  data.matrix.OnEnd();
}

void Animation::DoUpdate(const unsigned long &now) {
  unsigned long difference_idle = now - data.last_idle;

  switch (data.animation) {
    case Types::ANIM_IDLE:
      if (difference_idle >= data.duration_idle) {
        if (data.test == true) {
          DoAnimationTest(now);
        } else {
        }
      }
      break;
    case Types::ANIM_BLINK:
      switch (data.animation_sub) {
        case Types::SUB_NONE:
          break;
        case Types::SUB_CLOSING:
          if (data.eyes.AreTransitioned() == true) {
            data.animation_sub = Types::SUB_OPENING;

            data.eyes.OnMoodChange(data.mood, data.mood_sub, GetAnimationSteps());
          }
          break;
        case Types::SUB_OPENING:
          if (data.eyes.AreTransitioned() == true) {
            data.last_idle = now;
            data.duration_idle = GetIdleDuration();

            data.animation = Types::ANIM_IDLE;
            data.animation_sub = Types::SUB_NONE;

            data.eyes.OnMoodChange(data.mood, data.mood_sub, GetAnimationSteps());
          }
          break;
      }
      break;
    case Types::ANIM_SHAKE:
      break;
  }
}

void Animation::DoAnimationTest(const unsigned long &now) {
  if (data.test_anim1 % 2 == 0) {
    // blink
    data.animation = Types::ANIM_BLINK;
    data.animation_sub = Types::SUB_CLOSING;

    data.eyes.OnMoodChange(Types::MOOD_CLOSED, data.mood_sub, GetAnimationSteps());

    uint8_t new_direction = (uint8_t)random((uint8_t)(Types::DIRECTIONS - 1)) + 1;
    uint8_t new_cycles = (uint8_t)random((uint8_t)(Matrix::CYCLES_LOOP + 3));
    data.matrix.SetText(TEST.c_str(), (Types::Direction)new_direction, new_cycles);
  } else {
    // switch through all moods and submoods
    data.last_idle = now;
    data.duration_idle = GetIdleDuration();

    if (data.test_anim2 % 2 == 0) {
      int8_t new_mood = ((int8_t)data.mood + 1) % (int8_t)Types::MOODS;
      SetMood((Types::Mood)new_mood);
      SetSubMood(Types::MSUB_NORMAL);
    } else {
      int8_t new_submood = ((int8_t)data.mood_sub + 1) % (int8_t)Types::SUBMOODS;
      SetSubMood((Types::MoodSub)new_submood);
    }

    data.eyes.OnMoodChange(data.mood, data.mood_sub, GetAnimationSteps());

    if (data.mood != Types::MOOD_CLOSED) {
      data.test_anim2++;
    } else {
      data.test_anim1++;
    }

    data.matrix.Clear();
  }

  data.test_anim1++;
}

unsigned long Animation::GetIdleDuration(void) const {
  unsigned long random_idle = ((TIME_IDLE_MIN + (unsigned long)random(TIME_IDLE_VARIANCE)) / TIME_UPDATE) * TIME_UPDATE;

  return (random_idle);
}

unsigned long Animation::GetAnimationSteps(void) const {
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

void Animation::PrintMood(void) const {
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

void Animation::PrintSubMood(void) const {
  switch (data.mood_sub) {
    case Types::MSUB_NORMAL:
      Serial.println("submood=normal");
      break;
    case Types::MSUB_HAPPY:
      Serial.println("submood=happy");
      break;
    case Types::SUBMOODS:
      Serial.println("submood=submoods?!");
      break;
  }
}
