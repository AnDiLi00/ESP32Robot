#include "Matrix.h"

const uint8_t Matrix::BRIGHTNESS_DEFAULT = 8;

const int8_t Matrix::WIDTH = 8;
const int8_t Matrix::HEIGHT = 8;
const int8_t Matrix::OFFSET_DEFAULT = 0;

Matrix::Matrix(void) :
  data() {
}

Matrix::Matrix(const Matrix &copy) :
  data() {

  data.matrix = copy.data.matrix;

  data.direction = copy.data.direction;
  data.offset = copy.data.offset;
}

Matrix::~Matrix(void) {
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (&other != this) {
    data.matrix = other.data.matrix;

    data.direction = other.data.direction;
    data.offset = other.data.offset;
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

void Matrix::OnSetup(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  data.matrix->begin(0x70);
  data.matrix->setBrightness(BRIGHTNESS_DEFAULT);
  data.matrix->clear();

  data.matrix->writeDisplay();
}

void Matrix::OnLoop(const Types::Mood &mood, const Types::MoodSub &submood) {
  if (data.matrix == NULL) {
    return;
  }

  data.matrix->clear();

  uint64_t test = 0x00081c3e7f7f7722;
  DrawImage(test, data.offset, 0xf800);

  int8_t size = HEIGHT;
  if ((data.direction == Types::DIR_LEFT) ||(data.direction == Types::DIR_RIGHT)) {
    size = WIDTH;
  }
  data.offset = (data.offset + 1) % size;

  data.matrix->writeDisplay();
}

void Matrix::OnEnd(void) {
}

void Matrix::DrawImage(const uint64_t &image, const int8_t &offset, const uint16_t &color) {
  for (int8_t i = 0; i < HEIGHT; i++) {
    uint8_t row = (image >> i * WIDTH) & 0xFF;

    switch (data.direction) {
      case Types::DIR_UP:
      case Types::DIR_DOWN:
        int shifted_i = (data.direction == Types::DIR_UP) ? i - offset : i + offset;
        if ((shifted_i < 0) || (shifted_i >= HEIGHT)) {
          continue;
        }
        break;
      case Types::DIR_LEFT:
        row = (row << offset) & 0xFF;
        break;
      case Types::DIR_RIGHT:
        row = (row >> offset) & 0xFF;
        break;
    }

    data.matrix->displaybuffer[i] = row;
  }
}
