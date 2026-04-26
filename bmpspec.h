#include <stdint.h>

// The following Bitmap-specifications are adapted from Microsoft's standards

typedef struct 
{
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct
{
  uint32_t biSize;
  int16_t  biWidth;
  int16_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int16_t  biXPelsPerMeter;
  int16_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} __attribute__((packed)) BITMAPINFOHEADER;