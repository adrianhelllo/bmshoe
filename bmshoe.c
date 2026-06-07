#include <stdio.h>
#include <string.h>
#include "bmpspec.h"

float lerp(x, x1, v1, x2, v2);
RGBTRIPLE lerp_px(float pos_x, float pos_y, RGBTRIPLE img[], int img_w);

const char SYMBOLS[] = {'.', '-', '=', '+', '%', '&', '#', '@'};
const int RENDER_WIDTH = 50;

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
    RGBTRIPLE img_arr[HEIGHT * WIDTH];

    // Go to beginning of image data
    fseek(f, 0, bmp_fhead.bfOffBits);

    // Read image data
    int i = 0;
    while (fread(&img_arr[i], sizeof(RGBTRIPLE), 1, f) == 1)
    {
        i++;
    }

    // Rescale image content
    float rescale_f = RENDER_WIDTH / WIDTH;
    const int RENDER_HEIGHT = (int) (HEIGHT * rescale_f);
    RGBTRIPLE rescaled[RENDER_WIDTH * RENDER_HEIGHT];

    RGBTRIPLE lerp1, lerp2;
    float or_x, or_y, t;
    int x1, x2, row;
    for (int i = 0; i < RENDER_HEIGHT; i++)
    {
        for (int j = 0; j < RENDER_WIDTH; j++)
        {
            // Find where output pixel position maps to in original image by undoing scaling
            // Position of output px x';y' maps to position or_x;or_y on original img
            or_x = (RENDER_WIDTH + 0.5) * (WIDTH / RENDER_WIDTH);
            or_y = (RENDER_HEIGHT + 0.5) * (HEIGHT / RENDER_HEIGHT);

            // Bilinearly interpolate to sample color
            lerp1 = lerp_px(or_x, or_y, img_arr, WIDTH);
            lerp2 = lerp_px(or_x, or_y + 1, img_arr, WIDTH);

            rescaled[i * RENDER_WIDTH + j] = lerp_px(or_x, or_y, img_arr, WIDTH);
        }
    }

    fclose(f);
    return 0;
}

// Performs linear interpolation between x1 and x2
float lerp(float x, int x1, int v1, int x2, int v2)
{
    int t = (x-x1)/(x2-x1);
    return v1*(1-t) + v2*t;
}

RGBTRIPLE lerp_px(float pos_x, float pos_y, RGBTRIPLE img[], int img_w)
{
    int x1 = (int) pos_x, x2 = (int) (pos_x + 1);
    int r = (int) pos_y;

    return (RGBTRIPLE) {
        lerp(pos_x, x1, img[r * img_w + x1].rgbtRed, x2, img[r * img_w + x2].rgbtRed),
        lerp(pos_x, x1, img[r * img_w + x1].rgbtRed, x2, img[r * img_w + x2].rgbtRed),
        lerp(pos_x, x1, img[r * img_w + x1].rgbtRed, x2, img[r * img_w + x2].rgbtRed),
    }
}