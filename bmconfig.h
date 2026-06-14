#include <stdint.h>

const int RENDER_WIDTH = 50;
const char SYMBOLS[] = {'.',':','-','=','+','*','#','%','@'};
const char* CLR = "\x1b[48;2;";
const char* BG = "\x1b[48;2;0;0;0m";
const char* BOLD = "\x1b[1m";
const char* CLEAR = "\x1b[0m";

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
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int16_t  biXPelsPerMeter;
  int16_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} __attribute__((packed)) BITMAPINFOHEADER;

typedef struct
{
  uint8_t rgbtBlue;
  uint8_t rgbtGreen;
  uint8_t rgbtRed;
} __attribute__((packed)) RGBTRIPLE;