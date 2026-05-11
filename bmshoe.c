#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char SYMBOLS[] = {'.', '-', '=', '+', '%', '&', '#', '@'};

typedef struct
{
    char flagopt;
    char* description;
    int has_val; // 0 or 1
} optflag;

const optflag OPTINFO[] = {
    {'?', "Lists all available options for the program", 0},
    {'h', "Specifies a desired display height for the image to be shown", 1},
    {'w', "Specifies a desired display width for the image to be shown", 1},
};

const int OPTARR_NUM = sizeof(OPTINFO) / sizeof(OPTINFO[0]);

char *OPTS = ":?w:h:";


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
    FILE* f = fopen(f_name, "r");
    if (f == NULL)
    {
        printf("Could not open %s.", f_name);
        return 2;
    }
}