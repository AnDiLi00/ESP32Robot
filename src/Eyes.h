#ifndef __eyes_h__
#define __eyes_h__

#include <Adafruit_SSD1306.h>

#include "Eye.h"
#include "Types.h"

class Eyes {
public:
  static const uint16_t DISPLAY_WIDTH;
  static const uint16_t DISPLAY_HEIGHT;
  static const uint16_t DISPLAY_RESET;

  Eyes(void);
  Eyes(const Eyes &copy);
  virtual ~Eyes(void);
  Eyes &operator=(const Eyes &other);

  virtual bool AreTransitioned(void);

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetPosition(const Types::EyePosition &position);

  virtual void OnSetup(const Types::Mood &mood);
  virtual void OnLoop(const Types::Mood &mood);
  virtual void OnEnd(void);

  virtual void OnMoodChange(const Types::Mood &mood);

protected:
  virtual void DrawBackground(void);
  virtual void DrawEyes(const Types::Mood &mood);

  struct Data {
    Adafruit_SSD1306 *display;
    Types::EyePosition position;

    Eye eyes[Types::EYES];
    Eye eyes_new[Types::EYES];

    Data(void) :
      display(NULL),
      position(Types::POS_CENTER) {
    }
  };

  Data data;
};

#endif
