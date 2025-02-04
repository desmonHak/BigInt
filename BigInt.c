#include "BigInt.h"

void suma_sin_suma(subsize_t* a, subsize_t* b, subsize_t* resultado, int size) {
    subsize_t carry = 0;

    // Sumamos bloque por bloque de 32 bits
    for (int i = 0; i < size; i++) {
        uint64_t temp_resultado = (uint64_t)a[i] + b[i] + carry;
        resultado[i] = (subsize_t)temp_resultado;
        carry = (subsize_t)(temp_resultado >> 32);
    }

    // Si al final hay un acarreo y hay espacio en el array, almacenarlo
    if (carry && size < SIZE) {
        resultado[size] = carry;
    }
}

void complemento_a_dos(subsize_t* num, int size) {
    for (int i = 0; i < size; i++) {
        num[i] = ~num[i];
    }
    
    subsize_t carry = 1;
    for (int i = 0; i < size && carry; i++) {
        uint64_t sum = (uint64_t)num[i] + carry;
        num[i] = (subsize_t)sum;
        carry = (subsize_t)(sum >> 32);
    }
}

void resta_sin_resta(subsize_t* a, subsize_t* b, subsize_t* resultado, int size) {
    subsize_t b_complemento[SIZE];
    memcpy(b_complemento, b, sizeof(subsize_t) * size);
    
    complemento_a_dos(b_complemento, size);
    suma_sin_suma(a, b_complemento, resultado, size);
}



uint64_t suma_con_desbordamiento(subsize_t a, subsize_t b, subsize_t* resultado) {
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

bool es_mayor(subsize_t* a, subsize_t* b, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (a[i] > b[i]) return true;
        if (a[i] < b[i]) return false;
    }
    return false;  // Si son iguales, devuelve false
}
// Función que simula la división usando el algoritmo de Booth
void division_booth(subsize_t* dividend, subsize_t* divisor, subsize_t* cociente, subsize_t* residuo, int size) {
    subsize_t Q[SIZE];        // Cociente (inicializado con el dividendo)
    subsize_t Q_1 = 0;        // Bit extra Q-1
    subsize_t M[SIZE];        // Divisor
    subsize_t A[SIZE] = {0};  // Acumulador (inicialmente 0)

    // Copiar el dividendo en Q y el divisor en M
    memcpy(Q, dividend, sizeof(subsize_t) * size);
    memcpy(M, divisor, sizeof(subsize_t) * size);


    // Proceso de división (Booth)
    for (int i = 0; i < size * 32; i++) {
        // Desplazamiento a la izquierda de A:Q:Q_1
        int msb_A = (A[size-1] & 0x80000000) ? 1 : 0;
        for (int j = size - 1; j > 0; j--) {
            A[j] = (A[j] << 1) | (A[j-1] >> 31);
        }
        A[0] = (A[0] << 1) | (Q[size-1] >> 31);
        for (int j = size - 1; j > 0; j--) {
            Q[j] = (Q[j] << 1) | (Q[j-1] >> 31);
        }
        Q[0] = (Q[0] << 1) | Q_1;
        Q_1 = msb_A;

        // Realizar A = A - M o A = A + M según el bit más significativo de A
        if (msb_A) {
            subsize_t temp[SIZE];
            suma_sin_suma(A, M, temp, size);
            memcpy(A, temp, sizeof(subsize_t) * size);
            Q[0] |= 1;  // Set LSB of Q to 1
        } else {
            subsize_t temp[SIZE];
            resta_sin_resta(A, M, temp, size);
            memcpy(A, temp, sizeof(subsize_t) * size);
            if (A[size-1] & 0x80000000) {
                suma_sin_suma(A, M, temp, size);
                memcpy(A, temp, sizeof(subsize_t) * size);
            } else {
                Q[0] |= 1;  // Set LSB of Q to 1
            }
        }
    }

    // Copiar el cociente y el residuo a los arreglos de salida
    memcpy(cociente, Q, sizeof(subsize_t) * size);
    memcpy(residuo, A, sizeof(subsize_t) * size);
}

void exponenciacion_modular(
        subsize_t* base, subsize_t* exponente, 
        subsize_t* modulo, subsize_t* resultado, 
        int size
    ) {
    // https://es.wikipedia.org/wiki/Exponenciación_modular
    subsize_t temp_result[SIZE] = {0};
    temp_result[0] = 1;  // Inicializamos el resultado como 1

    subsize_t base_temp[SIZE];
    memcpy(base_temp, base, sizeof(subsize_t) * size);

    subsize_t exponente_temp[SIZE];
    memcpy(exponente_temp, exponente, sizeof(subsize_t) * size);

    subsize_t cociente[SIZE];  // Para almacenar el cociente (que no usaremos)

    while (!es_cero(exponente_temp, size)) {
        // Si el exponente es impar, multiplicamos el resultado por la base
        if (exponente_temp[0] & 1) {
            multiplicar_arreglos(temp_result, base_temp, temp_result, size);
            // Aplicamos el módulo
            division_booth(temp_result, modulo, cociente, temp_result, size);  
        }

        // Elevamos la base al cuadrado (base = base * base)
        multiplicar_arreglos(base_temp, base_temp, base_temp, size);
        division_booth(base_temp, modulo, cociente, base_temp, size);  // Aplicamos el módulo

        // Dividimos el exponente entre 2 (desplazamiento a la derecha)
        dividir_por_2(exponente_temp, size);
    }

    // Al final, el resultado está en temp_result
    memcpy(resultado, temp_result, sizeof(subsize_t) * size);
}



void imprimir_decimal(subsize_t* num, int size) {

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
    int longitud_valor_decimal = len_decimal(longitud_hex(num, size)); // 0.82 * longitud del número hex = aprox longitud num decimal
    char *decimal = malloc(longitud_valor_decimal + 1);
    memset(decimal, '0', longitud_valor_decimal);
    decimal[longitud_valor_decimal] = '\0';
    char *temp = calloc(longitud_valor_decimal + 1, sizeof(char));
    
for (int i = size - 1; i >= 0; i--) {
        subsize_t n = num[i];
        for (int bit = 31; bit >= 0; bit--) {
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
// Función para comparar si un arreglo es mayor o igual que otro
int es_mayor_o_igual(subsize_t* arr1, subsize_t* arr2, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (arr1[i] > arr2[i]) return 1;
        if (arr1[i] < arr2[i]) return 0;
    }
    return 1; // Son iguales
}

// Función para restar dos arreglos
void resta_arreglos(subsize_t* a, subsize_t* b, subsize_t* resultado, int size) {
    int borrow = 0;
    for (int i = 0; i < size; i++) {
        if (a[i] >= b[i] + borrow) {
            resultado[i] = a[i] - b[i] - borrow;
            borrow = 0;
        } else {
            resultado[i] = (SUBSIZE_MAX - b[i] - borrow) + a[i] + 1;
            borrow = 1;
        }
    }
}


size_t longitud_hex(subsize_t* num, size_t size) {
    if (size == 0) {
        return 0;
    }
    if (num == NULL) {
        return 0;
    }
    size_t longitud = 0;
    ssize_t i;

    // Encuentra el primer elemento no nulo desde la izquierda
    for (i = size; i > 0; i--) {
        if (num[i-1] != 0) {
            longitud = i;
            break;
        }
    }


    // Si todos los elementos son cero, la longitud es 1
    if (longitud == 0) {
        return 1;
    }

    // Cuenta los dígitos hexadecimales significativos en el elemento más significativo
    subsize_t msb = num[longitud - 1];
    size_t digitos = 8; // Máximo 8 dígitos hexadecimales en un subsize_t
    while (msb != 0) {
        msb >>= 4;
        digitos--;
    }

    return (longitud - 1) * 8 + (8 - digitos);
}


// Multiplica la representación entera (mantisa) por 10.
void multiplicar_por_10(subsize_t* num, int size) {
    uint64_t carry = 0;
    for (int i = 0; i < size; i++) {
        uint64_t prod = (uint64_t)num[i] * 10 + carry;
        num[i] = (subsize_t)(prod % 1000000000);
        carry = prod / 1000000000;
    }
}
 
// Divide la representación entera (mantisa) entre 10.
void dividir_por_10(subsize_t* num, int size) {
    uint64_t remainder = 0;
    for (int i = size - 1; i >= 0; i--) {
        uint64_t cur = remainder * 1000000000ULL + num[i];
        num[i] = (subsize_t)(cur / 10);
        remainder = cur % 10;
    }
}


// Función para verificar si un número es cero
bool es_cero(subsize_t* num, int size) {
    for (int i = 0; i < size; i++) {
        if (num[i] != 0) return false;
    }
    return true;
}

// División por 2 del número representado como arreglo de enteros
void dividir_por_2(subsize_t* num, int size) {
    unsigned long long carry = 0;
    for (int i = size - 1; i >= 0; i--) {
        unsigned long long val = (unsigned long long)num[i] + (carry << 32);
        num[i] = (subsize_t)(val >> 1);
        carry = val & 1;
    }
}


/* 
 * (Exponenciación por cuadrado - exponenciación rápida)
 * Función que realiza la exponenciación rápida usando multiplicaciones repetidas
 * Este es un método eficiente para calcular la potencia de un número, especialmente 
 * cuando los exponentes son grandes. El algoritmo se basa en la descomposición binaria 
 * del exponente y permite reducir la cantidad de multiplicaciones necesarias.
 * 
 * El algoritmo funciona de la siguiente forma:
 * 
 *  - Si el exponente es 0, el resultado es 1 (cualquier número elevado a 0 es 1).
 *  - Si el exponente es impar, puedes descomponerlo en base x = base x (base - 1)base x =base x(basex - 1 ).
 *  - Si el exponente es par, puedes descomponerlo en basex = (base x / 2 ) 2 base x = (base x/2 ) 2.
 */ 
void exponenciacion_rapida(subsize_t* base, subsize_t* exponente, subsize_t* resultado, int size) {
    subsize_t temp_result[SIZE] = {0};
    temp_result[0] = 1;  // Inicializamos el resultado como 1

    subsize_t base_temp[SIZE];
    memcpy(base_temp, base, sizeof(subsize_t) * size);

    subsize_t exponente_temp[SIZE];
    memcpy(exponente_temp, exponente, sizeof(subsize_t) * size);

    while (!es_cero(exponente_temp, size)) {
        // Si el exponente es impar, multiplicamos el resultado por la base
        if (exponente_temp[0] & 1) {
            multiplicar_arreglos(temp_result, base_temp, temp_result, size);
        }

        // Elevamos la base al cuadrado (base = base * base)
        multiplicar_arreglos(base_temp, base_temp, base_temp, size);

        // Dividimos el exponente entre 2 (desplazamiento a la derecha)
        dividir_por_2(exponente_temp, size);
    }

    // Al final, el resultado está en temp_result
    memcpy(resultado, temp_result, sizeof(subsize_t) * size);
}

void desplazar_izquierda(subsize_t* arr, int size, int shift) {
    for (int i = 0; i < size - shift; i++) {
        arr[i] = arr[i + shift];
    }
    for (int i = size - shift; i < size; i++) {
        arr[i] = 0;
    }
}
void desplazar_derecha(subsize_t* arr, int size, int shift) {
    for (int i = size - 1; i >= shift; i--) {
        arr[i] = arr[i - shift];
    }
    for (int i = 0; i < shift; i++) {
        arr[i] = 0;
    }
}


int comparar_arrays(subsize_t* arr1, subsize_t* arr2, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (arr1[i] > arr2[i]) return 1;
        if (arr1[i] < arr2[i]) return -1;
    }
    return 0;
}


// Multiplicación directa
void exponenciacion_directa(subsize_t* base, subsize_t* exponente, subsize_t* resultado, int size) {
    subsize_t temp_result[SIZE];
    subsize_t temp_multiply[SIZE];
    
    // Inicializar resultado a 1
    memset(resultado, 0, sizeof(subsize_t) * size);
    resultado[0] = 1;
    
    // Si el exponente es 0, el resultado ya es 1
    if (es_cero(exponente, size)) {
        return;
    }
    
    // Copiar la base a temp_result
    memcpy(temp_result, base, sizeof(subsize_t) * size);
    
    // Realizar la exponenciación
    for (subsize_t i = 1; i < exponente[0]; i++) {
        multiplicar_arreglos(resultado, temp_result, temp_multiply, size);
        memcpy(temp_result, temp_multiply, sizeof(subsize_t) * size);
    }
    
    // Copiar el resultado final
    memcpy(resultado, temp_result, sizeof(subsize_t) * size);
}


// Función de Exponenciación dinámica (elige el mejor enfoque)
void exponenciacion(subsize_t* base, subsize_t* exponente, subsize_t* resultado, int size) {
    subsize_t exponente_val = exponente[0];  // Asumimos que el exponente cabe en un solo subsize_t

    // Elegir la mejor estrategia según el tamaño del exponente
    if (exponente_val > 10) {
        printf("Usando Exponenciacion Rapida.\n");
        exponenciacion_rapida(base, exponente, resultado, size);
    } else {
        printf("Usando Exponenciacion Directa.\n");
        exponenciacion_directa(base, exponente, resultado, size);
    }
}

// Función para multiplicar dos arreglos (multiplicación de enteros grandes)
void multiplicar_arreglos(subsize_t* a, subsize_t* b, subsize_t* resultado, int size) {
    subsize_t temp[SIZE * 2] = {0};

    for (int i = 0; i < size; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < size; j++) {
            uint64_t product = (uint64_t)a[i] * b[j] + temp[i + j] + carry;
            temp[i + j] = (subsize_t)product;
            carry = product >> 32;
        }
        temp[i + size] = carry;
    }

    memcpy(resultado, temp, sizeof(subsize_t) * size);
}


void imprimir_hex(uint32_t *num, int size) {
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("%08X", num[i]);
    }
}

// Función para convertir un número hexadecimal o decimal en un arreglo de enteros de 32 bits
void string_a_array(const char* num_str, subsize_t* arr, int size) {
    if (arr == NULL) {
        printf("Error: Array de salida es NULL.\n");
        return;
    }

    if (num_str == NULL) {
        printf("Error: Cadena de entrada es NULL.\n");
        return;
    }

    if (size == 0) {
        printf("Error: Tamaño del arreglo de salida es 0.\n");
        return;
    }
    size_t len = strlen(num_str);
    puts("a");
    
    memset(arr, 0, sizeof(subsize_t) * size);

        if (len >= 2 && num_str[0] == '0' && (num_str[1] == 'x' || num_str[1] == 'X')) {
        // Convertir desde hexadecimal
        int arr_index = 0;
        subsize_t current_value = 0;
        int shift = 0;

        for (int i = len - 1; i >= 2 && arr_index < size; i--) {
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
                arr[arr_index++] = current_value;
                current_value = 0;
                shift = 0;
            }
        }

        if (shift > 0 && arr_index < size) {
            arr[arr_index] = current_value;
        }
    } else {
        // Convertir desde decimal (sin cambios)
        for (int i = 0; i < len; i++) {
            subsize_t digit = num_str[i] - '0';
            if (digit > 9) continue; // Ignorar caracteres no numéricos

            subsize_t carry = 0;
            for (int j = 0; j < size; j++) {
                uint64_t product = (uint64_t)arr[j] * 10 + carry;
                arr[j] = (subsize_t)(product & 0xFFFFFFFF);
                carry = (subsize_t)(product >> 32);
            }

            carry = digit;
            for (int j = 0; j < size && carry; j++) {
                uint64_t sum = (uint64_t)arr[j] + carry;
                arr[j] = (subsize_t)(sum & 0xFFFFFFFF);
                carry = (subsize_t)(sum >> 32);
            }
        }
    }
}
// Implementación de las nuevas funciones
void normalizar_float_grande(float_grande* num) {
    // Contar ceros a la izquierda de la mantisa
    int ceros_izquierda = 0;
    while (ceros_izquierda < SIZE && num->mantisa[ceros_izquierda] == 0) {
        ceros_izquierda++;
    }

    // Si hay ceros a la izquierda, desplazamos la mantisa a la izquierda
    if (ceros_izquierda > 0 && ceros_izquierda < SIZE) {
        for (int i = 0; i < SIZE - ceros_izquierda; i++) {
            num->mantisa[i] = num->mantisa[i + ceros_izquierda];
        }
        for (int i = SIZE - ceros_izquierda; i < SIZE; i++) {
            num->mantisa[i] = 0;
        }
        // Ajustamos el exponente según la cantidad de ceros eliminados
        num->exponente -= ceros_izquierda * 9;
    }

    // Ahora que hemos movido la mantisa a la izquierda, contar los dígitos significativos
    int digitos_significativos = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        if (num->mantisa[i] > 0) {
            digitos_significativos = i + 1;
            break;
        }
    }

    // Si el número es demasiado grande y no cabe en la mantisa, lo desplazamos
    if (digitos_significativos == SIZE && num->mantisa[SIZE - 1] >= 1000000000) {
        for (int i = 0; i < SIZE - 1; i++) {
            num->mantisa[i] = num->mantisa[i + 1];
        }
        num->mantisa[SIZE - 1] = 0;
        num->exponente += 9;  // Ajustar el exponente si la mantisa se ha desplazado
    }

    // Normalizamos el exponente después de mover la mantisa
    while (num->mantisa[0] == 0 && num->exponente > 0) {
        // Si el primer dígito es 0, movemos la mantisa hacia la derecha
        for (int i = SIZE - 1; i > 0; i--) {
            num->mantisa[i] = num->mantisa[i - 1];
        }
        num->mantisa[0] = 0;
        num->exponente--;  // Reducir el exponente cuando desplazamos a la derecha
    }
}




void inicializar_float_grande(float_grande* num, const char* valor_str) {
    memset(num, 0, sizeof(float_grande));
    const char* ptr = valor_str;
    
    if (*ptr == '-') {
        num->signo = 1;
        ptr++;
    }

    char mantisa_str[1000] = {0};
    int mantisa_len = 0;
    int punto_decimal = -1;

    while (*ptr && mantisa_len < 999) {
        if (*ptr >= '0' && *ptr <= '9') {
            mantisa_str[mantisa_len++] = *ptr;
        } else if (*ptr == '.' && punto_decimal == -1) {
            punto_decimal = mantisa_len;
        } else if (*ptr == 'e' || *ptr == 'E') {
            break;
        }
        ptr++;
    }

    if (punto_decimal == -1) punto_decimal = mantisa_len;
    num->exponente = punto_decimal - mantisa_len;

    if (*ptr == 'e' || *ptr == 'E') {
        ptr++;
        int exp_sign = (*ptr == '-') ? -1 : 1;
        if (*ptr == '+' || *ptr == '-') ptr++;
        int exp = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            exp = exp * 10 + (*ptr - '0');
            ptr++;
        }
        num->exponente += exp_sign * exp;
    }

    string_a_array(mantisa_str, num->mantisa, SIZE);
    normalizar_float_grande(num);
}
bool es_menor(subsize_t* a, subsize_t* b) {
    for (int i = 0; i < SIZE; i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    }
    return false;
}


void sumar_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    //if (a->exponente < b->exponente)  {b->exponente = a->exponente;}
    float_grande temp_a = *a, temp_b = *b;
    int diff_exp = temp_a.exponente - temp_b.exponente;
    
    // Alinear exponentes
    if (diff_exp > 0) {
        for (int i = 0; i < diff_exp; i++) dividir_por_10(temp_b.mantisa, SIZE);
    } else if (diff_exp < 0) {
        for (int i = 0; i < -diff_exp; i++) dividir_por_10(temp_a.mantisa, SIZE);
    }

    // Realizar la suma
    suma_sin_suma(temp_a.mantisa, temp_b.mantisa, resultado->mantisa, SIZE);
    resultado->exponente = (diff_exp > 0) ? temp_a.exponente : temp_b.exponente;
    resultado->signo = 0;  // Asumimos que ambos números son positivos por ahora

    normalizar_float_grande(resultado);
}


void multiplicar_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    // Separar partes enteras y decimales
    float_grande a_entero, a_decimal, b_entero, b_decimal;
    separar_partes(a, &a_entero, &a_decimal);
    separar_partes(b, &b_entero, &b_decimal);

    // Multiplicar partes enteras
    float_grande resultado_entero;
    multiplicar_partes(&a_entero, &b_entero, &resultado_entero);

    // Multiplicar partes decimales
    float_grande resultado_decimal;
    multiplicar_partes(&a_decimal, &b_decimal, &resultado_decimal);

    // Multiplicar cruzado y sumar
    float_grande temp1, temp2;
    multiplicar_partes(&a_entero, &b_decimal, &temp1);
    multiplicar_partes(&a_decimal, &b_entero, &temp2);

    // Sumar todos los resultados
    sumar_float_grande(&resultado_entero, &resultado_decimal, resultado);
    sumar_float_grande(resultado, &temp1, resultado);
    sumar_float_grande(resultado, &temp2, resultado);

    resultado->signo = a->signo ^ b->signo;

    // Normalizar el resultado final
    normalizar_float_grande(resultado);
}

void separar_partes(float_grande* num, float_grande* entero, float_grande* decimal) {
    *entero = *num;
    *decimal = *num;

    if (num->exponente >= 0) {
        // Todo es parte entera
        decimal->mantisa[0] = 0;
        decimal->exponente = 0;
    } else if (num->exponente < -9 * (SIZE - 1)) {
        // Todo es parte decimal
        entero->mantisa[0] = 0;
        entero->exponente = 0;
    } else {
        int shift = -num->exponente / 9;
        for (int i = 0; i < SIZE - shift; i++) {
            entero->mantisa[i] = num->mantisa[i + shift];
        }
        for (int i = SIZE - shift; i < SIZE; i++) {
            entero->mantisa[i] = 0;
        }
        entero->exponente = 0;

        for (int i = 0; i < shift; i++) {
            decimal->mantisa[i] = num->mantisa[i];
        }
        for (int i = shift; i < SIZE; i++) {
            decimal->mantisa[i] = 0;
        }
        decimal->exponente = num->exponente;
    }
}
int longitud_decimal(subsize_t num) {
    if (num == 0) return 1;
    int len = 0;
    while (num > 0) {
        num /= 10;
        len++;
    }
    return len;
}

void multiplicar_partes(float_grande* a, float_grande* b, float_grande* resultado) {
    subsize_t producto[SIZE * 2] = {0};

    for (int i = 0; i < SIZE; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < SIZE; j++) {
            uint64_t temp = producto[i + j] + (uint64_t)a->mantisa[i] * b->mantisa[j] + carry;
            producto[i + j] = temp % 1000000000;
            carry = temp / 1000000000;
        }
        producto[i + SIZE] = carry;
    }

    // Calcular el exponente como la suma de los exponentes de ambos números
    resultado->exponente = a->exponente + b->exponente;

    // Ahora corregir el exponente en función del desplazamiento de ceros en la mantisa
    int shift = 0;
    while (shift < SIZE * 2 && producto[shift] == 0) {
        shift++;
    }

    // Ajustar el exponente según el número de ceros desplazados
    resultado->exponente -= shift * 9;

    // Copiar los valores de la mantisa desplazados correctamente
    for (int i = 0; i < SIZE; i++) {
        resultado->mantisa[i] = (i + shift < SIZE * 2) ? producto[i + shift] : 0;
    }

    // Ahora normalizar el número para corregir el exponente si es necesario
    normalizar_float_grande(resultado);
}

void dividir_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    // Comprobar si el divisor es cero
    if (es_cero(b->mantisa, SIZE)) {
        printf("Error: División por cero\n");
        return;
    }

    // Inicializar el resultado
    resultado->signo = a->signo ^ b->signo;
    
    // Preparar las mantisas para la división
    subsize_t dividendo[SIZE * 2] = {0};
    subsize_t divisor[SIZE * 2] = {0};
    memcpy(dividendo, a->mantisa, sizeof(subsize_t) * SIZE);
    memcpy(divisor, b->mantisa, sizeof(subsize_t) * SIZE);

    // Normalizar el dividendo y el divisor
    int shift_dividendo = 0, shift_divisor = 0;
    while (dividendo[SIZE * 2 - 1] == 0 && !es_cero(dividendo, SIZE * 2)) {
        desplazar_izquierda(dividendo, SIZE * 2, 1);
        shift_dividendo++;
    }
    while (divisor[SIZE * 2 - 1] == 0 && !es_cero(divisor, SIZE * 2)) {
        desplazar_izquierda(divisor, SIZE * 2, 1);
        shift_divisor++;
    }

    // Realizar la división
    subsize_t cociente[SIZE * 2] = {0};
    int precision = SIZE * 64;  // Precisión deseada en bits

    for (int i = 0; i < precision; i++) {
        if (es_mayor_o_igual(dividendo, divisor, SIZE * 2)) {
            resta_arreglos(dividendo, divisor, dividendo, SIZE * 2);
            cociente[0] |= 1;
        }
        desplazar_izquierda(cociente, SIZE * 2, 1);
        desplazar_izquierda(dividendo, SIZE * 2, 1);
    }

    // Copiar el resultado al float_grande
    memcpy(resultado->mantisa, cociente, sizeof(subsize_t) * SIZE);

    // Ajustar el exponente
    resultado->exponente = a->exponente - b->exponente - shift_dividendo + shift_divisor;

    // Normalizar el resultado
    normalizar_float_grande(resultado);
}




void imprimir_float_grande(float_grande* num) {
    if (num->signo) printf("-");
    
    // Imprimir los dígitos significativos
    int digitos_impresos = 0;
    bool imprimiendo = false;
    for (int i = SIZE - 1; i >= 0; i--) {
        subsize_t valor = num->mantisa[i];
        for (int j = 0; j < 9; j++) {  // Cada subsize_t puede contener hasta 9 dígitos
            int digito = valor / 100000000;
            if (digito != 0 || imprimiendo) {
                printf("%d", digito);
                imprimiendo = true;
                digitos_impresos++;
            }
            valor = (valor % 100000000) * 10;
        }
    }
    
    if (digitos_impresos == 0) {
        printf("0");
    }
    
    printf(" x 10^%d", num->exponente);
}

int contar_digitos(subsize_t* cociente, int size) {
    char* str = malloc(size * 32 + 1); // Asumiendo que subsize_t es de 32 bits
    str[0] = '\0';
    char temp[33];
    int total_digitos = 0;

    for (int i = size - 1; i >= 0; i--) {
        sprintf(temp, "%u", cociente[i]);
        strcat(str, temp);
    }

    // Eliminar ceros a la izquierda
    char* start = str;
    while (*start == '0' && *(start + 1) != '\0') {
        start++;
    }

    total_digitos = strlen(start);
    free(str);

    return total_digitos;
}


#define MAX_ITERACIONES 32
void division_float_grande(subsize_t* a, subsize_t* b, float_grande* resultado) {
    // Comprobar si el divisor es cero
    if (es_cero(b, SIZE)) {
        printf("Error: División por cero\n");
        return;
    }
    subsize_t cociente[SIZE] = {0};   // Donde almacenaremos el cociente
    subsize_t residuo[SIZE] = {0};    // Donde almacenaremos el residuo

    // Llamamos a la función de división
    division_booth(a, b, cociente, residuo, SIZE);
    printf("Cociente: ");
    imprimir_hex(cociente, SIZE);
    printf("\nResiduo: ");
    imprimir_hex(residuo, SIZE);
    printf("\n");

    
    memset(resultado, 0, sizeof(float_grande));
    memcpy(&(resultado->mantisa), cociente, SIZE); // copiar el cociente al resultado
    //resultado->signo = !(a->signo ^ b->signo);  // establecer el signo del resultado
    /* -- = +; ++ = +; + - = -; - + = - */
    //int cotiente_len = contar_digitos(cociente, SIZE) *-1;
    //resultado->exponente = contar_digitos(cociente, SIZE);  // Calcular la longitud de la mantisa


    // si el resto no es 0
    // Si el resto no es 0, seguir dividiendo
    if (!es_cero(residuo, SIZE)) {
        int iteraciones = 0;  
        while (!es_cero(residuo, SIZE) && iteraciones < MAX_ITERACIONES) {
            iteraciones++;
            
            // Si el residuo es menor que el divisor, multiplicar el residuo por 10
            if (es_menor(b, residuo)) {
                multiplicar_por_10(residuo, SIZE);
            } else {
                subsize_t cociente_temp[SIZE] = {0};
                subsize_t residuo_temp[SIZE] = {0};
                subsize_t resultado_temp[SIZE] = {0};

                // Realizar la división sobre el residuo
                division_booth(residuo, b, cociente_temp, residuo_temp, SIZE);
                //  cociente_temp = residuo / b
                //  residuo_temp  = residuo % b

                multiplicar_por_10(residuo_temp, SIZE);
                // residuo_temp *= 10

                // Desplazar la mantisa del resultado
                multiplicar_por_10(resultado->mantisa, SIZE);
                // mantisa *= 10

                
                // Sumar el cociente intermedio al resultado final
                suma_sin_suma(resultado->mantisa, cociente_temp, resultado_temp, SIZE);
                // resultado_temp = resultado_temp + cociente_temp

                memcpy(&(resultado->mantisa), resultado_temp, SIZE);

                // Imprimir estado intermedio
                printf("Cociente Temp: ");
                imprimir_hex(cociente_temp, SIZE);
                printf("\nResiduo Temp: ");
                imprimir_hex(residuo_temp, SIZE);
                

                // Actualizamos el residuo
                memcpy(residuo, residuo_temp, SIZE);
                resultado->exponente++;  // Ajustar el exponente
            }
            printf("\nfloat: ");
                imprimir_float_grande(resultado);
                printf("\n");
        }
    }
    resultado->exponente *=-1;
    // Normalizar el resultado
    //normalizar_float_grande(resultado);
}


void generate_randomNumber(subsize_t* a, subsize_t* b, float_grande* resultado) {
    if (es_cero(b, SIZE)) {
        printf("Error: División por cero\n");
        return;
    }
    subsize_t cociente[SIZE] = {0};
    subsize_t residuo[SIZE] = {0};

    division_booth(a, b, cociente, residuo, SIZE);
    printf("Cociente inicial: ");
    imprimir_hex(cociente, SIZE);
    printf("\nResiduo inicial: ");
    imprimir_hex(residuo, SIZE);
    printf("\n");

    memset(resultado, 0, sizeof(float_grande));
    memcpy(resultado->mantisa, cociente, SIZE);
    resultado->exponente = 0;

    int precision = 0;
    while (!es_cero(residuo, SIZE) && precision < MAX_ITERACIONES) {
        multiplicar_por_10(residuo, SIZE);
        subsize_t cociente_temp[SIZE] = {0};
        subsize_t residuo_temp[SIZE] = {0};

        division_booth(residuo, b, cociente_temp, residuo_temp, SIZE);
        
        multiplicar_por_10(resultado->mantisa, SIZE);
        suma_sin_suma(resultado->mantisa, cociente_temp, resultado->mantisa, SIZE);

        memcpy(residuo, residuo_temp, SIZE);
        precision++;
        resultado->exponente--;

        printf("Cociente parcial: ");
        imprimir_hex(resultado->mantisa, SIZE);
        printf("\nResiduo parcial: ");
        imprimir_hex(residuo, SIZE);
        printf("\n");
    }
    
    normalizar_float_grande(resultado);
}



int main() {

    subsize_t Big8096_1[SIZE] = {0};
    string_a_array("0xc26ec4ed1870c34dffd110454de819992258b173d5189399fcdaa205ba44effec7fa789c4616c0b2c4398da6a276c6b049fad4d5ad9aabfcc9cc2d5117ee3a2b302b213db24757630eaaeeb1922e3ed5e8ef1716a9005675da846d78ac7d1637998a659c25672038d65932e37f8c47d6b70bf42013dc71b6176f7bc97a8e35374cc5d94c827574d38a0e8371025c400030b164ada6834fffa83b05dce52b0b10ceb1450843369cffda8aaf2e77e22c7e785e132ce87c205e99fc832ac411bc7cd965f3129e08784ee64214e5a0e8eb0d5a772189ab5dc965559a245800160069ae3c42fa5aaf6c5c5065de88fd34391fb9102722f31cb90bbf340619630cc7ada552e4bddd82930f6bf7b472addb7e239575387310d27137a68256768b1b42935791e5dfe0d26244d0fbaf0e20c77bcb848ea0a7bd624aaf873cdbaf757e34bc002f243da515fc7526ecc22ea56a998e7f7389530316d1d6da0968864215194d44a618fc2f253a0991dc3d60634a48d02b887a4621771baa6a9a809f54a3701faa0152a9e97b4240f775a8210d2214831d0ac4cea1bddbd702a7aa2adbba84ffea9e03e22e810fa0b8b9dcffeffcfe96c63d765d8387e71d41f905a20011256a16430851c32523f4a125093ad2c88a151fe0ee1a22b0c0118654b0c3e30acc4e254a09b4e709246653df58471a6f8a44e11d144ef39ff6e4e4250a5375ec07672d3947298f738270b1302dc9f50cd7388bebf14e77502557a0333cab137c0b35ef9780dacb390d43fd1ad8f22e18edd49f1e02aa38919c49826785b97121c439f709e95ed70163290a49271f8f2819362b61a716ba08d9ac3ff22f2ee89548161b786c092874edc1c15426618fd09bb8c5ec26dbd4ad398a1b48fe62e66c02f514158a53c776ae423a4ac699742d3bd90114c3344704601ad6059fa3f1bc8fc8b47962d58762c876ffa11fa0fe7b6d1f73c87ffac4838c5e727399383b48df71056b0103b6a69b3639ef7dc8cfdeae9cdda4790032771a923c55b1ab70082b8a6a6860c0ce094fbd31a45c079cd525c4c8924ccf61112b2d43755c4b8d62f343674144dc407eac39c4ae2d5ce1e36417509f60cb0da69c0f8f16a008b8be6febfaf9ad807924dfdfd25c8ec36b9a41f5d20bd5bfd135d3aa1655d557957623b156c3f2c7934abb8b739dd0ad8d8c8173f6df892f7c9e640dc92cfd9b67f2f64884d1c2f47f9c4720928bdc714071f5f18b3434affd40f93e5da808cd95f271a40efa1616639a7eb0c02a2795e4eba882f81631b9d9a7a32abdf9d229d12e059b87448712f71c084cea3d56d6ed8ec4fec26bee6f7533b4809f254fd6eb0141c652a12c0eacb2b513d5074f1cbd7dff114f93414573b1d8a6c06610533fad0a488ec48dc7325034870ef985547e49ba70d5c9cfb7",
    Big8096_1, sizeof(Big8096_1) / sizeof(subsize_t));
    printf("Resultado longitud_hex: %d, en decimal: %d \n", 
    longitud_hex(Big8096_1, sizeof(Big8096_1) / sizeof(subsize_t)), len_decimal(longitud_hex(Big8096_1, sizeof(Big8096_1) / sizeof(subsize_t))));
    printf("\nn1 = 0x");
    imprimir_hex(Big8096_1, sizeof(Big8096_1) / sizeof(subsize_t));
    printf("\n");
    imprimir_decimal(Big8096_1, sizeof(Big8096_1) / sizeof(subsize_t));
    printf("\n");


    subsize_t Big8096_2[SIZE] = {0};
    string_a_array("10128885721058409803664319139594583192517508446123665050358575548361261447151826537864583512198659795755550722363825886970527917839965349362677168604417109001112679000419261177596989305717312224003400240257168405461590096090962648372132839098849101424018470574334848974705135897391209601171882787719277830629728181337101214941192220038744058677045705323371358822627339007779127280392926896362047314500907712659638730466303339198437483074060872440807651371764072907894368700856824488299066224418263056282354911962863172947531348689108682635431121748948617976166797053780421641760092226760779737386134047352798485939855834126546749333253218411827287990631341245273051120698098111586206587599691123510953377140936649661994198007857777455731865514692026439605098364563727861345333442093306457824519948525732309240603477946496512743076612987297265933525043201953423110052841800951931549100818353498815945856248584075784127140326545918221515908567191911740253846780643791067392186783939289784550707608614967354135428852288104746944089029701580211739285959326435477317502374243382619444552030498369705717686099442834471556211780802833403159072258761145577820377517499715090545729847900716761592377068031881638128400424869899446719940156094146716984341579217733508760393729575190948842842875074946947101043924824150883060691628588555764664995268180924744722378720276313492306960382498214999208649287252382458891688523580800620854432343509244245389941946377540028181049455128282568314614136199439691101562963341564550761681853104965913330169929536428120609356172663544290376899865091135486379474813610776766860405334754638377496932854732203911588874972056361170705706858664473673808303463701719234237060047876383222623049726012080706494234744381196892105280966158525441463386533784786622069725029922771032703156802531622419268917917272391116314378019528545156950527122834381742461253755086252964636735314311176824910922143749506455094366452946784396356067981000053838215494000289618508837870121541728502319376850912649512534298906660116988894173343139581123816706723742915898634441989009133499229127890803022528026220539193624315934706696091027669180289463505305576305565161175878142481534131886196388568607713549718905641968145468548545926207513163233092369957488416383764069991144040999821039940019247982347990833552080147620878772067032030011262008763081889553247652251718201322531115689807246953905062117632070412706704934413302658516776531099",
    Big8096_2, sizeof(Big8096_2) / sizeof(subsize_t));
    printf("Resultado longitud_hex: %d, en decimal: %d \n", 
    longitud_hex(Big8096_2, sizeof(Big8096_2) / sizeof(subsize_t)), len_decimal(longitud_hex(Big8096_2, sizeof(Big8096_2) / sizeof(subsize_t))));
    printf("\nn2 = 0x");
    imprimir_hex(Big8096_2, sizeof(Big8096_2) );
    printf("\n");
    imprimir_decimal(Big8096_2, sizeof(Big8096_2)  / sizeof(subsize_t));
    printf("\n");


    subsize_t resultadoBig8089[SIZE * 2] = {0};  // Donde se almacenará el resultado
    suma_sin_suma(Big8096_1, Big8096_2, resultadoBig8089, SIZE * 2);

    // Imprimir el resultado en formato hexadecimal
    printf("Resultado Suma BigInt8096: 0x");
    imprimir_hex(resultadoBig8089, sizeof(Big8096_2) / sizeof(subsize_t) );
    printf("\nResultado Suma BigInt8096: ");
    imprimir_decimal(resultadoBig8089, sizeof(Big8096_2) / sizeof(subsize_t));
    printf("\n");


    subsize_t a[SIZE] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};  // Representa 0x00000001FFFFFFFF
    subsize_t b[SIZE] = {0xFFFFFFF1, 0x00000000};  // Representa 0x0000000000FFFFFFF1
    subsize_t resultado[SIZE] = {0};  // Donde se almacenará el resultado

    suma_sin_suma(a, b, resultado, SIZE);

    printf("Resultado longitud_hex: %d, en decimal: %d \n", 
        longitud_hex(resultado, SIZE), len_decimal(longitud_hex(resultado, SIZE)));

    // Imprimir el resultado en formato hexadecimal
    printf("Resultado: 0x");
    imprimir_hex(resultado, SIZE);
    printf("\n");
    imprimir_decimal(resultado, SIZE);
    printf("\n");

    resta_sin_resta(a, b, resultado, SIZE);
    // Imprimir el resultado en formato hexadecimal
    printf("Resultado longitud_hex: %d, en decimal: %d \n", 
        longitud_hex(resultado, SIZE), len_decimal(longitud_hex(resultado, SIZE)));
    printf("Resultado: 0x");
    imprimir_hex(resultado, SIZE);
    printf("\n");
    imprimir_decimal(resultado, SIZE);
    printf("\n");

    // b = 4, e = 13 y m = 497
    subsize_t base2[SIZE] = {4, 0};         // base = 4
    subsize_t exponente2[SIZE] = {13, 0};   // exponente = 13
    subsize_t m2[SIZE] = {497, 0};          // m = 497
    subsize_t resultado2[SIZE] = {0};
    printf("Ejemplo: exponenciacion_modular\n");
    exponenciacion_modular(base2, exponente2, m2, resultado2, SIZE);
    printf("resultado: 0x");
    imprimir_hex(resultado2, SIZE);
    printf("\n");
    

    subsize_t aa = 0xFFFFFFFF;
    subsize_t ba = 0x00000001;
    subsize_t resultadoa;

    uint64_t resultado_final = suma_con_desbordamiento(aa, ba, &resultadoa);

    if (resultado_final > 0xFFFFFFFF) {
        printf("Desbordamiento detectado. Resultado completo: 0x%016llX\n", resultado_final);
    } else {
        printf("Resultado: 0x%08X\n", resultadoa);
    }

    subsize_t aaaa[SIZE] = {0xFFFFFFFF, 0x00000000};  // Representa 0x00000001FFFFFFFF
    subsize_t bbbb[SIZE] = {0xFFFFFFF1, 0x00000000};  // Representa 0x0000000000000001
    subsize_t cociente[SIZE] = {0};   // Donde almacenaremos el cociente
    subsize_t residuo[SIZE] = {0};    // Donde almacenaremos el residuo
    

    // Llamamos a la función de división
    division_booth(aaaa, bbbb, cociente, residuo, SIZE);

    // Imprimir el cociente en hexadecimal
    printf("Cociente: 0x");
    imprimir_hex(cociente, SIZE);
    printf("\n");

    // Imprimir el residuo en hexadecimal
    printf("Residuo: 0x");
    imprimir_hex(residuo, SIZE);
    printf("\n");

    float_grande resultado_div;
    division_float_grande(aaaa, bbbb, &resultado_div);
    imprimir_float_grande(&resultado_div);
    printf("\n");


    subsize_t base[SIZE] = {2, 0}; // base = 2
    subsize_t exponente[SIZE] = {11, 0};  // exponente = 10
    subsize_t resultado1[SIZE] = {0};

    printf("Ejemplo 1: Exponente chiquito (10)\n");
    exponenciacion(base, exponente, resultado1, SIZE);
    printf("resultado1: 0x");
    imprimir_hex(resultado1, SIZE);
    printf("\n");




    base[0] = 3;  // base = 3
    exponente[0] = 100;  // exponente = 100

    printf("\nEjemplo 2: Exponente grande (100)\n");
    exponenciacion(base, exponente, resultado1, SIZE);
    printf("resultado1: 0x");
    imprimir_hex(resultado1, SIZE);
    printf("\n");


    subsize_t aaa[SIZE] = {0};  // Arreglo que almacenará el número convertido

    // Ejemplo 1: Número en formato hexadecimal
    char hex_num[] = "0xFFFFFFFFFFFFFFFFF";
    string_a_array(hex_num, aaa, SIZE);
    printf("Numero en formato hexadecimal: ");
    imprimir_hex(aaa, SIZE);
    printf("\n");
    memset(aaa, 0, sizeof(subsize_t) * SIZE);
    

    // Ejemplo 2: Número en formato decimal
    char dec_num[] = "4294967295";  // 0xFFFFFFFF en decimal
    string_a_array(dec_num, aaa, SIZE);
    printf("Numero en formato decimal: ");
    imprimir_decimal(aaa, SIZE);
    printf("\n");
    imprimir_hex(aaa, SIZE);
    printf("\n");

    float_grande num1, num2, resultadof;
    memset(&num1, 0, sizeof(float_grande));
    memset(&num2, 0, sizeof(float_grande));
    memset(&resultadof, 0, sizeof(float_grande));

    // Inicializar dos números de punto flotante grandes
    inicializar_float_grande(&num1, "2.456");
    inicializar_float_grande(&num2, "45.9");

    printf("Numero 1: ");
    imprimir_float_grande(&num1);
    printf("\n");

    printf("Numero 2: ");
    imprimir_float_grande(&num2);
    printf("\n");

    // Sumar los dos números
    sumar_float_grande(&num1, &num2, &resultadof);
    printf("Suma: ");
    imprimir_float_grande(&resultadof);
    printf("\n");

    // Multiplicar los dos números
    multiplicar_float_grande(&num1, &num2, &resultadof);
    printf("Multiplicacion: ");
    imprimir_float_grande(&resultadof);
    printf("\n");

    // Dividir num1 entre num2
    dividir_float_grande(&num1, &num2, &resultadof);
    printf("Division (num1 / num2): ");
    imprimir_float_grande(&resultadof);
    printf("\n");

    // Probar con un número negativo
    inicializar_float_grande(&num1, "-34.23456789");
    printf("Numero negativo: ");
    imprimir_float_grande(&num1);
    printf("\n");


    return 0;
}
