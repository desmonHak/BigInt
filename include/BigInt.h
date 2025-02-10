#ifndef BIGINT_H
#define BIGINT_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "debug_c.h"

//#define CONST_SIZE_HEX_INT (1 / log10(16))

// #define CONST_SIZE_HEX_INT 0.82
// static inline len_decimal(int len_hex) {
//     /*
//      * calcular el número de dígitos en decimal que se pueden representar
//      * en función del número de dígitos hexadecimales
//      */
//     return (int)round(len_hex / CONST_SIZE_HEX_INT);
// }
// static inline len_hex(int len_int) {
//     /*
//      * calcular el número de dígitos en hexadecimal que se pueden representar
//      * en función del número de dígitos decimales
//      */
//     return (int)round(len_int * CONST_SIZE_HEX_INT);
// }

#if   UINTPTR_MAX == 0xffffffffffffffff
typedef uint32_t subsize_t;
#define LEN_BITS_SUBSIZE_T 32
#define LEN_BITS_SIZE_T 64
#define UINTPTR_MAX_SUBSIZE_T 0xffffffff
#elif UINTPTR_MAX == 0xffffffff
typedef uint16_t subsize_t;
#define LEN_BITS_SUBSIZE_T 16
#define LEN_BITS_SIZE_T 32
#define UINTPTR_MAX_SUBSIZE_T 0xffff
#elif UINTPTR_MAX == 0xffff
//typedef uint8_t subsize_t;
#endif
#define SUBSIZE_MAX (subsize_t)-1

#define CONST_SIZE_HEX_INT 82
#define len_decimal(len_hex_) ((len_hex_ * 100 + CONST_SIZE_HEX_INT / 2) / CONST_SIZE_HEX_INT)
#define len_hex(len_int_) ((len_int_ * CONST_SIZE_HEX_INT + 50) / 100)

//#define CONST_SIZE_HEX_INT (1 / log10(16))
//#define len_decimal(len_hex_) (len_hex_ * CONST_SIZE_HEX_INT)
//#define len_hex(len_int_) (len_int_ * CONST_SIZE_HEX_INT)
/*
 * usando float:
 * >>> 10 / 0.82
 * 12.195121951219512
 * 
 * (len_hex * 100 + CONST_SIZE_HEX_INT / 2) / CONST_SIZE_HEX_INT
 * usando int
 * >>> 10 * 82
 * 820
 * >>> 10 * 82 / 10
 * 82.0
 * >>> 82 / 2
 * 41.0
 * >>> 10 * 100
 * 1000
 * >>> 1000 + 41
 * 1041
 * >>> 1041 / 82
 * 12.695121951219512
 * >>>
 * 
 * 
 */

typedef struct BigInt_t {
    subsize_t* number;
    size_t size;
} BigInt_t;

typedef struct {
    BigInt_t number_float;  // Mantisa como un número entero grande
    int exponente;            // Exponente como un entero simple
    int signo;                // 0 para positivo, 1 para negativo
} float_grande;

void of_string_to_numbre(const char* num_str, BigInt_t* big_int);

void hex_dump_BigInt(const BigInt_t* big_int);
void decimal_dump_BigInt(const BigInt_t* big_int);

size_t count_hex_digit_BigInt(const BigInt_t* big_int);
void add_BigInt(const BigInt_t* a, const BigInt_t* b, BigInt_t* resultado);
void sub_BigInt(const BigInt_t* a, const BigInt_t* b, BigInt_t* resultado);
void pow_BigInt(BigInt_t *base, BigInt_t *exponente, BigInt_t *resultado);
void modpow_BigInt(
    const BigInt_t* base, const BigInt_t* exponente, 
    const BigInt_t* modulo, BigInt_t* resultado
);

void complemento_a_dos(BigInt_t* big_int);
bool es_cero(const BigInt_t* big_int);

void div_booth(
    BigInt_t* dividend, 
    BigInt_t* divisor, 
    BigInt_t* cociente, 
    BigInt_t* residuo
);

void mult_arr(BigInt_t* a, BigInt_t* b, BigInt_t* resultado);

uint64_t add_with_overflow(subsize_t a, subsize_t b, subsize_t* resultado);

void float__dump_BigInt(float_grande* num);
void normalizar_float_grande(float_grande* num);
void mult_x10(BigInt_t* big_int);
void div_x10(BigInt_t* big_int);
void div_to_float_big(BigInt_t* a, BigInt_t* b, float_grande* resultado, const size_t MAX_ITERACIONES);
#endif