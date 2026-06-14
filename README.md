# Inline ANSI image renderer for Bitmap images in C

A CLI tool for rendering 24-bit BMP files via ANSI color codes.

## Usage

To render an image, run the compiled code and supply the image to be rendered:
```c
./bmshoe img.bmp
```
For invalid use cases, an error message and code will be given.

## Configuration

Configuration variables can be found atop the bmconfig.h header file.