#ifndef __esp32roboteyes_h__
#define __esp32roboteyes_h__

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Esp32RobotEyes {
public:
  static const uint8_t WIDTH_DEFAULT;
  static const uint8_t HEIGHT_DEFAULT;

  static const uint8_t EYE_WIDTH;
  static const uint8_t EYE_HEIGHT;
  static const uint8_t EYE_DISTANCE;
  static const uint8_t EYE_CORNER;
  static const uint8_t EYE_BORDER;

  static const uint16_t COLOR_DEFAULT;
  static const uint16_t COLOR_BACKGROUND_DEFAULT;

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
  virtual void SetDisplayDimensions(uint8_t width, uint8_t height);

  virtual void SetEyeColor(uint8_t red, uint8_t green, uint8_t blue);
  virtual void SetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);

  virtual void SetMood(const Mood &mood);
  virtual void SetPosition(const Position &position);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  virtual uint16_t GetColor(uint8_t red, uint8_t green, uint8_t blue) const;

  virtual void DrawBackground(void);
  virtual void DrawEyes(void);

  struct Data {
    Adafruit_SSD1306 *display;
    uint8_t width;
    uint8_t height;

    uint16_t color;
    uint16_t color_background;

    unsigned long last;

    Mood mood;
    Position position;

    Data(void) :
      display(NULL),
      width(WIDTH_DEFAULT),
      height(HEIGHT_DEFAULT),
      color(COLOR_DEFAULT),
      color_background(COLOR_BACKGROUND_DEFAULT),
      last(TIME_LAST_DEFAULT),
      mood(MOOD_NORMAL),
      position(POS_CENTER) {
    }
  };

  Data data;
};

#endif
