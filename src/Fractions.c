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
    // Checa denominador propio
    if (is_zero(&frac1->denominator) || is_zero(&frac2->denominator)) {
        fprintf(stderr, "Error: Division by zero (fraction denominator is zero)\n");
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

Fraction_t BigIntDecimal_to_Fraction_float(const BigFloat_t* number) {
    // 1. Copiar la mantisa (numerador)
    BigInt_t numerator;
    numerator.number = calloc(number->number_float.size, sizeof(subsize_t));
    numerator.size = number->number_float.size;
    memcpy(numerator.number, number->number_float.number, number->number_float.size * sizeof(subsize_t));

    // 2. Calcular el denominador
    BigInt_t denominator;
    denominator.number = calloc(number->number_float.size, sizeof(subsize_t));
    denominator.size = number->number_float.size;

    if (number->exponente < 0) {
        // Denominador = 10^(-exponente)
        int num_decimales = -number->exponente;
        char* denominador_str = calloc(num_decimales + 2, sizeof(char));
        denominador_str[0] = '1';
        memset(denominador_str + 1, '0', num_decimales);
        denominador_str[num_decimales + 1] = '\0';
        of_string_to_numbre(denominador_str, &denominator);
        free(denominador_str);
    } else if (number->exponente == 0) {
        of_string_to_numbre("1", &denominator);
    } else {
        // exponente > 0: Numerador = mantisa * 10^exponente
        char* factor_str = calloc(number->exponente + 2, sizeof(char));
        factor_str[0] = '1';
        memset(factor_str + 1, '0', number->exponente);
        factor_str[number->exponente + 1] = '\0';
        BigInt_t factor;
        factor.number = calloc(number->number_float.size, sizeof(subsize_t));
        factor.size = number->number_float.size;
        of_string_to_numbre(factor_str, &factor);
        mult_arr(&numerator, &factor, &numerator);
        free(factor.number);
        free(factor_str);
        of_string_to_numbre("1", &denominator);
    }



    Fraction_t frac = create_fraction(numerator, denominator);
    return frac;
}

void print_fraction_decimal(const Fraction_t* frac, size_t number_decimals) {
    BigFloat_t resultado_div = {
        .number_float = {
            .number = calloc(number_decimals, sizeof(subsize_t)),
            .size = number_decimals
        },
        .exponente = 0,
        .signo = 0
    };
    size_t desired_digits = number_decimals*2; // Por ejemplo, se quieren SIZE dígitos decimales
    div_to_float_big(&(frac->numerator), &(frac->denominator), &resultado_div, &desired_digits);
    float__dump_BigInt(&resultado_div);
    printf("\n");
}

/**
 * @brief La funcion elimina los elementos internos del tipo fraccion, es decir
 * el denominar y numerador son eliminados, mas no se libera la estructura
 * `Fraction_t*`
 * 
 * @param frac_ un puntero a fraccion. 
 */
void free_Fraction_t_inside(Fraction_t *frac) {
    if (frac == NULL) return;

    if (frac->numerator.number != NULL) {
        free(frac->numerator.number);
        frac->numerator.number = NULL;
    }

    if (frac->denominator.number != NULL) {
        free(frac->denominator.number);
        frac->denominator.number = NULL;
    }
}


/**
 * @brief La funcion libera la estructura completa si `Fraction_t*` fue 
 * reservada usando memoria dinamica, y libera sus componentes internos.
 * 
 * @param frac_ un puntero a puntero a fraccion. 
 */
void free_Fraction_t(Fraction_t **frac_) {
    if (frac_ == NULL || *frac_ == NULL) return;

    free_Fraction_t_inside(*frac_);
    free(*frac_);
    *frac_ = NULL;
}


// Calcula n! como BigInt
void factorial_BigInt(int n, BigInt_t* result, size_t size_big) {
    of_string_to_numbre("1", result);
    BigInt_t temp = {calloc(size_big, sizeof(subsize_t)), size_big};
    for (int i = 2; i <= n; ++i) {
        char str[32];
        sprintf(str, "%d", i);
        of_string_to_numbre(str, &temp);
        mult_arr(result, &temp, result);
    }
    free(temp.number);
}

// Calcula x^n como fracción (x es fracción)
Fraction_t pow_fraction(const Fraction_t* x, int n, size_t size_big) {
    Fraction_t res;
    // res = 1/1
    res.numerator.number = calloc(size_big, sizeof(subsize_t));
    res.numerator.size = size_big;
    res.denominator.number = calloc(size_big, sizeof(subsize_t));
    res.denominator.size = size_big;
    of_string_to_numbre("1", &res.numerator);
    of_string_to_numbre("1", &res.denominator);

    for (int i = 0; i < n; ++i) {
        if (n == 0) {
            Fraction_t res;
            res.numerator.number = calloc(size_big, sizeof(subsize_t));
            res.numerator.size = size_big;
            res.denominator.number = calloc(size_big, sizeof(subsize_t));
            res.denominator.size = size_big;
            of_string_to_numbre("1", &res.numerator);
            of_string_to_numbre("1", &res.denominator);
            return res;
        }

        Fraction_t temp = multiply_fractions(&res, x);
        free_Fraction_t_inside(&res);
        res = temp;
    }
    return res;
}



