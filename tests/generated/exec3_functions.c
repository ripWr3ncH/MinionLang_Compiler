// Execution Proof Test 3: Functions and Return
#include <stdio.h>

int addTwo(int a, int b) {
    return a + b;
}

int main() {
    int result = addTwo(4, 6);
    printf("R=%d\n", result);
    return 0;
}
