#include <stdio.h>
#include <string.h>
#include "bmpspec.h"

float lerp(float x, int x1, int v1, int x2, int v2);
RGBTRIPLE lerp_px(RGBTRIPLE px1, RGBTRIPLE px2, float pos);

const char SYMBOLS[] = {'.',':','-','=','+','*','#','%','@'};
const int N = sizeof(SYMBOLS) / sizeof(SYMBOLS[0]);

const char* CLR = "\x1b[48;2;";
const char* BG = "\x1b[48;2;0;0;0m";
const char* BOLD = "\x1b[1m";
const char* CLEAR = "\x1b[0m";

const int RENDER_WIDTH = 100;

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
            "Error: unrecognised file type (expected 0x424D, got 0x%04X)\n",
            bmp_fhead.bfType);
        return 4;
    }

    // Read info header
    BITMAPINFOHEADER bmp_ihead;
    uint8_t buffer[84]; // Largest possible number of buffered bytes
    if (fread(&bmp_ihead, sizeof(BITMAPINFOHEADER), 1, f) != 1)
    {
        fprintf(stderr, "Error: failed to read BITMAPINFOHEADER");
        return 5;
    }

    // Buffer extra bytes in case DIBHEADER isn't using BITMAPINFOHEADER
    if (bmp_ihead.biSize != sizeof(BITMAPINFOHEADER))
    {
        fread(&buffer, bmp_ihead.biSize - sizeof(BITMAPINFOHEADER), 1, f);
    }

    // Ensure 24 bits / pixel
    if (bmp_ihead.biBitCount != 0x0018)
    {
        fprintf(stderr, "Error: No support for bit count (expected 0x0018, got 0x%04X)\n",
        bmp_ihead.biBitCount);
        return 7;
    }

    // Initialise array for image
    const int HEIGHT = bmp_ihead.biHeight;
    const int WIDTH = bmp_ihead.biWidth;
    RGBTRIPLE img_arr[HEIGHT * WIDTH];

    // Go to beginning of image data
    fseek(f, bmp_fhead.bfOffBits, 0);

    // Read image data
    int pad = (4 - (sizeof(RGBTRIPLE) * WIDTH) % 4) % 4;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            // printf("%i %i\n", i, j);
            RGBTRIPLE cur;
            fread(&cur, sizeof(RGBTRIPLE), 1, f);
            img_arr[i * WIDTH + j] = cur;
        }
        // RGBTRIPLE cur = img_arr[i * WIDTH];
        // printf("%s%i;%i;%imREAD %i 0 - %i, %i, %i\n", CLR, cur.rgbtRed, cur.rgbtGreen, cur.rgbtBlue, i, cur.rgbtRed, cur.rgbtGreen, cur.rgbtBlue);

        // Skip padding
        fseek(f, pad, SEEK_CUR);
    }

    // Rescale image content
    const float RESCALE = RENDER_WIDTH / (float) WIDTH;
    const int RENDER_HEIGHT = (int) (HEIGHT * RESCALE);
    RGBTRIPLE rescaled[RENDER_WIDTH * RENDER_HEIGHT];

    for (int i = 0; i < RENDER_HEIGHT; i++)
    {
        for (int j = 0; j < RENDER_WIDTH; j++)
        {
            rescaled[i * RENDER_WIDTH + j] = (RGBTRIPLE) {0, 0, 0};
        }
    }

    // Initialise loop variables
    // printf("width: %i\nrescale: %f\nrheight: %i\n", WIDTH, RESCALE, RENDER_HEIGHT);
    RGBTRIPLE r1, r2, q11, q21, q12, q22;
    float or_x, or_y, tx, ty;

    // Perform interpolation
    for (int i = 0; i < RENDER_HEIGHT; i++)
    {
        for (int j = 0; j < RENDER_WIDTH; j++)
        {
            // Find where output pixel position maps to in original image by undoing scaling
            // Position of output pixel x';y' maps to position or_x;or_y on original img
            or_x = (j + 0.5) * (1 / RESCALE), tx = or_x - (int) or_x + 0.5;
            or_y = (i + 0.5) * (1 / RESCALE), ty = or_y - (int) or_y + 0.5;
            
            q11 = img_arr[(int) or_y * WIDTH + (int) or_x],
            q21 = img_arr[(int) or_y * WIDTH + (int) (or_x + 1)], 
            q12 = img_arr[(int) (or_y + 1) * WIDTH + (int) or_x],
            q22 = img_arr[(int) (or_y + 1) * WIDTH + (int) (or_x + 1)];

            // Bilinearly interpolate to sample color
            r1 = lerp_px(q11, q21, tx);
            r2 = lerp_px(q12, q22, tx);

            // Set new pixel value
            RGBTRIPLE f = lerp_px(r1, r2, ty);
            printf("LERP %s%i;%i;%imx | %s%i;%i;%imx%s, GOT %s%i;%i;%imx%s\n", CLR, r1.rgbtRed, r1.rgbtGreen, r1.rgbtBlue, CLR, r2.rgbtRed, r2.rgbtGreen, r2.rgbtBlue, CLEAR, CLR, f.rgbtRed, f.rgbtGreen, f.rgbtBlue, CLEAR);
            printf("SET %s%i;%i;%imx%s\n", CLR, f.rgbtRed, f.rgbtGreen, f.rgbtBlue, CLEAR);
            rescaled[i * RENDER_WIDTH + j] = f;
        }
    }

    // Render image in terminal using text characters
    RGBTRIPLE cur;
    int avg, br;
    for (int i = 0; i < RENDER_HEIGHT; i++)
    {
        for (int j = 0; j < RENDER_WIDTH; j++)
        {
            cur = rescaled[(RENDER_HEIGHT - i) * RENDER_WIDTH + j]; // Image data is stored in little-endian
            avg = (int) ((cur.rgbtRed + cur.rgbtGreen + cur.rgbtBlue)/3);

            // Calculate brightness index          
            br = (int) ((avg * N) / 256);
            
            // Render pixel
            printf("%s%s%s%i;%i;%im%c ",
                   BG, BOLD, CLR, cur.rgbtRed, cur.rgbtGreen, cur.rgbtBlue, SYMBOLS[br]);
        }
        printf("%s\n", CLEAR);
    }
    fclose(f);
    return 0;
}

// Performs linear interpolation between x1 and x2
float lerp(float x, int x1, int v1, int x2, int v2)
{
    float t = (x-x1)/(x2-x1);
    return v1*(1-t) + v2*t;
}

RGBTRIPLE lerp_px(RGBTRIPLE px1, RGBTRIPLE px2, float pos)
{
    // printf("%s%i;%i;%imPX1: %i, %i, %i\x1b[0m |||||| ", CLR, px1.rgbtRed, px1.rgbtGreen, px1.rgbtBlue, px1.rgbtRed, px1.rgbtGreen, px1.rgbtBlue);
    // printf("%s%i;%i;%imPX2: %i, %i, %i\x1b[0m\n", CLR, px2.rgbtRed, px2.rgbtGreen, px2.rgbtBlue, px2.rgbtRed, px2.rgbtGreen, px2.rgbtBlue);
    return (RGBTRIPLE)
    {
        lerp(pos, 0, px1.rgbtBlue, 2, px2.rgbtBlue),
        lerp(pos, 0, px1.rgbtGreen, 2, px2.rgbtGreen),
        lerp(pos, 0, px1.rgbtRed, 2, px2.rgbtRed)
    };
}