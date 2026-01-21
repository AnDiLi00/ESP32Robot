#include "MatrixDisplay.h"

const uint8_t MatrixDisplay::PIXEL_W;
const uint8_t MatrixDisplay::PIXEL_H;

MatrixDisplay::MatrixDisplay(void) :
  data() {
}

MatrixDisplay::MatrixDisplay(const MatrixDisplay &copy) :
  data() {

  data.direction = copy.data.direction;
}

MatrixDisplay::~MatrixDisplay(void) {
}

MatrixDisplay &MatrixDisplay::operator=(const MatrixDisplay &other) {
  if (&other != this) {
    data.direction = other.data.direction;
  }

  return (*this);
}

void MatrixDisplay::DrawImage(const uint64_t &image, const uint8_t &offset) {
  for (uint8_t i = 0; i < WIDTH; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (uint8_t j = 0; j < HEIGHT; j++) {
      //display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
