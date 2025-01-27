#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PRECISION 100

typedef struct {
    char integer_part;
    bool is_negative;
    char decimal_part[PRECISION + 1]; // Extra for null terminator
} LongNumber;

void init_long_number(LongNumber* num, int value) {
    num->is_negative = false;
    if (value < 0) {
        num->integer_part = -(value / 1000000000);
        num->is_negative = true;
    } else {
        num->integer_part = value / 1000000000;
    }
    // Initialize decimal part to zero
    for(int i=0; i<PRECISION; i++) {
        num->decimal_part[i] = '0';
    }
    num->decimal_part[PRECISION] = '\0';
}

void print_long_number(LongNumber num) {
    if(num.is_negative) printf("-");
    printf("%d.", num.integer_part);
    for(int i=0; i<PRECISION; i++) {
        printf("%c", num.decimal_part[i]);
    }
    printf("\n");
}

// Add two long numbers
LongNumber add_long_number(LongNumber a, LongNumber b) {
    LongNumber result = {0, false, {'0'}};
    // Implement addition logic here...
    return result;
}

// Subtract two long numbers
LongNumber subtract_long_number(LongNumber a, LongNumber b) {
    LongNumber result = {0, false, {'0'}};
    // Implement subtraction logic here...
    return result;
}

// Multiply two long numbers
LongNumber multiply_long_number(LongNumber a, LongNumber b) {
    LongNumber result = {0, false, {'0'}};
    // Implement multiplication logic here...
    return result;
}

// Compute square root using Newton-Raphson method
LongNumber sqrt_long_number(LongNumber n) {
    LongNumber result = {1, false, {'0'}};
    // Implement square root logic here...
    return result;
}

void compute_pi() {
    LongNumber a = {2, false, 
"00000000000000000000000000000000000000000000000000"};
    LongNumber b = {1, false, 
"00000000000000000000000000000000000000000000000000"};
    
    for(int i=0; i<100; i++) {
        // Compute new_a as (a + b)/2
        LongNumber sum = add_long_number(a, b);
        LongNumber new_a = {sum.integer_part / 2, false, ""};
        // Update a and b for next iteration
        a = new_a;
        LongNumber product = multiply_long_number(new_a, b);
        LongNumber new_b = sqrt_long_number(product);
        b = new_b;
    }
    
    print_long_number(a);
}

int main() {
    compute_pi();
    return 0;
}
