#include <stdio.h>
#include <stdlib.h>

#include "BigInt.h"
#include "Fractions.h"

// Assuming SIZE is defined somewhere (e.g., in BigInt.h)
#ifndef SIZE
#define SIZE 10 // Define a default SIZE if not already defined
#endif

int main() {
    // 1. Create BigInts for numerators and denominators
    BigInt_t num1 = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t den1 = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t num2 = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t den2 = {calloc(SIZE, sizeof(subsize_t)), SIZE};

    // 2. Initialize the BigInts (e.g., 1/2 + 1/3)
    num1.number[0] = 1; // Fraction 1: 1 / 2
    den1.number[0] = 2;
    num2.number[0] = 1; // Fraction 2: 1 / 3
    den2.number[0] = 3;

    // 3. Create Fraction_t instances
    Fraction_t frac1 = create_fraction(num1, den1);
    Fraction_t frac2 = create_fraction(num2, den2);

    // 4. Perform an operation (e.g., addition)
    Fraction_t sum_frac = add_fractions(&frac1, &frac2);

    // 5. Print the result (needs a function to print Fraction_t)
    printf("Fraction 1: ");
    hex_dump_BigInt(&(frac1.numerator));
    printf(" / ");
    hex_dump_BigInt(&(frac1.denominator));
    printf("\n");

    printf("Fraction 2: ");
    hex_dump_BigInt(&(frac2.numerator));
    printf(" / ");
    hex_dump_BigInt(&(frac2.denominator));
    printf("\n");

    printf("Sum: ");
    hex_dump_BigInt(&(sum_frac.numerator));
    printf(" / ");
    hex_dump_BigInt(&(sum_frac.denominator));
    printf(" = ");
    float_grande resultado_div = {
        .number_float = {
            .number = {calloc(SIZE*10, sizeof(subsize_t)), SIZE*10},
            .size = SIZE
        }
    };
    size_t desired_digits = 50; // I want 50 decimal digits

    div_to_float_big(&(sum_frac.numerator), &(sum_frac.denominator), &resultado_div, &desired_digits);
    float__dump_BigInt(&resultado_div);
    printf("\n");

    // 6. Free allocated memory to prevent memory leaks
    free(frac1.numerator.number);
    free(frac1.denominator.number);
    free(frac2.numerator.number);
    free(frac2.denominator.number);
    free(sum_frac.numerator.number);
    free(sum_frac.denominator.number);

    return 0;
}
