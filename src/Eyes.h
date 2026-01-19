#ifndef __eyes_h__
#define __eyes_h__

#include <Adafruit_SSD1306.h>

#include "Eye.h"

class Eyes {
public:
  static const uint16_t DISPLAY_WIDTH;
  static const uint16_t DISPLAY_HEIGHT;
  static const uint16_t DISPLAY_RESET;

  Eyes(void);
  Eyes(const Eyes &copy);
  virtual ~Eyes(void);
  Eyes &operator=(const Eyes &other);

  virtual bool AreTransitioned(void) const;

  virtual void SetDisplay(Adafruit_SSD1306 *display);
  virtual void SetPosition(const Eye::Position &position);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

  virtual void OnMoodChange(const Animation::Mood &mood);

protected:
  virtual void DrawBackground(void);
  virtual void DrawEyes(void);

  struct Data {
    Adafruit_SSD1306 *display;
    Eye::Position position;

    Eye eyes[Eye::EYES];
    Eye eyes_new[Eye::EYES];

    Data(void) :
      display(NULL),
      position(Eye::POS_CENTER) {
    }
  };

  Data data;
};

#endif
