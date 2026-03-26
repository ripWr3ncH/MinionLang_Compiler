// Execution Proof Test 2: For, While, Do-While
#include <stdio.h>

int main() {
    for(int i = 1 ; i <= 3 ; i = i + 1) {
        printf("F%d\n", i);
    }

    int j = 0;
    while (j < 2) {
        printf("W%d\n", j);
        j = j + 1;
    }

    int k = 2;
    do {
        printf("D%d\n", k);
        k = k - 1;
    } while (k > 0);

    return 0;
}
