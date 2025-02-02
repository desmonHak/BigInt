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


int longitud_hex(subsize_t* num, int size) {
    int longitud = 0;
    int i;

    // Encuentra el primer elemento no nulo desde la izquierda
    for (i = size - 1; i >= 0; i--) {
        if (num[i] != 0) {
            longitud = i + 1;
            break;
        }
    }

    // Si todos los elementos son cero, la longitud es 1
    if (longitud == 0) {
        return 1;
    }

    // Cuenta los dígitos hexadecimales significativos en el elemento más significativo
    subsize_t msb = num[longitud - 1];
    int digitos = 8; // Máximo 8 dígitos hexadecimales en un subsize_t
    while (msb != 0 && (msb & 0xF0000000) == 0) {
        msb <<= 4;
        digitos--;
    }

    return (longitud - 1) * 8 + digitos;
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
    int len = strlen(num_str);
    memset(arr, 0, sizeof(subsize_t) * size);

    if (len >= 2 && num_str[0] == '0' && (num_str[1] == 'x' || num_str[1] == 'X')) {
        // Convertir desde hexadecimal
        int hex_len = len - 2;  // Longitud sin "0x"
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

            if (shift == 32 || i == 2) {
                arr[arr_index++] = current_value;
                current_value = 0;
                shift = 0;
            }
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

void normalizar_float_grande(float_grande* num) {
    while (num->mantisa[SIZE-1] >= 10) {
        dividir_por_10(num->mantisa, SIZE);
        num->exponente++;
    }
    while (num->mantisa[0] == 0 && num->exponente > -100) {  // Evitar pérdida excesiva de precisión
        multiplicar_por_10(num->mantisa, SIZE);
        num->exponente--;
    }
}

void inicializar_float_grande(float_grande* num, const char* valor_str) {
    num->signo = (valor_str[0] == '-') ? 1 : 0;
    const char* start = num->signo ? valor_str + 1 : valor_str;

    char copia[1000];
    strncpy(copia, start, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    char* punto_decimal = strchr(copia, '.');
    char* exponente_str = strchr(copia, 'e');
    if (!exponente_str) exponente_str = strchr(copia, 'E');

    char mantisa_str[1000] = {0};
    int ajuste_exp = 0;

    if (punto_decimal) {
        *punto_decimal = '\0';
        strcpy(mantisa_str, copia);
        strcat(mantisa_str, punto_decimal + 1);
        ajuste_exp = strlen(punto_decimal + 1);
    } else {
        strcpy(mantisa_str, copia);
    }

    if (exponente_str) {
        *exponente_str = '\0';
        num->exponente = atoi(exponente_str + 1);
    } else {
        num->exponente = 0;
    }

    num->exponente -= ajuste_exp;
    string_a_array(mantisa_str, num->mantisa, SIZE);
    normalizar_float_grande(num);
}


bool es_mayor(subsize_t* a, subsize_t* b, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (a[i] > b[i]) return true;
        if (a[i] < b[i]) return false;
    }
    return false;  // Son iguales
}

void sumar_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    int diff_exp = a->exponente - b->exponente;
    float_grande temp_a = *a, temp_b = *b;

    if (diff_exp > 0) {
        for (int i = 0; i < diff_exp; i++) {
            dividir_por_10(temp_b.mantisa, SIZE);
        }
        temp_b.exponente = temp_a.exponente;
    } else if (diff_exp < 0) {
        for (int i = 0; i < -diff_exp; i++) {
            dividir_por_10(temp_a.mantisa, SIZE);
        }
        temp_a.exponente = temp_b.exponente;
    }

    if (temp_a.signo == temp_b.signo) {
        suma_sin_suma(temp_a.mantisa, temp_b.mantisa, resultado->mantisa, SIZE);
        resultado->signo = temp_a.signo;
    } else {
        if (es_mayor(temp_a.mantisa, temp_b.mantisa, SIZE)) {
            resta_sin_resta(temp_a.mantisa, temp_b.mantisa, resultado->mantisa, SIZE);
            resultado->signo = temp_a.signo;
        } else {
            resta_sin_resta(temp_b.mantisa, temp_a.mantisa, resultado->mantisa, SIZE);
            resultado->signo = temp_b.signo;
        }
    }
    resultado->exponente = temp_a.exponente;
    normalizar_float_grande(resultado);
}


void multiplicar_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    multiplicar_arreglos(a->mantisa, b->mantisa, resultado->mantisa, SIZE);
    resultado->exponente = a->exponente + b->exponente;
    resultado->signo = a->signo ^ b->signo;
    normalizar_float_grande(resultado);
}

void dividir_float_grande(float_grande* a, float_grande* b, float_grande* resultado) {
    subsize_t cociente[SIZE] = {0};
    subsize_t dividendo[SIZE * 2] = {0};
    subsize_t divisor[SIZE] = {0};
    
    // Copiar el dividendo y el divisor
    memcpy(dividendo + SIZE, a->mantisa, sizeof(subsize_t) * SIZE);
    memcpy(divisor, b->mantisa, sizeof(subsize_t) * SIZE);
    
    int shift = 0;
    int max_iterations = SIZE * 32; // Máximo número de iteraciones para evitar bucle infinito
    
    for (int i = 0; i < max_iterations; i++) {
        // Realizar una iteración de la división
        subsize_t bit = 0;
        if (es_mayor_o_igual(dividendo, divisor, SIZE * 2, SIZE)) {
            resta_arreglos(dividendo, divisor, dividendo, SIZE * 2, SIZE);
            bit = 1;
        }
        
        // Desplazar el cociente y añadir el nuevo bit
        for (int j = SIZE - 1; j > 0; j--) {
            cociente[j] = (cociente[j] << 1) | (cociente[j-1] >> 31);
        }
        cociente[0] = (cociente[0] << 1) | bit;
        
        // Desplazar el dividendo
        for (int j = SIZE * 2 - 1; j > 0; j--) {
            dividendo[j] = (dividendo[j] << 1) | (dividendo[j-1] >> 31);
        }
        dividendo[0] <<= 1;
        
        // Si hemos obtenido suficiente precisión, salimos del bucle
        if (!es_cero(cociente, SIZE) && i >= SIZE * 32 - 1) break;
    }
    
    memcpy(resultado->mantisa, cociente, sizeof(subsize_t) * SIZE);
    resultado->exponente = a->exponente - b->exponente;
    resultado->signo = a->signo ^ b->signo;
    normalizar_float_grande(resultado);
}

void imprimir_float_grande(float_grande* num) {
    printf("%s0.", num->signo ? "-" : "");
    imprimir_decimal(num->mantisa, SIZE);
    //printf(" x 10^%d", num->exponente);
}


bool es_mayor_o_igual(subsize_t* a, subsize_t* b, int size_a, int size_b) {
    for (int i = size_a - 1, j = size_b - 1; i >= 0 && j >= 0; i--, j--) {
        if (a[i] > b[j]) return true;
        if (a[i] < b[j]) return false;
    }
    return true;
}


void resta_arreglos(subsize_t* a, subsize_t* b, subsize_t* resultado, int size_a, int size_b) {
    int borrow = 0;
    for (int i = 0; i < size_a; i++) {
        uint64_t diff = (uint64_t)a[i] - (i < size_b ? b[i] : 0) - borrow;
        resultado[i] = (subsize_t)diff;
        borrow = (diff >> 32) & 1;
    }
}


int main() {
    subsize_t a[SIZE] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};  // Representa 0x00000001FFFFFFFF
    subsize_t b[SIZE] = {0xFFFFFFF1, 0x00000000};  // Representa 0x0000000000000001
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
    division_booth(a, b, cociente, residuo, SIZE);

    // Imprimir el cociente en hexadecimal
    printf("Cociente: 0x");
    imprimir_hex(cociente, SIZE);
    printf("\n");

    // Imprimir el residuo en hexadecimal
    printf("Residuo: 0x");
    imprimir_hex(residuo, SIZE);
    printf("\n");


    subsize_t base[SIZE] = {2, 0}; // base = 2
    subsize_t exponente[SIZE] = {11, 0};  // exponente = 10
    subsize_t resultado1[SIZE] = {0};

    printf("Ejemplo 1: Exponente chiquito (10)\n");
    exponenciacion(base, exponente, resultado1, SIZE);
    printf("resultado1: 0x");
    imprimir_hex(resultado1, SIZE);


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

    // Inicializar dos números de punto flotante grandes
    inicializar_float_grande(&num1, "-0.4");
    inicializar_float_grande(&num2, "0.3");

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
    inicializar_float_grande(&num1, "-1.23456789e-10");
    printf("Numero negativo: ");
    imprimir_float_grande(&num1);
    printf("\n");


    return 0;
}
