#include "BigInt.h"


uint64_t add_with_overflow(subsize_t a, subsize_t b, subsize_t* resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(uint64_t, add_with_overflow)
            TYPE_DATA_DBG(subsize_t, "a = %p")
            TYPE_DATA_DBG(subsize_t, "b = %p")
            TYPE_DATA_DBG(subsize_t*, "resultado = %p")
        END_TYPE_FUNC_DBG,
        a, b, resultado);
    subsize_t carry;
    
    while (b != 0) {
        carry = (a & b);  // Calcular el acarreo
        a = a ^ b;        // Suma sin acarreo
        b = carry << 1;   // Desplazar el acarreo
    }

    *resultado = a;  // Guardamos el resultado en 32 bits

    // Si el acarreo generó un bit extra (desbordamiento), lo manejamos con un uint64_t
    if (carry != 0) {
        return ((uint64_t)1 << 32) | a;  // Agregamos el bit extra al resultado final
    }

    return a;  // Retornamos el resultado de 32 bits si no hay desbordamiento
}

void of_string_to_numbre(const char* num_str, BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, of_string_to_numbre)
            TYPE_DATA_DBG(const char*, "num_str = %p")
            TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
        END_TYPE_FUNC_DBG,
        num_str, big_int);
    if (big_int == NULL) {
        printf("Error: BigInt_t is NULL.\n");
        return;
    }
    if (big_int->number == NULL) {
        printf("Error: Array de salida es NULL.\n");
        return;
    }

    if (num_str == NULL) {
        printf("Error: Cadena de entrada es NULL.\n");
        return;
    }

    if (big_int->size == 0) {
        printf("Error: Tamaño del arreglo de salida es 0.\n");
        return;
    }
    size_t len = strlen(num_str);
    
    memset(big_int->number, 0, sizeof(subsize_t) * big_int->size);

        if (len >= 2 && num_str[0] == '0' && (num_str[1] == 'x' || num_str[1] == 'X')) {
        // Convertir desde hexadecimal
        int arr_index = 0;
        subsize_t current_value = 0;
        int shift = 0;

        for (int i = len - 1; i >= 2 && arr_index < big_int->size; i--) {
            char c = num_str[i];
            int digit;

            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                digit = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                digit = c - 'a' + 10;
            } else {
                continue;  // Ignorar caracteres no válidos
            }

            current_value |= ((subsize_t)digit << shift);
            shift += 4;

            if (shift == 32) {
                big_int->number[arr_index++] = current_value;
                current_value = 0;
                shift = 0;
            }
        }

        if (shift > 0 && arr_index < big_int->size) {
            big_int->number[arr_index] = current_value;
        }
    } else {
        // Convertir desde decimal (sin cambios)
        for (int i = 0; i < len; i++) {
            subsize_t digit = num_str[i] - '0';
            if (digit > 9) continue; // Ignorar caracteres no numéricos

            subsize_t carry = 0;
            for (int j = 0; j < big_int->size; j++) {
                uint64_t product = (uint64_t)big_int->number[j] * 10 + carry;
                big_int->number[j] = (subsize_t)(product & UINTPTR_MAX_SUBSIZE_T);
                carry = (subsize_t)(product >> 32);
            }

            carry = digit;
            for (int j = 0; j < big_int->size && carry; j++) {
                uint64_t sum = (uint64_t)big_int->number[j] + carry;
                big_int->number[j] = (subsize_t)(sum & UINTPTR_MAX_SUBSIZE_T);
                carry = (subsize_t)(sum >> 32);
            }
        }
    }
}

/**
 *
 * @param big_int Numero BigInt a imprimir en formato hexadecimal
 */
void hex_dump_BigInt(const BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, hex_dump_BigInt)
            TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
        END_TYPE_FUNC_DBG,
        big_int);
    for (int i = big_int->size - 1; i >= 0; i--) {
        printf("%08X", big_int->number[i]);
    }
}

/**
 *
 * @param big_int Numero BigInt a imprimir en formato decimal
 */
void decimal_dump_BigInt(const BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, decimal_dump_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
    END_TYPE_FUNC_DBG,
    big_int);
    if (big_int == NULL) {
        printf("Error: big_int es NULL.\n");
        return;
    }
    if (big_int->number == NULL) {
        printf("Error: Array de entrada es NULL.\n");
        return;
    }

    /*
     * Digitos: 3
     * hex(int('1'*3))[2:] => 6f
     * tamaño: 2
     * math.ceil(( math.log10(10**3) / math.log10(16))) => 2
     * 
     * Digitos: 4
     * hex(int('1'*4))[2:] => 457
     * tamaño: 3
     * math.ceil(( math.log10(10**4) / math.log10(16))) => 3
     * 
     * Digitos: 5
     * hex(int('1'*5))[2:] => 2b67
     * tamaño: 4
     * math.ceil(( math.log10(10**5) / math.log10(16))) => 4
     * 
     * Digitos: 6
     * hex(int('1'*6))[2:] => 1b207
     * tamaño: 5
     * math.ceil(( math.log10(10**6) / math.log10(16))) => 5
     * 
     * Digitos: 7
     * hex(int('1'*7))[2:] => 10f447
     * 
     * >>> import math
     * >>> math.ceil(( math.log10(10**1) / math.log10(16)))
     * 1
     * >>> math.1 / math.log10(16)
     * 0.8304820237218405
     * 
     */
    int longitud_valor_decimal = len_decimal(count_hex_digit_BigInt(big_int)); // 0.82 * longitud del número hex = aprox longitud num decimal
    char *decimal = calloc(longitud_valor_decimal + 1, sizeof(char));
    memset(decimal, '0', longitud_valor_decimal);
    decimal[longitud_valor_decimal] = '\0';
    char *temp = calloc(longitud_valor_decimal + 1, sizeof(char));
    
    for (int i = big_int->size - 1; i >= 0; i--) {
        subsize_t n = big_int->number[i];
        for (int bit = (LEN_BITS_SUBSIZE_T -1); bit >= 0; bit--) {
            int carry = 0;
            for (int j = strlen(decimal) - 1; j >= 0; j--) {
                int digit = (decimal[j] - '0') * 2 + carry;
                decimal[j] = (digit % 10) + '0';
                carry = digit / 10;
            }
            if (carry) {
                memmove(decimal + 1, decimal, strlen(decimal) + 1);
                decimal[0] = carry + '0';
            }
            
            if (n & (1U << bit)) {
                carry = 1;
                for (int j = strlen(decimal) - 1; j >= 0 && carry; j--) {
                    int digit = (decimal[j] - '0') + carry;
                    decimal[j] = (digit % 10) + '0';
                    carry = digit / 10;
                }
                if (carry) {
                    memmove(decimal + 1, decimal, strlen(decimal) + 1);
                    decimal[0] = '1';
                }
            }
        }
    }
    printf("%s", decimal);
    free(decimal);
    free(temp);

}

size_t count_hex_digit_BigInt(const BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(size_t, count_hex_digit_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
    END_TYPE_FUNC_DBG,
    big_int);
    if (big_int->size == 0) {
        printf("Error: Tamaño del arreglo es 0.\n");
        return 0;
    }
    if (big_int->number == NULL) {
        printf("Error: num == NULL.\n");
        return 0;
    }
    size_t longitud = 0;
    ssize_t i;

    // Encuentra el primer elemento no nulo desde la izquierda
    for (i = big_int->size; i > 0; i--) {
        if (big_int->number[i-1] != 0) {
            longitud = i;
            break;
        }
    }

    // Si todos los elementos son cero, la longitud es 1
    if (longitud == 0) {
        return 1;
    }

    // Cuenta los dígitos hexadecimales significativos en el elemento más significativo
    subsize_t msb = big_int->number[longitud - 1];
    size_t digitos = 8; // Máximo 8 dígitos hexadecimales en un subsize_t
    while (msb != 0) {
        msb >>= 4;
        digitos--;
    }

    return (longitud - 1) * 8 + (8 - digitos);
}

void add_BigInt(const BigInt_t* a, const BigInt_t* b, BigInt_t* resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, add_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "a = %p")
        TYPE_DATA_DBG(BigInt_t*, "b = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    a, b, resultado);

    if (a == NULL || a->number == NULL || b == NULL || b->number == NULL || resultado == NULL || resultado->number == NULL) {
        printf("Error: One or more BigInt_t pointers are NULL.\n");
        return;
    }

    if (a->size != b->size || a->size != resultado->size) {
        printf("Error: BigInt_t sizes are not equal.\n");
        return;
    }
    uint64_t  carry = 0;

    // Sumamos bloque por bloque de 32 bits
    for (int i = 0; i < a->size; i++) {
        uint64_t  temp_resultado = (uint64_t )a->number[i] + b->number[i] + carry;
        resultado->number[i] = (subsize_t)temp_resultado;
        carry = (temp_resultado >> LEN_BITS_SUBSIZE_T);
    }

    // Si al final hay un acarreo y hay espacio en el array, almacenarlo
    if (carry && resultado->size > a->size) {
        // Verificamos si hay espacio para almacenar el acarreo
        // (asumimos que el usuario pasa `resultado` con `size + 1` espacio si es necesario)
        resultado->number[a->size] = (subsize_t)carry;
    }


}

void sub_BigInt(const BigInt_t* a, const BigInt_t* b, BigInt_t* resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, sub_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "a = %p")
        TYPE_DATA_DBG(BigInt_t*, "b = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    a, b, resultado);

    if (a == NULL || a->number == NULL || b == NULL || b->number == NULL || resultado == NULL || resultado->number == NULL) {
        printf("Error: One or more BigInt_t pointers are NULL.\n");
        return;
    }

    if (a->size != b->size || a->size != resultado->size) {
        printf("Error: BigInt_t sizes are not equal.\n");
        return;
    }
    subsize_t *b_complemento = calloc(b->size, sizeof(subsize_t));
    memcpy(b_complemento, b->number, sizeof(subsize_t) * b->size);
    
    complemento_a_dos(&(BigInt_t){b_complemento, b->size});
    add_BigInt(a, &(BigInt_t){b_complemento, b->size}, resultado);
    free(b_complemento);
}

void complemento_a_dos(BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, complemento_a_dos)
        TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
    END_TYPE_FUNC_DBG,
    big_int);

    if (big_int == NULL || big_int->number == NULL) {
        printf("Error: BigInt_t pointer is NULL.\n");
        return;
    }
    for (int i = 0; i < big_int->size; i++) {
        big_int->number[i] = ~big_int->number[i];
    }
    
    subsize_t carry = 1;
    for (int i = 0; i < big_int->size && carry; i++) {
        size_t sum = (size_t)big_int->number[i] + carry;
        big_int->number[i] = (subsize_t)sum;
        carry = (subsize_t)(sum >> 32);
    }
}


// exponenciacion_modular
void modpow_BigInt(
    const BigInt_t* base, const BigInt_t* exponente, 
    const BigInt_t* modulo, BigInt_t* resultado
    ) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, modpow_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "base = %p")
        TYPE_DATA_DBG(BigInt_t*, "exponente = %p")
        TYPE_DATA_DBG(BigInt_t*, "modulo = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    base, exponente, modulo, resultado);

    if (!base || !base->number || !exponente || !exponente->number || !modulo || !modulo->number || !resultado || !resultado->number) {
        printf("Error: One or more BigInt_t pointers are NULL.\n");
        return;
    }

    if (base->size != modulo->size || base->size != resultado->size || base->size != exponente->size) {
        printf("Error: BigInt_t sizes are not equal.\n");
        return;
    }
    // https://es.wikipedia.org/wiki/Exponenciación_modular
    // necesario para almacenar el resultado de la multiplicación
    BigInt_t temp_result_bigint = {calloc(2 * base->size, sizeof(subsize_t)), 2 * base->size};
    subsize_t *temp_result= temp_result_bigint.number;
    temp_result[0] = 1;  // Inicializamos el resultado como 1

    // necesario para almacenar el resultado de la multiplicación
    BigInt_t base_temp_bigint = {calloc(2 * base->size, sizeof(subsize_t)), 2 * base->size};
    subsize_t *base_temp = base_temp_bigint.number;
    memcpy(base_temp, base->number, sizeof(subsize_t) * base->size);

    BigInt_t exponente_temp_bigint = {calloc(base->size, sizeof(subsize_t)), base->size};
    subsize_t *exponente_temp = exponente_temp_bigint.number;
    memcpy(exponente_temp, exponente->number, sizeof(subsize_t) * base->size);

    BigInt_t cociente_bigint = {calloc(base->size, sizeof(subsize_t)), base->size};
    subsize_t *cociente = cociente_bigint.number;  // Para almacenar el cociente (que no usaremos)

    while (!is_zero(&(BigInt_t){exponente_temp, base->size})) {
        // Si el exponente es impar, multiplicamos el resultado por la base
        if (exponente_temp[0] & 1) {
            mult_arr(&(BigInt_t){temp_result, base->size}, &(BigInt_t){base_temp, base->size}, &(BigInt_t){temp_result, base->size});
            // Aplicamos el módulo
            div_booth(&(BigInt_t){temp_result, base->size}, modulo, &(BigInt_t){cociente, base->size}, &(BigInt_t){temp_result, base->size});  
        }

        // Elevamos la base al cuadrado (base = base * base)
        mult_arr(&(BigInt_t){base_temp, base->size}, &(BigInt_t){base_temp, base->size}, &(BigInt_t){base_temp, base->size});
        div_booth(&(BigInt_t){base_temp, base->size}, modulo,  &(BigInt_t){cociente, base->size}, &(BigInt_t){base_temp, base->size});  // Aplicamos el módulo

        // Dividimos el exponente entre 2 (desplazamiento a la derecha)
        div_x2(&(BigInt_t){exponente_temp, base->size});
    }

    // Al final, el resultado está en temp_result
    memcpy(resultado->number, temp_result, sizeof(subsize_t) * base->size);
    free(temp_result);
    free(base_temp);
    free(exponente_temp);
    free(cociente);
}

void div_booth(
    BigInt_t* dividend, 
    BigInt_t* divisor, 
    BigInt_t* cociente, 
    BigInt_t* residuo
    ) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, div_booth)
        TYPE_DATA_DBG(BigInt_t*, "dividend = %p")
        TYPE_DATA_DBG(BigInt_t*, "divisor = %p")
        TYPE_DATA_DBG(BigInt_t*, "cociente = %p")
        TYPE_DATA_DBG(BigInt_t*, "residuo = %p")
    END_TYPE_FUNC_DBG,
    dividend, divisor, cociente, residuo);

        if (!dividend || !dividend->number || !divisor || !divisor->number || !cociente || !cociente->number || !residuo || !residuo->number) {
        printf("Error: One or more BigInt_t pointers are NULL.\n");
        return;
    }

    if (dividend->size != divisor->size || dividend->size != cociente->size || dividend->size != residuo->size) {
        printf("Error: BigInt_t sizes are not equal.\n");
        return;
    }
    BigInt_t Q_bigint = {calloc(dividend->size, sizeof(subsize_t)), dividend->size};       // Cociente (inicializado con el dividendo)
    subsize_t *Q= Q_bigint.number;
    subsize_t Q_1 = 0;        // Bit extra Q-1
    BigInt_t M_bigint = {calloc(divisor->size, sizeof(subsize_t)), divisor->size};        // Divisor
    subsize_t *M= M_bigint.number;
    BigInt_t A_bigint = {calloc(dividend->size, sizeof(subsize_t)), dividend->size};  // Acumulador (inicialmente 0)
    subsize_t *A = A_bigint.number;

    // Copiar el dividendo en Q y el divisor en M
    memcpy(Q, dividend->number, sizeof(subsize_t) * dividend->size);
    memcpy(M, divisor->number, sizeof(subsize_t) * divisor->size);
    // Proceso de división (Booth)
    for (int i = 0; i < dividend->size * 32; i++) {
        // Desplazamiento a la izquierda de A:Q:Q_1
        int msb_A = (A[dividend->size-1] & 0x80000000) ? 1 : 0;
        for (int j = dividend->size - 1; j > 0; j--) {
            A[j] = (A[j] << 1) | (A[j-1] >> 31);
        }
        A[0] = (A[0] << 1) | (Q[dividend->size-1] >> 31);
        for (int j = dividend->size - 1; j > 0; j--) {
            Q[j] = (Q[j] << 1) | (Q[j-1] >> 31);
        }
        Q[0] = (Q[0] << 1) | Q_1;
        Q_1 = msb_A;
        BigInt_t temp_bigint = {calloc(dividend->size, sizeof(subsize_t)), dividend->size};
        subsize_t *temp = temp_bigint.number;
        // Realizar A = A - M o A = A + M según el bit más significativo de A
        if (msb_A) {
            add_BigInt(&(BigInt_t){A, dividend->size}, &(BigInt_t){M, divisor->size}, &(BigInt_t){temp, dividend->size});
            memcpy(A, temp, sizeof(subsize_t) * dividend->size);
            Q[0] |= 1;  // Set LSB of Q to 1
        } else {
            sub_BigInt(&(BigInt_t){A, dividend->size}, &(BigInt_t){M, divisor->size}, &(BigInt_t){temp, dividend->size});
            memcpy(A, temp, sizeof(subsize_t) * dividend->size);
            if (A[dividend->size-1] & 0x80000000) {
                add_BigInt(&(BigInt_t){A, dividend->size}, &(BigInt_t){M, divisor->size},  &(BigInt_t){temp, dividend->size});
                memcpy(A, temp, sizeof(subsize_t) * dividend->size);
            } else {
                Q[0] |= 1;  // Set LSB of Q to 1
            }
        }
        free(temp);
    }

    // Copiar el cociente y el residuo a los arreglos de salida
    memcpy(cociente->number, Q, sizeof(subsize_t) * dividend->size);
    memcpy(residuo->number, A, sizeof(subsize_t) * dividend->size);
    free(Q);
    free(M);
    free(A);
}

// Función para verificar si un número es cero
bool is_zero(const BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, is_zero)
        TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
    END_TYPE_FUNC_DBG,
    big_int);

    if (big_int == NULL || big_int->number == NULL) {
        printf("Error: BigInt_t pointer is NULL.\n");
        return true;
    }
    for (int i = 0; i < big_int->size; i++) {
        if (big_int->number[i] != 0) return false;
    }
    return true;
}

// División por 2 del número representado como arreglo de enteros
void div_x2(BigInt_t* big_int) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, div_x2)
        TYPE_DATA_DBG(BigInt_t*, "big_int = %p")
    END_TYPE_FUNC_DBG,
    big_int);

    if (big_int == NULL || big_int->number == NULL) {
        printf("Error: BigInt_t pointer is NULL.\n");
        return;
    }
    unsigned long long carry = 0;
    for (int i = big_int->size - 1; i >= 0; i--) {
        unsigned long long val = (unsigned long long)big_int->number[i] + (carry << 32);
        big_int->number[i] = (subsize_t)(val >> 1);
        carry = val & 1;
    }
}


// Función para multiplicar dos arreglos (multiplicación de enteros grandes)
void mult_arr(BigInt_t* a, BigInt_t* b, BigInt_t* resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, mult_arr)
        TYPE_DATA_DBG(BigInt_t*, "a = %p")
        TYPE_DATA_DBG(BigInt_t*, "b = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    a, b, resultado);

    if (!a || !a->number || !b || !b->number || !resultado || !resultado->number) {
        printf("Error: One or more BigInt_t pointers are NULL.\n");
        return;
    }

     if (a->size != b->size || a->size != resultado->size) {
        printf("Error: BigInt_t sizes are not equal.\n");
        return;
    }

    size_t size = a->size;
    size_t res_size = resultado->size;
    memset(resultado->number, 0, sizeof(subsize_t) * res_size);

    for (size_t i = 0; i < size; i++) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < size; j++) {
            if (i + j >= res_size) continue; // Evita overflow
            unsigned long long product = (unsigned long long)a->number[i] * b->number[j] + resultado->number[i + j] + carry;
            resultado->number[i + j] = (subsize_t)(product & UINTPTR_MAX_SUBSIZE_T);
            carry = product >> LEN_BITS_SUBSIZE_T;
        }
        if (i + size < res_size)
            resultado->number[i + size] = (subsize_t)carry;
    }

}

// Calcula la raíz cuadrada entera de n usando Newton-Raphson.
void sqrt_BigInt(const BigInt_t* n, BigInt_t* raiz) {
    size_t size = n->size;
    // Inicializa x = n / 2 (aproximación inicial)
    BigInt_t x = {calloc(size, sizeof(subsize_t)), size};
    BigInt_t x_prev = {calloc(size, sizeof(subsize_t)), size};
    BigInt_t temp = {calloc(size * 2, sizeof(subsize_t)), size };
    BigInt_t two = {calloc(size, sizeof(subsize_t)), size};

    // two = 2
    memset(two.number, 0, sizeof(subsize_t) * size);
    two.number[0] = 2;

    // x = n / 2
    memcpy(x.number, n->number, sizeof(subsize_t) * size);
    div_x2(&x);

    // Si n < 2, la raíz es n
    if (is_zero(n) || (n->number[0] == 1 && size == 1)) {
        memcpy(raiz->number, n->number, sizeof(subsize_t) * size);
        free(x.number); free(x_prev.number); free(temp.number); free(two.number);
        return;
    }

    // Bucle de Newton-Raphson
    while (1) {
        // x_prev = x
        memcpy(x_prev.number, x.number, sizeof(subsize_t) * size);

        // temp = n / x
        memset(temp.number, 0, sizeof(subsize_t) * size * 2);
        BigInt_t cociente = {calloc(size, sizeof(subsize_t)), size};
        BigInt_t residuo = {calloc(size, sizeof(subsize_t)), size};
        div_booth((BigInt_t*)n, &x, &cociente, &residuo);

        // temp = x + (n / x)
        add_BigInt(&x, &cociente, &temp);

        // x = temp / 2
        memcpy(x.number, temp.number, sizeof(subsize_t) * size);
        div_x2(&x);

        free(cociente.number);
        free(residuo.number);

        // Si x == x_prev (sin cambios), terminamos
        if (cmp_BigInt(&x, &x_prev) == 0) {
            break;
        }
    }

    // Copia el resultado
    memcpy(raiz->number, x.number, sizeof(subsize_t) * size);

    free(x.number);
    free(x_prev.number);
    free(temp.number);
    free(two.number);
}



void sqrt_BigFloat(const BigFloat_t* n, BigFloat_t* raiz, size_t decimales) {
#warning "por implementar"
}



void float__dump_BigInt(BigFloat_t* num) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, float__dump_BigInt)
        END_TYPE_FUNC_DBG,
        num);
    if (num->signo) printf("-");

    size_t SIZE = num->number_float.size; // Get the size from the BigInt_t
    int longitud_valor_decimal = len_decimal(count_hex_digit_BigInt(&(num->number_float))); // Pass the BigInt_t
    char *decimal = calloc(longitud_valor_decimal + 1, sizeof(char));
    memset(decimal, '0', longitud_valor_decimal);
    decimal[longitud_valor_decimal] = '\0';

    for (int i = SIZE - 1; i >= 0; i--) {
        subsize_t n = num->number_float.number[i]; // Access the number array through BigInt_t
        for (int bit = 31; bit >= 0; bit--) {
            int carry = 0;
            for (int j = longitud_valor_decimal - 1; j >= 0; j--) {
                int digit = (decimal[j] - '0') * 2 + carry;
                decimal[j] = (digit % 10) + '0';
                carry = digit / 10;
            }
            if (carry) {
                memmove(decimal + 1, decimal, longitud_valor_decimal);
                decimal[0] = carry + '0';
            }

            if (n & (1U << bit)) {
                carry = 1;
                for (int j = longitud_valor_decimal - 1; j >= 0 && carry; j--) {
                    int digit = (decimal[j] - '0') + carry;
                    decimal[j] = (digit % 10) + '0';
                    carry = digit / 10;
                }
                if (carry) {
                    memmove(decimal + 1, decimal, longitud_valor_decimal);
                    decimal[0] = '1';
                }
            }
        }
    }

    // Imprimir el número con el punto decimal en la posición correcta
    int punto_decimal = longitud_valor_decimal + num->exponente;
    if (punto_decimal <= 0) {
        printf("0.");
        for (int i = 0; i < -punto_decimal; i++) {
            printf("0");
        }
        printf("%s", decimal);
    } else if (punto_decimal >= longitud_valor_decimal) {
        printf("%s", decimal);
        for (int i = 0; i < punto_decimal - longitud_valor_decimal; i++) {
            printf("0");
        }
    } else {
        for (int i = 0; i < punto_decimal; i++) {
            printf("%c", decimal[i]);
        }
        printf(".");
        for (int i = punto_decimal; i < longitud_valor_decimal; i++) {
            printf("%c", decimal[i]);
        }
    }
    free(decimal);
}


void div_to_float_big(BigInt_t* a, BigInt_t* b, BigFloat_t* resultado, size_t* num_digits) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, mult_arr)
        TYPE_DATA_DBG(BigInt_t*, "a = %p")
        TYPE_DATA_DBG(BigInt_t*, "b = %p")
        TYPE_DATA_DBG(BigFloat_t*, "resultado = %p")
        TYPE_DATA_DBG(size_t*, "num_digits = %zu")
    END_TYPE_FUNC_DBG,
    a, b, resultado, *num_digits);

    if (!a || !a->number || !b || !b->number || a->size != b->size) {
        printf("Error: Division por cero o argumentos invalidos.\n");
        return;
    }

    // Comprobar si el divisor es cero
    if (is_zero(b)) {
        printf("Error: División por cero\n");
        return;
    }

    //Aloca la memoria para cociente y residuo
    BigInt_t cociente = {calloc(a->size, sizeof(subsize_t)), a->size};
    BigInt_t residuo = {calloc(a->size, sizeof(subsize_t)), a->size};

    // Llamamos a la función de división
    div_booth(a, b, &cociente, &residuo);

    memset(resultado, 0, sizeof(BigFloat_t));
    resultado->number_float = cociente;

    BigInt_t cociente_temp = {calloc(a->size, sizeof(subsize_t)), a->size};
    BigInt_t residuo_temp = {calloc(a->size, sizeof(subsize_t)), a->size};
    size_t digits_obtained = 0;
    size_t number_max_digits = *num_digits;

    // Si el resto no es 0, seguir dividiendo
    if (!is_zero(&residuo) ) {
        while (!is_zero(&residuo) && digits_obtained < number_max_digits) {
            digits_obtained++;
            mult_x10(&residuo);
            div_booth(&residuo, b, &cociente_temp, &residuo_temp);
            mult_x10(&(resultado->number_float));
            add_BigInt(&(resultado->number_float), &cociente_temp, &(resultado->number_float));
            memcpy(residuo.number, residuo_temp.number, sizeof(subsize_t) * a->size);
            resultado->exponente--;
        }
        // Normalizar el resultado
        normalizar_BigFloat(resultado);
    }
    *num_digits = digits_obtained;
    free(residuo_temp.number);
    free(cociente_temp.number);
    free(residuo.number);
}


// Implementación de las nuevas funciones
void normalizar_BigFloat(BigFloat_t* num) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, normalizar_BigFloat)
        TYPE_DATA_DBG(BigFloat_t*, "num = %p")
    END_TYPE_FUNC_DBG,
    num);
    size_t SIZE = num->number_float.size;
    // Contar ceros a la izquierda de la mantisa
    int ceros_izquierda = 0;
    while (ceros_izquierda < SIZE && num->number_float.number[ceros_izquierda] == 0) {
        ceros_izquierda++;
    }

    // Si hay ceros a la izquierda, desplazamos la mantisa a la izquierda
    if (ceros_izquierda > 0 && ceros_izquierda < SIZE) {
        for (int i = 0; i < SIZE - ceros_izquierda; i++) {
            num->number_float.number[i] = num->number_float.number[i + ceros_izquierda];
        }
        for (int i = SIZE - ceros_izquierda; i < SIZE; i++) {
            num->number_float.number[i] = 0;
        }
        // Ajustamos el exponente según la cantidad de ceros eliminados
        num->exponente -= ceros_izquierda * 9;
    }

    // Ahora que hemos movido la mantisa a la izquierda, contar los dígitos significativos
    int digitos_significativos = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        if (num->number_float.number[i] > 0) {
            digitos_significativos = i + 1;
            break;
        }
    }

    // Si el número es demasiado grande y no cabe en la mantisa, lo desplazamos
    if (digitos_significativos == SIZE && num->number_float.number[SIZE - 1] >= 1000000000) {
        for (int i = 0; i < SIZE - 1; i++) {
            num->number_float.number[i] = num->number_float.number[i + 1];
        }
        num->number_float.number[SIZE - 1] = 0;
        num->exponente += 9;  // Ajustar el exponente si la mantisa se ha desplazado
    }

    // Normalizamos el exponente después de mover la mantisa
    while (num->number_float.number[0] == 0 && num->exponente > 0) {
        // Si el primer dígito es 0, movemos la mantisa hacia la derecha
        for (int i = SIZE - 1; i > 0; i--) {
            num->number_float.number[i] = num->number_float.number[i - 1];
        }
        num->number_float.number[0] = 0;
        num->exponente--;  // Reducir el exponente cuando desplazamos a la derecha
    }
}





// Multiplica la representación entera (mantisa) por 10.
void mult_x10(BigInt_t* big_int) {
    size_t size = big_int->size;
    subsize_t* num = big_int->number;
    size_t carry = 0;
    for (int i = 0; i < size; i++) {
        size_t prod = (size_t)num[i] * 10 + carry;
        num[i] = (subsize_t)(prod & 0xFFFFFFFF);
        carry = prod >> 32;
    }
    // Si hay carry al final y espacio en el array, añadirlo
    if (carry && size < big_int->size) {
        num[size] = (subsize_t)carry;
    }
}


// Divide la representación entera (mantisa) entre 10.
void div_x10(BigInt_t* big_int) {
    size_t size = big_int->size;
    subsize_t* num = big_int->number;
    size_t remainder = 0;
    for (int i = size - 1; i >= 0; i--) {
        size_t cur = remainder * 1000000000ULL + num[i];
        num[i] = (subsize_t)(cur / 10);
        remainder = cur % 10;
    }
}


/* 
 * (Exponentiation by squaring - fast exponentiation)
 * Function that performs fast exponentiation using repeated multiplications
 * This is an efficient method for calculating the power of a number, especially 
 * when exponents are large. The algorithm is based on the binary decomposition 
 * of the exponent and allows reducing the number of multiplications needed.
 * 
 * The algorithm works as follows:
 * 
 *  - If the exponent is 0, the result is 1 (any number raised to 0 is 1).
 *  - If the exponent is odd, you can decompose it into base x = base x (base - 1)base x =base x(basex - 1 ).
 *  - If the exponent is even, you can decompose it into basex = (base x / 2 ) 2 base x = (base x/2 ) 2.
 */ 
void pow_BigInt_rapida(BigInt_t *base, BigInt_t *exponente, BigInt_t *resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, pow_BigInt_rapida)
        TYPE_DATA_DBG(BigInt_t*, "base = %p")
        TYPE_DATA_DBG(BigInt_t*, "exponente = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    base, exponente, resultado);
    size_t size = base->size;
    BigInt_t temp_result = {calloc(size * 2, sizeof(subsize_t)), size};
    temp_result.number[0] = 1;  // Initialize the result as 1

    BigInt_t base_temp = {calloc(size  * 2, sizeof(subsize_t)), size};
    memcpy(base_temp.number, base->number, sizeof(subsize_t) * size);

    BigInt_t exponente_temp = {calloc(size, sizeof(subsize_t)), size};
    memcpy(exponente_temp.number, exponente->number, sizeof(subsize_t) * size);

    while (!is_zero(&exponente_temp)) {
        // If the exponent is odd, multiply the result by the base
        if (exponente_temp.number[0] & 1) {
            mult_arr(&temp_result, &base_temp, &temp_result);
        }

        // Raise the base to the square (base = base * base)
        mult_arr(&base_temp, &base_temp, &base_temp);

        // Divide the exponent by 2 (shift to the right)
        div_x2(&exponente_temp);
    }

    // In the end, the result is in temp_result
    memcpy(resultado->number, temp_result.number, sizeof(subsize_t) * size);
    free(temp_result.number);
    free(base_temp.number);
    free(exponente_temp.number);
}

void lshift_BigInt(BigInt_t *arr, int shift) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, lshift_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "arr = %p")
        TYPE_DATA_DBG(int, "shift = %d")
    END_TYPE_FUNC_DBG,
    arr, shift);
    int size = arr->size;
    for (int i = 0; i < size - shift; i++) {
        arr->number[i] = arr->number[i + shift];
    }
    for (int i = size - shift; i < size; i++) {
        arr->number[i] = 0;
    }
}
void rshift_BigInt(BigInt_t *arr, int shift) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, rshift_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "arr = %p")
        TYPE_DATA_DBG(int, "shift = %d")
    END_TYPE_FUNC_DBG,
    arr, shift);
    int size = arr->size;
    for (int i = size - 1; i >= shift; i--) {
        arr->number[i] = arr->number[i - shift];
    }
    for (int i = 0; i < shift; i++) {
        arr->number[i] = 0;
    }
}

/*
 * Comparación de dígitos: Se recorre el arreglo de dígitos desde el último 
 * hasta el primero (de derecha a izquierda). En cada iteración, se comparan 
 * los dígitos correspondientes de arr1 y arr2:
 * 
 * Si el dígito de arr1 es mayor que el de arr2, la función retorna 1, 
 * indicando que arr1 es mayor que arr2.
 * Si el dígito de arr1 es menor que el de arr2, la función retorna -1, 
 * indicando que arr1 es menor que arr2.
 * Igualdad: Si todos los dígitos son iguales, la función retorna 0, 
 * indicando que arr1 y arr2 son iguales.
 */
int cmp_BigInt(BigInt_t *arr1, BigInt_t *arr2) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, cmp_BigInt)
        TYPE_DATA_DBG(BigInt_t*, "arr1 = %p")
        TYPE_DATA_DBG(BigInt_t*, "arr2 = %p")
    END_TYPE_FUNC_DBG,
    arr1, arr2);
    int size = arr1->size;
    for (int i = size - 1; i >= 0; i--) {
        if (arr1->number[i] > arr2->number[i]) return 1;
        if (arr1->number[i] < arr2->number[i]) return -1;
    }
    return 0;
}

// Direct multiplication
void pow_BigInt_directa(BigInt_t *base, BigInt_t *exponente, BigInt_t *resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, pow_BigInt_rapida)
        TYPE_DATA_DBG(BigInt_t*, "base = %p")
        TYPE_DATA_DBG(BigInt_t*, "exponente = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    base, exponente, resultado);
    size_t size = base->size;
    BigInt_t temp_result = {calloc(size, sizeof(subsize_t)), size};
    BigInt_t temp_multiply = {calloc(size, sizeof(subsize_t)), size};
    
    // Initialize result to 1
    memset(resultado->number, 0, sizeof(subsize_t) * size);
    resultado->number[0] = 1;
    
    // If the exponent is 0, the result is already 1
    if (is_zero(exponente)) {
        return;
    }
    
    // Copy the base to temp_result
    memcpy(temp_result.number, base->number, sizeof(subsize_t) * size);
    
    // Perform the exponentiation
    for (subsize_t i = 1; i < exponente->number[0]; i++) {
        mult_arr(resultado, &temp_result, &temp_multiply);
        memcpy(temp_result.number, temp_multiply.number, sizeof(subsize_t) * size);
    }
    
    // Copy the final result
    memcpy(resultado->number, temp_result.number, sizeof(subsize_t) * size);
     free(temp_result.number);
    free(temp_multiply.number);
}

// Dynamic exponentiation function (chooses the best approach)
void pow_BigInt(BigInt_t *base, BigInt_t *exponente, BigInt_t *resultado) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, pow_BigInt_rapida)
        TYPE_DATA_DBG(BigInt_t*, "base = %p")
        TYPE_DATA_DBG(BigInt_t*, "exponente = %p")
        TYPE_DATA_DBG(BigInt_t*, "resultado = %p")
    END_TYPE_FUNC_DBG,
    base, exponente, resultado);
    subsize_t exponente_val = exponente->number[0];  // We assume that the exponent fits in a single subsize_t

    // Choose the best strategy depending on the size of the exponent
    if (exponente_val > 10) {
        printf("Using Fast Exponentiation.\n");
        pow_BigInt_rapida(base, exponente, resultado);
    } else {
        printf("Using Direct Exponentiation.\n");
        pow_BigInt_directa(base, exponente, resultado);
    }
}

/**
 * @brief Libero el entero que contiene este big int, pero no la estructura que 
 * lo representa, si es que fue creado usando memoria dinamica.
 * 
 * @param number espera recibir un puntero a un big int al que liberar su entero
 * , puede a ver sido reservado usando memoria dinamica o no.
 */
void free_BigInt_inside(BigInt_t* number) {
    if (number == NULL) return;
    if (number->number == NULL) return;
    free(number->number);
    number->number = NULL;
}
void free_BigFloat_inside(BigFloat_t* number) {
    if (number == NULL) return;
    if (number->number_float.number == NULL) return;
    free(number->number_float.number);
    number->number_float.number = NULL;
}