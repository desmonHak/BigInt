#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"
#include "Fractions.h"

#ifndef SIZE
#define SIZE 5 // Tamaño por defecto si no se define en otro lado
#endif




int main() {
    // 1. Crear BigInts para numeradores y denominadores
    BigInt_t num1 = { calloc(SIZE, sizeof(subsize_t)), SIZE };
    BigInt_t den1 = { calloc(SIZE, sizeof(subsize_t)), SIZE };
    BigInt_t num2 = { calloc(SIZE, sizeof(subsize_t)), SIZE };
    BigInt_t den2 = { calloc(SIZE, sizeof(subsize_t)), SIZE };

    // 2. Inicializar los BigInts (ej. 1/2 y 1/3)
    num1.number[0] = 1; // Fracción 1: 1 / 2
    den1.number[0] = 2;
    num2.number[0] = 1; // Fracción 2: 1 / 3
    den2.number[0] = 3;

    // 3. Crear instancias Fraction_t (la fracción "posee" ya los BigInts)
    Fraction_t frac1 = create_fraction(num1, den1);
    Fraction_t frac2 = create_fraction(num2, den2);

    // 4. Realizar operaciones con las fracciones
    Fraction_t sum_frac = add_fractions(&frac1, &frac2);
    Fraction_t sub_frac = subtract_fractions(&frac1, &frac2);
    Fraction_t mul_frac = multiply_fractions(&frac1, &frac2);
    Fraction_t div_frac = divide_fractions(&frac1, &frac2);

    // 5. Imprimir resultados usando formato hexadecimal
    printf("Fraction 1: ");
    decimal_dump_BigInt(&(frac1.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(frac1.denominator));
    printf("\n");

    printf("Fraction 2: ");
    decimal_dump_BigInt(&(frac2.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(frac2.denominator));
    printf("\n");

    printf("Sum: ");
    decimal_dump_BigInt(&(sum_frac.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(sum_frac.denominator));
    printf("\n");

    printf("Subtraction: ");
    decimal_dump_BigInt(&(sub_frac.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(sub_frac.denominator));
    printf("\n");

    printf("Multiplication: ");
    decimal_dump_BigInt(&(mul_frac.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(mul_frac.denominator));
    printf("\n");

    printf("Division: ");
    decimal_dump_BigInt(&(div_frac.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(div_frac.denominator));
    printf(" = ");
    print_fraction_decimal(&div_frac, 32);
    printf("\n");

    // 6. Convertir la suma a "float grande" y mostrar el resultado
    printf("Sum (as float grande): ");
    decimal_dump_BigInt(&(sum_frac.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(sum_frac.denominator));
    printf(" = ");
    BigFloat_t resultado_div = {
        .number_float = {
            .number = calloc(SIZE, sizeof(subsize_t)),
            .size = SIZE
        },
        .exponente = 0,
        .signo = 0
    };
    size_t desired_digits = SIZE*2; // Por ejemplo, se quieren SIZE dígitos decimales
    div_to_float_big(&(sum_frac.numerator), &(sum_frac.denominator), &resultado_div, &desired_digits);
    float__dump_BigInt(&resultado_div);
    printf("\n");


    // Supón que tenemos un BigFloat_t equivalente a 123.45
    #define BIG_SIZE 10

    BigFloat_t num;
    num.number_float.number = calloc(BIG_SIZE, sizeof(subsize_t));
    num.number_float.size = BIG_SIZE;
    of_string_to_numbre("12345", &num.number_float); // 123.45 → 12345 (sin punto)
    num.exponente = -2; // 2 dígitos decimales
    num.signo = 0; // positivo

    Fraction_t f = BigIntDecimal_to_Fraction_float(&num);

    float__dump_BigInt(&num);

    printf("\n");
    decimal_dump_BigInt(&(f.numerator));
    printf(" / ");
    decimal_dump_BigInt(&(f.denominator));
    printf("\n");

    printf("resultado: ");
    print_fraction_decimal(&f,32);

    //  liberar memoria!
    free_BigFloat_inside(&num);
    free_Fraction_t_inside(&f);

    // 7. Liberar la memoria asignada para cada fracción
    // NOTA: Como las fracciones poseen los BigInts (ya sean originales o resultantes),
    // se debe liberar la memoria una sola vez.
    free(frac1.numerator.number);
    free(frac1.denominator.number);
    free(frac2.numerator.number);
    free(frac2.denominator.number);
    free(sum_frac.numerator.number);
    free(sum_frac.denominator.number);
    free(sub_frac.numerator.number);
    free(sub_frac.denominator.number);
    free(mul_frac.numerator.number);
    free(mul_frac.denominator.number);
    free(div_frac.numerator.number);
    free(div_frac.denominator.number);

    free_BigFloat_inside(&resultado_div);




    puts("adios");
    return 0;
}
