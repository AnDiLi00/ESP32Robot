#include "Matrix.h"

const uint8_t Matrix::BRIGHTNESS_DEFAULT = 6;

const uint8_t Matrix::UPDATE_CYCLES = 5;
const uint8_t Matrix::CYCLES_DEFAULT = 0;

const int8_t Matrix::SIZE = 8;
const int8_t Matrix::OFFSET_DEFAULT = 0;

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

  String newtext = "Das ist ein Test! ] / ";
  SetText(newtext.c_str());

  data.matrix->writeDisplay();
}

void Matrix::OnLoop(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  if (data.cycle == 0) {
    data.matrix->clear();

    DrawCharacter(data.current, data.offset);

    /*
    char *next = data.current + 1;
    if (*next == '\0') {
      next = (char *)data.text;
    }

    DrawCharacter(next, data.offset + SIZE);
    */

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
  if ((character[0] >= '0') && (character[0] <= '1')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'0';
    DrawImage(IMG_DIGITS[index], offset);
  } else if ((character[0] >= 'A') && (character[0] <= 'Z')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'A';
    DrawImage(IMG_LETTERS_BIG[index], offset);
  } else if ((character[0] >= 'a') && (character[0] <= 'z')) {
    uint8_t index = (uint8_t)character[0] - (uint8_t)'a';
    DrawImage(IMG_LETTERS_SMALL[index], offset);
  } else {
    for (uint8_t i = 0; i < IMG_SIGNS_SIZE; i++) {
      if (character[0] == IMG_SIGNS[i].sign) {
        DrawImage(IMG_SIGNS[i].image, offset);
        break;
      }
    }
  }
}

void Matrix::DrawImage(const uint64_t &image, const int8_t &offset) {
  for (int8_t i = 0; i < SIZE; i++) {
    uint8_t row = (image >> i * SIZE) & 0xFF;

    switch (data.direction) {
      case Types::DIR_UP:
        row = (row >> offset) & 0xFF;
        break;
      case Types::DIR_DOWN:
        row = (row >> offset) & 0xFF;
        break;
      case Types::DIR_LEFT:
      case Types::DIR_RIGHT:
        int shifted_i = (data.direction == Types::DIR_LEFT) ? i - offset : i + offset;
        if ((shifted_i < 0) || (shifted_i >= SIZE)) {
          continue;
        }
        break;
    }

    data.matrix->displaybuffer[i] = row;
  }
}
