#ifndef FRACTIONS_H
#define FRACTIONS_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "BigInt.h"

// estructura para representar una fraccion
typedef struct Fraction_t {
    BigInt_t numerator;
    BigInt_t denominator;
} Fraction_t;

static inline Fraction_t create_fraction(BigInt_t numerator, BigInt_t denominator) {
    Fraction_t fraction;
    fraction.numerator = numerator;
    fraction.denominator = denominator;
    return fraction;
}

Fraction_t BigIntDecimal_to_Fraction_float(const float_grande* number);
Fraction_t add_fractions(const Fraction_t* frac1, const Fraction_t* frac2);
Fraction_t divide_fractions(const Fraction_t* frac1, const Fraction_t* frac2);
Fraction_t multiply_fractions(const Fraction_t* frac1, const Fraction_t* frac2);
Fraction_t subtract_fractions(const Fraction_t* frac1, const Fraction_t* frac2);
void print_fraction_decimal(const Fraction_t* frac, size_t number_decimals);
#endif