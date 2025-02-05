#ifndef BIGINT_H
#define BIGINT_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


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

//#if   UINTPTR_MAX == 0xffffffffffffffff
typedef uint32_t subsize_t;
//#elif UINTPTR_MAX == 0xffffffff
//typedef uint16_t subsize_t;
//#elif UINTPTR_MAX == 0xffff
//typedef uint8_t subsize_t;
//#endif
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


#endif