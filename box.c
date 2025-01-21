#include <stdio.h>

void box (int x, int y);
void width (int x, char c);
void height(int x, int y, char c);

int main(void) {
    box (10, 10);
    printf("\n");
    return 0;
}

void box (int x, int y) {
    width(x, '-');
    printf("\n");
    height(x, y/2, ':');
    width(x, '-');
}

void width (int x, char c) {
    for (int width = 0; width < x; ++width) {
            printf("%c", c);
    }
}

void height(int x, int y, char c) {
    for (int height = 0; height < y; ++height) {
        printf("%c", c);
        width(x - 2, ' ');
        printf("%c\n", c);
    }
}
