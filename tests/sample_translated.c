// MinionLang Sample Program
// Demonstrates all language features

#include <stdio.h>
#include <math.h>
#define MAX_VALUE 100

/* 
   This is a multi-line comment
   MinionLang is fun!
*/

int main() {
    // Variable declarations
    int x = 10;
    int y = 5;
    float pi = 3.14159;
    double bigNum = 123456.789;
    char letter = 'A';
    bool flag = 1;
    long int bigInt = 9999999;
    
    // Arithmetic operations
    int sum = x + y;
    int diff = x - y;
    int product = x * y;
    int quotient = x / y;
    int remainder = x % y;
    
    // If-else statement
    if (x > y) {
        printf("x is greater than y");
    } else if (x == y) {
        printf("x is equal to y");
    } else {
        printf("x is less than y");
    }
    
    // Logical operators
    if (flag && (x > 0)) {
        printf("Flag is true AND x is positive");
    }
    
    if (! flag || (y <= 0)) {
        printf("Either flag is false OR y is non-positive");
    }
    
    // For loop
    for(int i = 0 ; i < 10 ; i = i + 1) {
        printf("Loop iteration");
    }
    
    // While loop
    int count = 0;
    while(count < 5) {
        printf("While loop");
        count = count + 1;
    }
    
    // Do-while loop
    do {
        printf("Do-while loop");
        count = count - 1;
    } while(count > 0)
    
    // Switch statement
    switch(x) {
        case 1:
            printf("x is 1");
            break;
        case 10:
            printf("x is 10");
            break;
        default:
            printf("x is something else");
    }
    
    // Math functions
    float sqrtVal = sqrt(16.0);
    float powVal = pow(2.0, 3.0);
    float floorVal = floor(3.7);
    float ceilVal = ceil(3.2);
    int absVal = abs(- 5);
    float logVal = log(10.0);
    float sinVal = sin(0.5);
    float cosVal = cos(0.5);
    float tanVal = tan(0.5);
    float asinVal = asin(0.5);
    float acosVal = acos(0.5);
    float atanVal = atan(0.5);
    bool isPrime = isPrime(7);
    
    // Input from user
    int userInput;
    scanf("%d", userInput);
    
    // Relational operators test
    if (x < y) { printf("less than"); }
    if (x > y) { printf("greater than"); }
    if (x <= y) { printf("less or equal"); }
    if (x >= y) { printf("greater or equal"); }
    if (x == y) { printf("equal"); }
    if (x != y) { printf("not equal"); }
    
    // XOR operation
    if (flag ^ (x > 0)) {
        printf("XOR is true");
    }
    
    // Array declaration
    int arr[10];
    arr[0] = 1;
    arr[1] = 2;
    
    // Continue statement test
    for(int j = 0 ; j < 5 ; j = j + 1) {
        if (j == 2) {
            continue;
        }
        printf("Value of j");
    }
    
    return 0;
}

// Function with return type indicator
void printMessage()   {
    printf("Hello from MinionLang!");
}

int addNumbers(int a, int b)   {
    return a + b;
}
