#include "Fractions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Function to add two fractions
Fraction_t add_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Determinar el tamaño máximo
    size_t max_size = (frac1->numerator.size > frac2->numerator.size) ? frac1->numerator.size : frac2->numerator.size;
    max_size = (frac1->denominator.size > max_size) ? frac1->denominator.size : max_size;
    max_size = (frac2->denominator.size > max_size) ? frac2->denominator.size : max_size;

    // Calcular el nuevo numerador y denominador
    BigInt_t new_numerator;
    new_numerator.number = calloc(max_size, sizeof(subsize_t));
    new_numerator.size = max_size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(max_size, sizeof(subsize_t));
    new_denominator.size = max_size;

    // Se usan arreglos temporales para almacenar productos parciales
    BigInt_t temp1;
    temp1.number = calloc(max_size, sizeof(subsize_t));
    temp1.size = max_size;

    BigInt_t temp2;
    temp2.number = calloc(max_size, sizeof(subsize_t));
    temp2.size = max_size;

    // Realizar los productos cruzados y sumar
    mult_arr(&(frac1->numerator), &(frac2->denominator), &temp1);
    mult_arr(&(frac2->numerator), &(frac1->denominator), &temp2);
    add_BigInt(&temp1, &temp2, &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    // Crear la fracción resultante
    Fraction_t result = create_fraction(new_numerator, new_denominator);

    // Liberar la memoria temporal
    free(temp1.number);
    free(temp2.number);

    return result;
}

// Function to subtract two fractions
Fraction_t subtract_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Determinar el tamaño máximo
    size_t max_size = (frac1->numerator.size > frac2->numerator.size) ? frac1->numerator.size : frac2->numerator.size;
    max_size = (frac1->denominator.size > max_size) ? frac1->denominator.size : max_size;
    max_size = (frac2->denominator.size > max_size) ? frac2->denominator.size : frac2->denominator.size;

    // Calcular el nuevo numerador y denominador
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

    // Inicializar en cero
    memset(new_numerator.number, 0, sizeof(subsize_t) * max_size);
    memset(new_denominator.number, 0, sizeof(subsize_t) * max_size);
    memset(temp1.number, 0, sizeof(subsize_t) * max_size);
    memset(temp2.number, 0, sizeof(subsize_t) * max_size);

    mult_arr(&(frac1->numerator), &(frac2->denominator), &temp1);
    mult_arr(&(frac2->numerator), &(frac1->denominator), &temp2);
    sub_BigInt(&temp1, &temp2, &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    // Crear la fracción resultante
    Fraction_t result = create_fraction(new_numerator, new_denominator);

    // Liberar la memoria de los arreglos temporales
    free(temp1.number);
    free(temp2.number);
    
    // ¡No liberar new_numerator.number ni new_denominator.number!
    // ya que forman parte de la fracción resultante.

    return result;
}

// Function to multiply two fractions
Fraction_t multiply_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Determinar el tamaño máximo
    size_t max_size = (frac1->numerator.size > frac2->numerator.size) ? frac1->numerator.size : frac2->numerator.size;
    max_size = (frac1->denominator.size > max_size) ? frac1->denominator.size : max_size;
    max_size = (frac2->denominator.size > max_size) ? frac2->denominator.size : frac2->denominator.size;

    // Calcular el nuevo numerador y denominador
    BigInt_t new_numerator;
    new_numerator.number = calloc(max_size, sizeof(subsize_t));
    new_numerator.size = max_size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(max_size, sizeof(subsize_t));
    new_denominator.size = max_size;

    memset(new_numerator.number, 0, sizeof(subsize_t) * max_size);
    memset(new_denominator.number, 0, sizeof(subsize_t) * max_size);

    mult_arr(&(frac1->numerator), &(frac2->numerator), &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->denominator), &new_denominator);

    // Crear la fracción resultante
    Fraction_t result = create_fraction(new_numerator, new_denominator);

    // No liberar new_numerator.number ni new_denominator.number porque la fracción resultante los utiliza

    return result;
}

// Function to divide two fractions
Fraction_t divide_fractions(const Fraction_t* frac1, const Fraction_t* frac2) {
    // Determinar el tamaño máximo
    size_t max_size = (frac1->numerator.size > frac2->numerator.size) ? frac1->numerator.size : frac2->numerator.size;
    max_size = (frac1->denominator.size > max_size) ? frac1->denominator.size : max_size;
    max_size = (frac2->denominator.size > max_size) ? frac2->denominator.size : frac2->denominator.size;

    // Verificar división por cero: se considera división por cero si el numerador del divisor es 0.
    if (is_zero(&(frac2->numerator))) {
        fprintf(stderr, "Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }

    // Calcular el nuevo numerador y denominador
    BigInt_t new_numerator;
    new_numerator.number = calloc(max_size, sizeof(subsize_t));
    new_numerator.size = max_size;

    BigInt_t new_denominator;
    new_denominator.number = calloc(max_size, sizeof(subsize_t));
    new_denominator.size = max_size;

    memset(new_numerator.number, 0, sizeof(subsize_t) * max_size);
    memset(new_denominator.number, 0, sizeof(subsize_t) * max_size);

    mult_arr(&(frac1->numerator), &(frac2->denominator), &new_numerator);
    mult_arr(&(frac1->denominator), &(frac2->numerator), &new_denominator);

    // Crear la fracción resultante
    Fraction_t result = create_fraction(new_numerator, new_denominator);

    // No liberar new_numerator.number ni new_denominator.number porque forman parte del resultado

    return result;
}
