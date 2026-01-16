#include "Esp32RobotEyes.h"

const uint16_t Esp32RobotEyes::DISPLAY_WIDTH = 128;
const uint16_t Esp32RobotEyes::DISPLAY_HEIGHT = 64;
const uint16_t Esp32RobotEyes::DISPLAY_RESET = 4;

const unsigned long Esp32RobotEyes::TIME_DEFAULT = 0;
const unsigned long Esp32RobotEyes::TIME_UPDATE = 20;
const unsigned long Esp32RobotEyes::TIME_IDLE = 5000;

Esp32RobotEyes::Esp32RobotEyes(void) :
  data() {
}

Esp32RobotEyes::Esp32RobotEyes(const Esp32RobotEyes &copy) :
  data() {

  data.display = copy.data.display;

  data.mood = copy.data.mood;
  data.position = copy.data.position;

  data.animation = copy.data.animation;
  data.animation_sub = copy.data.animation_sub;

  data.last_update = copy.data.last_update;
  data.last_idle = copy.data.last_idle;
  data.duration = copy.data.duration;

  data.anim = copy.data.anim;

  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    data.eyes[i] = copy.data.eyes[i];
    data.eyes_new[i] = copy.data.eyes_new[i];
  }
}

Esp32RobotEyes::~Esp32RobotEyes(void) {
}

Esp32RobotEyes &Esp32RobotEyes::operator=(const Esp32RobotEyes &other) {
  if (&other != this) {
    data.display = other.data.display;

    data.mood = other.data.mood;
    data.position = other.data.position;

    data.animation = other.data.animation;
    data.animation_sub = other.data.animation_sub;

    data.last_update = other.data.last_update;
    data.last_idle = other.data.last_idle;
    data.duration = other.data.duration;

    data.anim = other.data.anim;

    for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
      data.eyes[i] = other.data.eyes[i];
      data.eyes_new[i] = other.data.eyes_new[i];
    }
  }

  return (*this);
}

void Esp32RobotEyes::SetDisplay(Adafruit_SSD1306 *display) {
  data.display = display;
}

void Esp32RobotEyes::SetMood(const Esp32RobotEye::Mood &mood) {
  if (mood != data.mood) {
    data.mood = mood;

    switch (data.mood) {
      case Esp32RobotEye::MOOD_NORMAL:
        Serial.println("mood=normal");
        break;
      case Esp32RobotEye::MOOD_CONFUSED:
        Serial.println("mood=confused");
        break;
      case Esp32RobotEye::MOOD_ANGRY:
        Serial.println("mood=angry");
        break;
      case Esp32RobotEye::MOOD_TIRED:
        Serial.println("mood=tired");
        break;
      case Esp32RobotEye::MOOD_CLOSED:
        Serial.println("mood=closed");
        break;
      case Esp32RobotEye::MOODS:
        Serial.println("mood=moods?!");
        break;
    }
  }
}

void Esp32RobotEyes::SetPosition(const Esp32RobotEye::Position &position) {
  if (position != data.position) {
    data.position = position;

    switch (data.position) {
      case Esp32RobotEye::POS_CENTER:
        Serial.println("position=center");
        break;
      case Esp32RobotEye::POS_CENTER_LEFT:
        Serial.println("position=center left");
        break;
      case Esp32RobotEye::POS_CENTER_RIGHT:
        Serial.println("position=center right");
        break;
      case Esp32RobotEye::POS_TOP:
        Serial.println("position=top center");
        break;
      case Esp32RobotEye::POS_TOP_LEFT:
        Serial.println("position=top left");
        break;
      case Esp32RobotEye::POS_TOP_RIGHT:
        Serial.println("position=top right");
        break;
      case Esp32RobotEye::POS_BOTTOM:
        Serial.println("position=bottom center");
        break;
      case Esp32RobotEye::POS_BOTTOM_LEFT:
        Serial.println("position=bottom left");
        break;
      case Esp32RobotEye::POS_BOTTOM_RIGHT:
        Serial.println("position=bottom right");
        break;
      case Esp32RobotEye::POSITIONS:
        Serial.println("position=positions?!");
        break;
    }
  }
}

void Esp32RobotEyes::OnSetup(void) {
  if (data.display == NULL) {
    return;
  }

  data.display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  data.display->clearDisplay();
  data.display->display();

  data.duration = GetRandomIdleDuration();

  Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), data.mood, data.position, data.eyes);
  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    data.eyes_new[i] = data.eyes[i];
  }
}

void Esp32RobotEyes::OnLoop(void) {
  if (data.display == NULL) {
    return;
  }

  unsigned long now = millis();
  unsigned long difference_idle = now - data.last_idle;
  unsigned long difference_update = now - data.last_update;

  switch (data.animation) {
    case ANIM_IDLE:
      if (difference_idle >= data.duration) {
        if (data.anim % 2 == 0) {
          // blink
          data.animation = ANIM_BLINK;
          data.animation_sub = SUB_CLOSING;
          Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), Esp32RobotEye::MOOD_CLOSED, data.position, data.eyes_new);
        } else {
          // switch mood
          data.last_idle = now;
          data.duration = GetRandomIdleDuration();

          int8_t new_mood = (int8_t)data.mood;
          new_mood++;
          new_mood %= (int8_t)Esp32RobotEye::MOODS;
          SetMood((Esp32RobotEye::Mood)new_mood);
          Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), data.mood, data.position, data.eyes_new);
        }
        data.anim++;
      }
      break;
    case ANIM_BLINK:
      switch (data.animation_sub) {
        case SUB_NONE:
          break;
        case SUB_CLOSING:
          if (CheckEyes() == true) {
            data.animation_sub = SUB_OPENING;
            Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), data.mood, data.position, data.eyes_new);
          }
          break;
        case SUB_OPENING:
          if (CheckEyes() == true) {
            data.last_idle = now;
            data.duration = GetRandomIdleDuration();

            data.animation = ANIM_IDLE;
            data.animation_sub = SUB_NONE;
            Esp32RobotEye::GetEyes(data.display->width(), data.display->height(), data.mood, data.position, data.eyes_new);
          }
          break;
      }
      break;
    case ANIM_SHAKE:
      break;
  }

  if (difference_update >= TIME_UPDATE) {
    data.last_update = now;

    data.display->clearDisplay();

    DrawBackground();
    DrawEyes();

    data.display->display();
  }
}

void Esp32RobotEyes::OnEnd(void) {
}

void Esp32RobotEyes::DrawBackground(void) {
  data.display->fillScreen(SSD1306_BLACK);
}

void Esp32RobotEyes::DrawEyes(void) {
  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    Esp32RobotEye::UpdateEye(data.eyes[i], data.eyes_new[i]);

    data.eyes[i].Draw(data.display, (Esp32RobotEye::Eye)i, data.mood);
  }
}

unsigned long Esp32RobotEyes::GetRandomIdleDuration(void) {
  unsigned long random_number = (unsigned long)random(TIME_IDLE / 4);
  return (TIME_IDLE + random_number);
}

bool Esp32RobotEyes::CheckEyes(void) {
  bool ret = true;

  for (uint8_t i = 0; i < Esp32RobotEye::EYES; i++) {
    if (data.eyes[i] != data.eyes_new[i]) {
      ret = false; 
      break;
    }
  }

  return (ret);
}