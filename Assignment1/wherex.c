#include <stdio.h>

int main(void) {
    int x;
    printf("The answer is almost 42\n");
    x = wherex();
    printf("horizontal cursor position from where this text appears = %d\n", x);
    return 0;
}