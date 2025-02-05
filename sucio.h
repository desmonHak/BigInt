#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define SIZE 10  // Tamaño del array (para representar números de hasta 64 bits)

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

typedef struct {
    subsize_t mantisa[SIZE];  // Mantisa como un número entero grande
    int exponente;            // Exponente como un entero simple
    int signo;                // 0 para positivo, 1 para negativo
} float_grande;

// Funciones auxiliares (ya proporcionadas)
size_t longitud_hex(subsize_t* num, size_t size);
void suma_sin_suma(subsize_t* a, subsize_t* b, subsize_t* resultado, int size);
void resta_sin_resta(subsize_t* a, subsize_t* b, subsize_t* resultado, int size);
void multiplicar_arreglos(subsize_t* a, subsize_t* b, subsize_t* resultado, int size);
void division_booth(subsize_t* dividend, subsize_t* divisor, subsize_t* cociente, subsize_t* residuo, int size);
void imprimir_decimal(subsize_t* num, int size);
void multiplicar_por_10(subsize_t* num, int size);
void dividir_por_10(subsize_t* num, int size);
bool es_cero(subsize_t* num, int size);
void string_a_array(const char* num_str, subsize_t* arr, int size);
void desplazar_izquierda(subsize_t* arr, int size, int shift);
void desplazar_derecha(subsize_t* arr, int size, int shift);
int comparar_arrays(subsize_t* arr1, subsize_t* arr2, int size);
bool es_mayor(subsize_t* a, subsize_t* b, int size);
bool es_menor(subsize_t* a, subsize_t* b);
int es_mayor_o_igual(subsize_t* arr1, subsize_t* arr2, int size);
void resta_arreglos(subsize_t* a, subsize_t* b, subsize_t* resultado, int size);

// Nuevas funciones para float_grande
void normalizar_float_grande(float_grande* num);
void inicializar_float_grande(float_grande* num, const char* valor_str);
void sumar_float_grande(float_grande* a, float_grande* b, float_grande* resultado);
void restar_float_grande(float_grande* a, float_grande* b, float_grande* resultado);
void multiplicar_float_grande(float_grande* a, float_grande* b, float_grande* resultado);
void dividir_float_grande(float_grande* a, float_grande* b, float_grande* resultado);
void imprimir_float_grande(float_grande* num);