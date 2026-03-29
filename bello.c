// MinionLang playground sample
#include <stdio.h>

int square(int n) {
    return n * n;
}

int main() {
    int a = 5;
    int b = 2 + 3 * 4;

    if (a > 3) {
        printf("a is %d", a);
    } else {
        printf("small a");
    }

    int s = square(a);
    printf("square is %d", s);
    printf("b = %d", b);

    return 0;
}