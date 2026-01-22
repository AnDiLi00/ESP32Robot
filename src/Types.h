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

  enum MoodSub {
    MSUB_NORMAL,
    MSUB_HAPPY,
    SUBMOODS
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

  enum MovementMode {
    MOVE_DRIVE,
    MOVE_WALK
  };

  enum BodyParts {
    PART_LEFT_DRIVE,
    PART_LEFT_ANKLE,
    PART_LEFT_ARM,
    PART_RIGHT_DRIVE,
    PART_RIGHT_ANKLE,
    PART_RIGHT_ARM,
    PART_PARTS
  };

  enum Direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
  };

private:
  Types(void);
  virtual ~Types(void);
};

#endif
