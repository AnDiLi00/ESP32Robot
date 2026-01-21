#ifndef __matrixdisplay_h__
#define __matrixdisplay_h__

#include "Types.h"

class MatrixDisplay {
public:
  static const uint8_t WIDTH;
  static const uint8_t HEIGHT;

  MatrixDisplay(void);
  MatrixDisplay(const MatrixDisplay &copy);
  virtual ~MatrixDisplay(void);
  MatrixDisplay &operator=(const MatrixDisplay &other);

protected:
  virtual void DrawImage(const uint64_t &image, const uint8_t &offset);

  struct Data {
    Types::Direction direction;

    Data(void) :
      direction(Types::DIR_UP) {
    }
  };

  Data data;
};

#endif
