#include <stdio.h>

int main()
{
    int a = 10, b = -20;
    if(a > 0)
    {
        if(b > 0)
            printf("Both are positive\n");
        else
            printf("a positive, b negative\n");
    }
    else
    {
        printf("a is negative\n");
    }
    if(0) {
        printf("Unreachable code\n");
    }
    return 0;
}
