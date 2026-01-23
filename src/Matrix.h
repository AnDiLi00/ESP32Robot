#ifndef __matrix_h__
#define __matrix_h__

#include <Adafruit_LEDBackpack.h>

#include "Types.h"

class Matrix {
public:
  static const uint8_t BRIGHTNESS_DEFAULT;
  static const uint8_t BRIGHTNESS_MIN;
  static const uint8_t BRIGHTNESS_MAX;

  static const uint8_t CYCLES_UPDATE;
  static const uint8_t CYCLES_LOOP;

  static const int8_t SIZE;

  static const String DRAW_IMAGE;
  static const String TEST;

  struct Special {
    int sign;
    uint64_t image;
  };

  static constexpr const uint64_t IMG_DIGITS[] = {
    0x007cfe928afe7c00,
    0x008088fefe808000,
    0x00c4e6a2929e8c00,
    0x0044c69292fe6c00,
    0x00302824fefe2000,
    0x004ece8a8afa7200,
    0x007cfe9292f66400,
    0x000606e2fa1e0600,
    0x006cfe9292fe6c00,
    0x004cde9292fe7c00
  };
  static constexpr const uint8_t IMG_DIGITS_SIZE = sizeof(IMG_DIGITS) / sizeof(IMG_DIGITS[0]);

  static constexpr const uint64_t IMG_LETTERS_BIG[] = {
    0x00fcfe1212fefc00,
    0x00fefe9292fe6c00,
    0x007cfe8282c64400,
    0x00fefe8282fe7c00,
    0x00fefe9292928200,
    0x00fefe1212120200,
    0x007cfe82a2e66400,
    0x00fefe1010fefe00,
    0x000082fefe820000,
    0x0060e082fe7e0200,
    0x00fefe386cc68200,
    0x00fefe8080808000,
    0x00fefe0c180cfefe,
    0x00fefe0c1830fefe,
    0x007cfe8282fe7c00,
    0x00fefe22223e1c00,
    0x003c7e4262febc00,
    0x00fefe3272de8c00,
    0x004cde9292f66400,
    0x000602fefe020600,
    0x007efe8080fefe00,
    0x003e7ec0c07e3e00,
    0x00fefe603060fefe,
    0x00c6ee381038eec6,
    0x000e1ef0f01e0e00,
    0x00c2e2b29a8e8600
  };
  static constexpr const uint8_t IMG_LETTERS_BIG_SIZE = sizeof(IMG_LETTERS_BIG) / sizeof(IMG_LETTERS_BIG[0]);

  static constexpr const uint64_t IMG_LETTERS_SMALL[] = {
    0x0040e8a8a8f8f000,
    0x00fefe9090f06000,
    0x0070f88888d85000,
    0x0060f09090fefe00,
    0x0070f8a8a8b83000,
    0x0020fcfe22260400,
    0x0018bca4a4fc7c00,
    0x00fefe1010f0e000,
    0x000080f4f4800000,
    0x0060e080fa7a0000,
    0x00fefe2070d88800,
    0x000000fefe000000,
    0x00f8f830e030f8f8,
    0x00f8f81818f8f000,
    0x0070f88888f87000,
    0x00fcfc24243c1800,
    0x00183c24fcfc80c0,
    0x00f8f80808383000,
    0x0090a8a8a8a84800,
    0x001010fcfc101000,
    0x0078f88080f8f800,
    0x003070c0c0703000,
    0x0078f880f080f878,
    0x0088d87070d88800,
    0x0018b8a0a0f87800,
    0x0000c8e8b8980000
  };
  static constexpr const uint8_t IMG_LETTERS_SMALL_SIZE = sizeof(IMG_LETTERS_SMALL) / sizeof(IMG_LETTERS_SMALL[0]);

  static constexpr const Special IMG_SIGNS[] = {
     { ' ', 0x0000000000000000 }, // space
     { '.', 0x00c0c00000000000 }, // .
     { ':', 0x0000006c6c000000 }, // :
     { ';', 0x000080ec6c000000 }, // ;
     { ',', 0x0080f07000000000 }, // ,
     { '!', 0x00000cbebe0c0000 }, // !
     { '?', 0x000406b2b21e0c00 }, // ?
     { '/', 0x001c3e7cf87c3e1c }, // heart == '/'
     { '$', 0x0010387cfe7c3810 }, // diamond == '$'
     { '%', 0x00103894fe943810 }, // club == '%'
     { '&', 0x001038bcfebc3810 }, // spade == '&'
     { '+', 0x00609090720a061e }, // male == '+'
     { '-', 0x000064949e946400 }, // female == '-'
     { '(', 0x00c0e07e02040000 }, // note == '('
     { ')', 0x00c0e07c0662723e }, // notes == ')'
     { '{', 0x00105438ee385410 }, // sun == '{'
     { '}', 0x0026464040462600 }, // smile == '}'
     { '[', 0x0046262020264600 }, // frown == '['
     { ']', 0x0026242020242600 } // angry == ']'
  };
  static constexpr const uint8_t IMG_SIGNS_SIZE = sizeof(IMG_SIGNS) / sizeof(IMG_SIGNS[0]);

  Matrix(void);
  Matrix(const Matrix &copy);
  virtual ~Matrix(void);
  Matrix &operator=(const Matrix &other);

  virtual void SetMatrix(Adafruit_LEDBackpack *matrix);
  virtual void SetMatrixBrightness(const uint8_t &brightness);

  virtual void SetImage(const uint64_t &image, const Types::Direction &direction = Types::DIR_LEFT, const uint8_t &cycles = CYCLES_LOOP);
  virtual void SetText(const char *text, const Types::Direction &direction = Types::DIR_LEFT, const uint8_t &cycles = CYCLES_LOOP);
  virtual void Clear(void);

  virtual void OnSetup(const Types::Mood &mood, const Types::MoodSub &submood);
  virtual void OnLoop(const Types::Mood &mood, const Types::MoodSub &submood);
  virtual void OnEnd(void);

protected:
  virtual void DrawCharacter(const char *character, const int8_t &offset);
  virtual void DrawImage(const uint64_t &image, const uint64_t &image_next, const int8_t &offset);

  virtual uint64_t GetImage(const char *character);

  struct Data {
    Adafruit_LEDBackpack *matrix;
    uint8_t brightness;

    uint8_t cycles_animation;
    uint8_t cycle_animation;
    uint8_t cycle_update;

    Types::Direction direction;
    int8_t offset;

    const char *text;
    char *current;

    uint64_t image;

    Data(void) :
      matrix(NULL),
      brightness(BRIGHTNESS_DEFAULT),
      cycles_animation(CYCLES_LOOP),
      cycle_animation(0),
      cycle_update(0),
      direction(Types::DIR_NO),
      offset(0),
      text(NULL),
      current(NULL),
      image(0) {
    }
  };

  Data data;
};

#endif
