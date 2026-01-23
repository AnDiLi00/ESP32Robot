#include "Matrix.h"

const uint8_t Matrix::BRIGHTNESS_DEFAULT = 6;
const uint8_t Matrix::BRIGHTNESS_MIN = 0;
const uint8_t Matrix::BRIGHTNESS_MAX = 15;

const uint8_t Matrix::CYCLES_UPDATE = 2;
const uint8_t Matrix::CYCLES_LOOP = 0;

const int8_t Matrix::SIZE = 8;

const String Matrix::DRAW_IMAGE = "*";

Matrix::Matrix(void) :
  data() {
}

Matrix::Matrix(const Matrix &copy) :
  data() {

  data.matrix = copy.data.matrix;
  data.brightness = copy.data.brightness;

  data.cycles_animation = copy.data.cycles_animation;
  data.cycle_animation = copy.data.cycle_animation;
  data.cycle_update = copy.data.cycle_update;

  data.direction = copy.data.direction;
  data.offset = copy.data.offset;

  data.text = copy.data.text;
  data.current = copy.data.current;

  data.image = copy.data.image;
}

Matrix::~Matrix(void) {
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (&other != this) {
    data.matrix = other.data.matrix;
    data.brightness = other.data.brightness;

    data.cycles_animation = other.data.cycles_animation;
    data.cycle_animation = other.data.cycle_animation;
    data.cycle_update = other.data.cycle_update;

    data.direction = other.data.direction;
    data.offset = other.data.offset;

    data.text = other.data.text;
    data.current = other.data.current;

    data.image = other.data.image;
  }

  return (*this);
}

void Matrix::SetMatrix(Adafruit_LEDBackpack *matrix) {
  if (matrix != data.matrix) {
    data.matrix = matrix;

    if (data.matrix != NULL) {
      data.matrix->begin(0x70);
      data.matrix->setBrightness(data.brightness);
      data.matrix->clear();
      data.matrix->writeDisplay();
    }
  }
}

void Matrix::SetMatrixBrightness(const uint8_t &brightness) {
  if (brightness != data.brightness) {
    data.brightness = brightness;

    if (data.matrix != NULL) {
      data.matrix->setBrightness(data.brightness);
    }
  }
}

void Matrix::SetImage(const uint64_t &image, const Types::Direction &direction, const uint8_t &cycles) {
  if (image != data.image) {
    data.image = image;
  }

  SetText(DRAW_IMAGE.c_str(), direction, cycles);
}

void Matrix::SetText(const char *text, const Types::Direction &direction, const uint8_t &cycles) {
  data.cycles_animation = cycles;
  data.cycle_animation = 0;

  data.direction = direction;
  data.offset = 0;

  if (text != data.text) {
    data.text = text;
    data.current = (char *)data.text;

    if (*data.current == DRAW_IMAGE[0]) {
      data.image = 0;
    }
  }
}

void Matrix::Clear(void) {
  data.cycle_animation = 0;

  data.offset = 0;

  data.text = NULL;
  data.current = NULL;

  data.image = 0;
}

void Matrix::OnSetup(const Types::Mood &mood, const Types::MoodSub &submood) {
}

void Matrix::OnLoop(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  if (data.cycle_update == 0) {
    data.matrix->clear();

    DrawCharacter(data.current, data.offset);

    if ((data.direction != Types::DIR_NO) &&
        ((data.cycles_animation == CYCLES_LOOP) || (data.cycle_animation < (data.cycles_animation - 1)))) {
      data.offset++;
      if (data.offset == SIZE) {
        data.offset = 0;

        data.current++;
        if (*data.current == '\0') {
          data.current = (char *)data.text;

          if (data.cycles_animation != CYCLES_LOOP) {
            data.cycle_animation++;
          }
        }
      }
    }

    data.matrix->writeDisplay();
  }

  data.cycle_update = (data.cycle_update + 1) % CYCLES_UPDATE;
}

void Matrix::OnEnd(void) {
}

void Matrix::DrawCharacter(const char *character, const int8_t &offset) {
  uint64_t image = GetImage(character);

  char *next = (char *)character + 1;
  if (*next == '\0') {
    next = (char *)data.text;
  }
  uint64_t image_next = GetImage(next);

  DrawImage(image, image_next, offset);
}

void Matrix::DrawImage(const uint64_t &image, const uint64_t &image_next, const int8_t &offset) {
  for (int8_t i = 0; i < SIZE; i++) {
    uint8_t row = 0;

    switch (data.direction) {
      case Types::DIR_DOWN:
      case Types::DIR_UP:
        {
          uint8_t r1 = (image >> i * SIZE) & 0xFF;
          uint8_t r2 = (image_next >> i * SIZE) & 0xFF;

          uint8_t r1_shifted = (data.direction == Types::DIR_UP) ? ((r1 >> offset) & 0xFF) : ((r1 << offset) & 0xFF);
          uint8_t r2_shifted = (data.direction == Types::DIR_UP) ? ((r2 << (SIZE - offset)) & 0xFF) : ((r2 >> (SIZE - offset)) & 0xFF);

          row = r1_shifted | r2_shifted;
        }
        break;
      case Types::DIR_LEFT:
      case Types::DIR_RIGHT:
        {
          int8_t shifted_i = (data.direction == Types::DIR_RIGHT) ? (i + offset) : (i - offset);
          if ((shifted_i >= 0) && (shifted_i < SIZE)) {
            row = (image >> shifted_i * SIZE) & 0xFF;
          } else {
            shifted_i = (shifted_i + SIZE) % SIZE;
            row = (image_next >> shifted_i * SIZE) & 0xFF;
          }
        }
        break;
      case Types::DIRECTIONS:
        break;
    }

    data.matrix->displaybuffer[i] = row;
  }
}

uint64_t Matrix::GetImage(const char *character) {
  uint64_t image = 0;

  if ((character[0] >= '0') && (character[0] <= '1')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'0';
    image = IMG_DIGITS[index];
  } else if ((character[0] >= 'A') && (character[0] <= 'Z')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'A';
    image = IMG_LETTERS_BIG[index];
  } else if ((character[0] >= 'a') && (character[0] <= 'z')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'a';
    image = IMG_LETTERS_SMALL[index];
  } else if (character[0] == DRAW_IMAGE[0]) {
    image = data.image;
  } else {
    for (uint8_t i = 0; i < IMG_SIGNS_SIZE; i++) {
      if (character[0] == IMG_SIGNS[i].sign) {
        image = IMG_SIGNS[i].image;
        break;
      }
    }
  }

  return (image);
}
