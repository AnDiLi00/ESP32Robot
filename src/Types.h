#ifndef __types_h__
#define __types_h__

class Types {
public:
  enum EyeType {
    EYE_LEFT,
    EYE_RIGHT,
    EYES
  };

  enum EyePosition {
    POS_CENTER,
    POS_CENTER_LEFT,
    POS_CENTER_RIGHT,
    POS_TOP,
    POS_TOP_LEFT,
    POS_TOP_RIGHT,
    POS_BOTTOM,
    POS_BOTTOM_LEFT,
    POS_BOTTOM_RIGHT,
    POSITIONS
  };

  enum Mood {
    MOOD_NORMAL,
    MOOD_CONFUSED,
    MOOD_ANGRY,
    MOOD_TIRED,
    MOOD_CLOSED,
    MOODS
  };

  enum AnimType {
    ANIM_IDLE,
    ANIM_BLINK,
    ANIM_SHAKE
  };

  enum AnimSubType {
    SUB_NONE,
    SUB_CLOSING,
    SUB_OPENING,
  };

private:
  Types(void);
  virtual ~Types(void);
};

#endif
