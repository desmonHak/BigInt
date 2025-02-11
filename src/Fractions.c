#include "Fractions.h"

#include "Fractions.h"

// Function to add two fractions
Fraction_t add_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Determine the maximum size
    size_t max_size = (frac1->numerator.size > frac2->numerator.size) ? frac1->numerator.size : frac2->numerator.size;
    max_size = (frac1->denominator.size > max_size) ? frac1->denominator.size : max_size;
    max_size = (frac2->denominator.size > max_size) ? frac2->denominator.size : max_size;

    // Calculate the new numerator and denominator
    BigInt_t new_numerator;
    new_numerator.number = calloc(max_size, sizeof(subsize_t));
    new_numerator.size = max_size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(max_size, sizeof(subsize_t));
    new_denominator.size = max_size;

    BigInt_t temp1;
    temp1.number = calloc(max_size, sizeof(subsize_t));
    temp1.size = max_size;

    BigInt_t temp2;
    temp2.number = calloc(max_size, sizeof(subsize_t));
    temp2.size = max_size;

    mult_arr(&(frac1->numerator), &(frac2->denominator), &temp1);
    mult_arr(&(frac2->numerator), &(frac1->denominator), &temp2);
    add_BigInt(&temp1, &temp2, &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    // Create the new fraction
    Fraction_t result = create_fraction(new_numerator, new_denominator);

    // Free temporary memory
    free(temp1.number);
    free(temp2.number);

    return result;
}


// Function to subtract two fractions
Fraction_t subtract_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Calculate the new numerator and denominator
    BigInt_t new_numerator;
    new_numerator.number = calloc(frac1->numerator.size + frac2->numerator.size, sizeof(subsize_t));
    new_numerator.size = frac1->numerator.size + frac2->numerator.size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(frac1->denominator.size + frac2->denominator.size, sizeof(subsize_t));
    new_denominator.size = frac1->denominator.size + frac2->denominator.size;

    BigInt_t temp1;
    temp1.number = calloc(frac1->numerator.size + frac2->denominator.size, sizeof(subsize_t));
    temp1.size = frac1->numerator.size + frac2->denominator.size;

    BigInt_t temp2;
    temp2.number = calloc(frac2->numerator.size + frac1->denominator.size, sizeof(subsize_t));
    temp2.size = frac2->numerator.size + frac1->denominator.size;

    mult_arr(&(frac1->numerator), &(frac2->denominator), &temp1);
    mult_arr(&(frac2->numerator), &(frac1->denominator), &temp2);
    sub_BigInt(&temp1, &temp2, &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    free(temp1.number);
    free(temp2.number);

    // Create the new fraction
    Fraction_t result = create_fraction(new_numerator, new_denominator);
    return result;
}

// Function to multiply two fractions
Fraction_t multiply_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Calculate the new numerator and denominator
    BigInt_t new_numerator;
    new_numerator.number = calloc(frac1->numerator.size + frac2->numerator.size, sizeof(subsize_t));
    new_numerator.size = frac1->numerator.size + frac2->numerator.size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(frac1->denominator.size + frac2->denominator.size, sizeof(subsize_t));
    new_denominator.size = frac1->denominator.size + frac2->denominator.size;

    mult_arr(&(frac1->numerator), &(frac2->numerator), &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    // Create the new fraction
    Fraction_t result = create_fraction(new_numerator, new_denominator);
    return result;
}

// Function to divide two fractions
Fraction_t divide_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Check for division by zero
    if (is_zero(&(frac2->numerator))) {
        fprintf(stderr, "Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }

    // Calculate the new numerator and denominator
   BigInt_t new_numerator;
    new_numerator.number = calloc(frac1->numerator.size + frac2->numerator.size, sizeof(subsize_t));
    new_numerator.size = frac1->numerator.size + frac2->numerator.size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(frac1->denominator.size + frac2->denominator.size, sizeof(subsize_t));
    new_denominator.size = frac1->denominator.size + frac2->denominator.size;

    mult_arr(&(frac1->numerator), &(frac2->denominator), &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->numerator), &new_denominator);

    // Create the new fraction
    Fraction_t result = create_fraction(new_numerator, new_denominator);
    return result;
}
