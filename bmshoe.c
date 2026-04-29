#include <stdio.h>
#include <string.h>

const char SYMBOLS[] = {'.', '-', '=', '+'};

int main(int argc, char** argv)
{
    // Ensure correct usage
    if (argc != 2)
    {
        printf("Correct usage: ./bmshoe file.bmp\n");
        return 1;
    }

    // Open .bmp file for reading
    char* f_name = argv[1];
    FILE* f = fopen(f_name, "r");
    if (f == NULL)
    {
        printf("Could not open %s.", f_name);
        return 1;
    }
}