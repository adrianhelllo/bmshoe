#include <stdio.h>
#include <string.h>
#include "bmpspec.h"

const char SYMBOLS[] = {'.', '-', '=', '+', '%', '&', '#', '@'};

int main(int argc, char** argv)
{
    // Ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./bmshoe image.bmp");
        return 1;   
    }
        
    // Open .bmp file for reading
    char* f_name = argv[argc - 1];
    FILE* f = fopen(f_name, "rb");

    // Return if opening the file was unsuccessful
    if (f == NULL)
    {
        fprintf(stderr, "Error: could not open %s.", f_name);
        return 2;
    }

    // Read file header
    BITMAPFILEHEADER bmp_fhead;
    if (fread(&bmp_fhead, sizeof(BITMAPFILEHEADER), 1, f) != 1) {
        fprintf(stderr, "Error: failed to read BMPFILEHEADER");
        return 3;
    }

    // Check for file type
    if (bmp_fhead.bfType != 0x4d42)
    {
        fprintf(stderr,
            "Error: unrecognised file type (expected 0x424D, got 0x%04X\n",
            bmp_fhead.bfType);
        return 4;
    }

    // Read info header
    BITMAPINFOHEADER bmp_ihead;
    if (fread(&bmp_ihead, sizeof(BITMAPINFOHEADER), 1, f) != 1)
    {
        fprintf(stderr, "Error: failed to read BITMAPINFOHEADER");
        return 5;
    }

    // Initialise array for image
    const int HEIGHT = bmp_ihead.biHeight;
    const int WIDTH = bmp_ihead.biWidth;
    RGBTRIPLE img_arr[HEIGHT][WIDTH];

    // Go to beginning of image data
    fseek(f, 0, bmp_fhead.bfOffBits);

    // Read image data
    int i = 0;
    while (fread(&img_arr[i], sizeof(RGBTRIPLE), 1, f) == 1)
    {
        i++;
    }

    // Calculate new pixel values
    for (int i = 0; i < WIDTH; i++)
    {
        
    }

    fclose(f);

    return 0;
}