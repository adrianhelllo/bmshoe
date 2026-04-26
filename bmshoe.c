#include <stdio.h>

const char SYMBOLS[] = {'.', '-', '=', '+'};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Correct usage: ./bmshoe file.bmp\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Could not open %s.", argv[1]);
        return 1;
    }
    
    printf("%p", f);
}