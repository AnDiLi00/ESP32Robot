#include "Matrix.h"

const uint8_t Matrix::BRIGHTNESS_DEFAULT = 6;

const uint8_t Matrix::UPDATE_CYCLES = 2;
const uint8_t Matrix::CYCLES_DEFAULT = 0;

const int8_t Matrix::SIZE = 8;
const int8_t Matrix::OFFSET_DEFAULT = 0;

const String Matrix::TEST = "Das ist ein Test /$%&+- ";
const String Matrix::TEST2 = "Test ";
const String Matrix::TEST3 = "D";

Matrix::Matrix(void) :
  data() {
}

Matrix::Matrix(const Matrix &copy) :
  data() {

  data.matrix = copy.data.matrix;
  data.cycle = copy.data.cycle;

  data.direction = copy.data.direction;
  data.offset = copy.data.offset;

  data.text = copy.data.text;
  data.current = copy.data.current;
}

Matrix::~Matrix(void) {
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (&other != this) {
    data.matrix = other.data.matrix;
    data.cycle = other.data.cycle;

    data.direction = other.data.direction;
    data.offset = other.data.offset;

    data.text = other.data.text;
    data.current = other.data.current;
  }

  return (*this);
}

void Matrix::SetMatrix(Adafruit_LEDBackpack *matrix) {
  data.matrix = matrix;
}

void Matrix::SetDirection(const Types::Direction &direction) {
  if (direction != data.direction) {
    data.direction = direction;
  }
}

void Matrix::SetText(const char *text) {
  if (text != data.text) {
    data.offset = 0;

    data.text = text;
    data.current = (char *)data.text;
  }
}

void Matrix::OnSetup(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  data.matrix->begin(0x70);
  data.matrix->setBrightness(BRIGHTNESS_DEFAULT);
  data.matrix->clear();

  SetText(TEST.c_str());

  data.matrix->writeDisplay();
}

void Matrix::OnLoop(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  if (data.cycle == 0) {
    data.matrix->clear();

    DrawCharacter(data.current, data.offset);

    if (data.direction != Types::DIR_NO) {
      data.offset++;
      if (data.offset == SIZE) {
        data.offset = 0;

        data.current++;
        if (*data.current == '\0') {
          data.current = (char *)data.text;
        }
      }
    }

    data.matrix->writeDisplay();
  }

  data.cycle = (data.cycle + 1) % UPDATE_CYCLES;
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
    uint8_t row = 0x00;

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
