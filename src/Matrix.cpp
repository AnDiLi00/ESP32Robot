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

void Matrix::SetMatrix(Adafruit_8x8matrix *matrix) {
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
  if (data.display == NULL) {
    return;
  }

  data.matrix->clear();

  uint64_t test = 0x8100000000000081;
  DrawImage(test, data.offset, 0xf800);

  data.matrix->writeDisplay();
}

void Matrix::OnEnd(void) {
}

void Matrix::DrawImage(const uint64_t &image, const int8_t &offset, const uint16_t &color) {
  for (int8_t i = 0; i < WIDTH; i++) {
    byte row = (image >> i * WIDTH) & 0xFF;
    for (int8_t j = 0; j < HEIGHT; j++) {
      if (bitRead(row, j) == 1) {
        data.matrix->drawPixel(i, j, color);
      }
    }
  }
}
