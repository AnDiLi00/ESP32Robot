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

  static const uint64_t IMG_DIGITS[] = {
    0x003f0c0c0c0c0e0c, // 1
    0x003f33061c30331e,
    0x001e33301c30331e,
    0x0078307f33363c38,
    0x001e3330301f033f,
    0x001e33331f03061c,
    0x000c0c0c1830333f,
    0x001e33331e33331e,
    0x000e18303e33331e, // 9
    0x003e676f7b73633e // 0
  };

  static const uint64_t IMG_LETTERS[] = {
    0x0033333f33331e0c, // A
    0x003f66663e66663f,
    0x003c66030303663c,
    0x001f36666666361f,
    0x007f46161e16467f,
    0x000f06161e16467f,
    0x007c66730303663c,
    0x003333333f333333,
    0x001e0c0c0c0c0c1e,
    0x001e333330303078,
    0x006766361e366667,
    0x007f66460606060f,
    0x0063636b7f7f7763,
    0x006363737b6f6763,
    0x001c36636363361c,
    0x000f06063e66663f,
    0x00381e3b3333331e,
    0x006766363e66663f,
    0x001e33380e07331e,
    0x001e0c0c0c0c2d3f,
    0x003f333333333333,
    0x000c1e3333333333,
    0x0063777f6b636363,
    0x0063361c1c366363,
    0x001e0c0c1e333333,
    0x007f664c1831637f, // Z
    0x0000000000000000, // SPACE
    0x006e333e301e0000, // a
    0x003b66663e060607,
    0x001e3303331e0000,
    0x006e33333e303038,
    0x001e033f331e0000,
    0x000f06060f06361c,
    0x1f303e33336e0000,
    0x006766666e360607,
    0x001e0c0c0c0e000c,
    0x1e33333030300030,
    0x0067361e36660607,
    0x001e0c0c0c0c0c0e,
    0x00636b7f7f330000,
    0x00333333331f0000,
    0x001e3333331e0000,
    0x0f063e66663b0000,
    0x78303e33336e0000,
    0x000f06666e3b0000,
    0x001f301e033e0000,
    0x00182c0c0c3e0c08,
    0x006e333333330000,
    0x000c1e3333330000,
    0x00367f7f6b630000,
    0x0063361c36630000,
    0x1f303e3333330000,
    0x003f260c193f0000 // z
  };

  static const uint64_t IMG_SIGNS[] = {
    0x00000c0c3f0c0c00, // +
    0x000000003f000000, // -
    0x0000663cff3c6600, // *
    0x000103060c183060, // /
    0x0063660c18336300, // %
    0x00003f00003f0000, // =
    0x0000000000003b6e, // ~
    0x0000000063361c08, // ^
    0x00180c0603060c18, // <
    0x00060c1830180c06, // >
    0x00180c0606060c18, // (
    0x00060c1818180c06, // )
    0x001e06060606061e, // [
    0x001e18181818181e, // ]
    0x00380c0c070c0c38, // {
    0x00070c0c380c0c07, // }
    0x000c0c0000000000, // .
    0x000c0c00000c0c00, // :
    0x060c0c00000c0c00, // ;
    0x060c0c0000000000, // ,
    0x00180018183c3c18, // !
    0x000c000c1830331e, // ?
    0x001e037b7b7b633e, // @
    0x006e333b6e1c361c, // &
    0x000c1f301e033e0c, // $
    0x0036367f367f3636, // #
    0x00406030180c0603, // '\'
    0x0000000000180c0c, // 'left ''
    0x0000000000030606, // 'right ''
    0x00081c3e7f7f7722, // 'heart'
    0x00081c3e7f3e1c08, // 'diamond'
    0x001c082a7f2a1c08, // 'club'
    0x001c083e7f3e1c08, // 'spade'
    0x000609094e506078, // 'male'
    0x001c22221c083e08, // 'female'
    0x00060e0c08082818, // 'note'
    0x0003377664444c78, // 'notes'
    0x00082a1c771c2a08, // 'sun'
    0x003c7e4200666600, // 'smile'
    0x00427e3c00666600, // 'frown'
    0x007e7e0000664200, // 'angry'
    0x003c427e00660000 // 'laugh'
  };

private:
  Types(void);
  virtual ~Types(void);
};

#endif
