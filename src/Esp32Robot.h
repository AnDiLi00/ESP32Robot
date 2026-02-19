#ifndef __esp32robot_h__
#define __esp32robot_h__

#include "Animation.h"
#include "Movement.h"
#include "Types.h"

class Esp32Robot : public Animation, public Movement {
public:
  Esp32Robot(void);
  Esp32Robot(const Esp32Robot &copy);
  virtual ~Esp32Robot(void);
  Esp32Robot &operator=(const Esp32Robot &other);

  virtual void OnSetup(void);
  virtual void OnLoop(void);
  virtual void OnEnd(void);

protected:
  struct Data {
    unsigned long last_update;

    Data(void) :
      last_update(Types::TIME_DEFAULT) {
    }
  };

  Data data;
};

#endif
