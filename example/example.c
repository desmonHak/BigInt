#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"

int main(int argc, char *argv[]) {
    #define SIZE_BIGINT_8096 9000 / LEN_BITS_SUBSIZE_T

    BigInt_t BigInt8096_1 = {calloc(SIZE_BIGINT_8096, sizeof(subsize_t)), SIZE_BIGINT_8096};
    BigInt_t BigInt8096_2 = {calloc(SIZE_BIGINT_8096, sizeof(subsize_t)), SIZE_BIGINT_8096};
    BigInt_t resultadoBigInt8096 = {calloc(SIZE_BIGINT_8096, sizeof(subsize_t)), SIZE_BIGINT_8096};

    of_string_to_numbre("0xc26ec4ed1870c34dffd110454de819992258b173d5189399fcdaa205ba44effec7fa789c4616c0b2c4398da6a276c6b049fad4d5ad9aabfcc9cc2d5117ee3a2b302b213db24757630eaaeeb1922e3ed5e8ef1716a9005675da846d78ac7d1637998a659c25672038d65932e37f8c47d6b70bf42013dc71b6176f7bc97a8e35374cc5d94c827574d38a0e8371025c400030b164ada6834fffa83b05dce52b0b10ceb1450843369cffda8aaf2e77e22c7e785e132ce87c205e99fc832ac411bc7cd965f3129e08784ee64214e5a0e8eb0d5a772189ab5dc965559a245800160069ae3c42fa5aaf6c5c5065de88fd34391fb9102722f31cb90bbf340619630cc7ada552e4bddd82930f6bf7b472addb7e239575387310d27137a68256768b1b42935791e5dfe0d26244d0fbaf0e20c77bcb848ea0a7bd624aaf873cdbaf757e34bc002f243da515fc7526ecc22ea56a998e7f7389530316d1d6da0968864215194d44a618fc2f253a0991dc3d60634a48d02b887a4621771baa6a9a809f54a3701faa0152a9e97b4240f775a8210d2214831d0ac4cea1bddbd702a7aa2adbba84ffea9e03e22e810fa0b8b9dcffeffcfe96c63d765d8387e71d41f905a20011256a16430851c32523f4a125093ad2c88a151fe0ee1a22b0c0118654b0c3e30acc4e254a09b4e709246653df58471a6f8a44e11d144ef39ff6e4e4250a5375ec07672d3947298f738270b1302dc9f50cd7388bebf14e77502557a0333cab137c0b35ef9780dacb390d43fd1ad8f22e18edd49f1e02aa38919c49826785b97121c439f709e95ed70163290a49271f8f2819362b61a716ba08d9ac3ff22f2ee89548161b786c092874edc1c15426618fd09bb8c5ec26dbd4ad398a1b48fe62e66c02f514158a53c776ae423a4ac699742d3bd90114c3344704601ad6059fa3f1bc8fc8b47962d58762c876ffa11fa0fe7b6d1f73c87ffac4838c5e727399383b48df71056b0103b6a69b3639ef7dc8cfdeae9cdda4790032771a923c55b1ab70082b8a6a6860c0ce094fbd31a45c079cd525c4c8924ccf61112b2d43755c4b8d62f343674144dc407eac39c4ae2d5ce1e36417509f60cb0da69c0f8f16a008b8be6febfaf9ad807924dfdfd25c8ec36b9a41f5d20bd5bfd135d3aa1655d557957623b156c3f2c7934abb8b739dd0ad8d8c8173f6df892f7c9e640dc92cfd9b67f2f64884d1c2f47f9c4720928bdc714071f5f18b3434affd40f93e5da808cd95f271a40efa1616639a7eb0c02a2795e4eba882f81631b9d9a7a32abdf9d229d12e059b87448712f71c084cea3d56d6ed8ec4fec26bee6f7533b4809f254fd6eb0141c652a12c0eacb2b513d5074f1cbd7dff114f93414573b1d8a6c06610533fad0a488ec48dc7325034870ef985547e49ba70d5c9cfb7", &BigInt8096_1);
    hex_dump_BigInt(&BigInt8096_1);
    printf("\nResultado count_hex_digit_BigInt: %d, en decimal: %d \n", 
        count_hex_digit_BigInt(&BigInt8096_1), 
        len_decimal(count_hex_digit_BigInt(&BigInt8096_1))
    );

    of_string_to_numbre("10128885721058409803664319139594583192517508446123665050358575548361261447151826537864583512198659795755550722363825886970527917839965349362677168604417109001112679000419261177596989305717312224003400240257168405461590096090962648372132839098849101424018470574334848974705135897391209601171882787719277830629728181337101214941192220038744058677045705323371358822627339007779127280392926896362047314500907712659638730466303339198437483074060872440807651371764072907894368700856824488299066224418263056282354911962863172947531348689108682635431121748948617976166797053780421641760092226760779737386134047352798485939855834126546749333253218411827287990631341245273051120698098111586206587599691123510953377140936649661994198007857777455731865514692026439605098364563727861345333442093306457824519948525732309240603477946496512743076612987297265933525043201953423110052841800951931549100818353498815945856248584075784127140326545918221515908567191911740253846780643791067392186783939289784550707608614967354135428852288104746944089029701580211739285959326435477317502374243382619444552030498369705717686099442834471556211780802833403159072258761145577820377517499715090545729847900716761592377068031881638128400424869899446719940156094146716984341579217733508760393729575190948842842875074946947101043924824150883060691628588555764664995268180924744722378720276313492306960382498214999208649287252382458891688523580800620854432343509244245389941946377540028181049455128282568314614136199439691101562963341564550761681853104965913330169929536428120609356172663544290376899865091135486379474813610776766860405334754638377496932854732203911588874972056361170705706858664473673808303463701719234237060047876383222623049726012080706494234744381196892105280966158525441463386533784786622069725029922771032703156802531622419268917917272391116314378019528545156950527122834381742461253755086252964636735314311176824910922143749506455094366452946784396356067981000053838215494000289618508837870121541728502319376850912649512534298906660116988894173343139581123816706723742915898634441989009133499229127890803022528026220539193624315934706696091027669180289463505305576305565161175878142481534131886196388568607713549718905641968145468548545926207513163233092369957488416383764069991144040999821039940019247982347990833552080147620878772067032030011262008763081889553247652251718201322531115689807246953905062117632070412706704934413302658516776531099", &BigInt8096_2);
    hex_dump_BigInt(&BigInt8096_2);
    printf("\nResultado count_hex_digit_BigInt: %d, en decimal: %d \n", 
        count_hex_digit_BigInt(&BigInt8096_2), 
        len_decimal(count_hex_digit_BigInt(&BigInt8096_2))
    );

    add_BigInt(&BigInt8096_1, &BigInt8096_2, &resultadoBigInt8096);

    printf("Resultado Suma BigInt8096: 0x");
    hex_dump_BigInt(&resultadoBigInt8096);
    printf("\nResultado Suma BigInt8096: ");
    decimal_dump_BigInt(&resultadoBigInt8096);
    printf("\n");

    #define SIZE_320bits 10  // numeros de 10 * 32 == 320 bits
    BigInt_t BigInt320_1 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};
    BigInt_t BigInt320_2 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};
    BigInt_t resultadoBigInt320 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};

    // Representa 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    for (int i = 0; i < 5; i++) {
        BigInt320_1.number[i] = 0xFFFFFFFF;
    }
    // Representa 0x0000000000FFFFFFF1
    BigInt320_2.number[0] = 0xFFFFFFF1;

    add_BigInt(&BigInt320_1, &BigInt320_2, &resultadoBigInt320);
    printf("Resultado count_hex_digit_BigInt: %d, en decimal: %d \n", 
        count_hex_digit_BigInt(&resultadoBigInt320), 
        len_decimal(count_hex_digit_BigInt(&resultadoBigInt320)));

    printf("Resultado: 0x");
    hex_dump_BigInt(&resultadoBigInt320);
    printf("\n");
    decimal_dump_BigInt(&resultadoBigInt320);
    printf("\n");

    memset(resultadoBigInt320.number, 0, sizeof(subsize_t) * SIZE_320bits);
    sub_BigInt(&BigInt320_1, &BigInt320_2, &resultadoBigInt320);
    printf("Resultado count_hex_digit_BigInt: %d, en decimal: %d \n", 
        count_hex_digit_BigInt(&resultadoBigInt320), 
        len_decimal(count_hex_digit_BigInt(&resultadoBigInt320)));
    printf("Resultado: ");
    hex_dump_BigInt(&BigInt320_1);
    printf(" - ");
    hex_dump_BigInt(&BigInt320_2);
    printf(" = ");
    hex_dump_BigInt(&resultadoBigInt320);
    printf("\n");
    decimal_dump_BigInt(&resultadoBigInt320);
    printf("\n");

    // b = 4, e = 13 y m = 497
    BigInt_t base2 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};
    BigInt_t exponente2 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};
    BigInt_t m2 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};
    BigInt_t resultado2 = {calloc(SIZE_320bits, sizeof(subsize_t)), SIZE_320bits};

    base2.number[0] = 4;
    exponente2.number[0] = 13;
    m2.number[0] = 497;

    printf("Ejemplo: exponenciacion_modular\n");
    modpow_BigInt(&base2, &exponente2, &m2, &resultado2);
    printf("resultado: 0x");
    hex_dump_BigInt(&resultado2);
    printf("\n");



    subsize_t aa = 0xFFFFFFFF;
    subsize_t ba = 0x00000001;
    subsize_t resultadoa;

    uint64_t resultado_final = add_with_overflow(aa, ba, &resultadoa);

    if (resultado_final > 0xFFFFFFFF) {
        printf("Desbordamiento detectado. Resultado completo: 0x%016llX\n", resultado_final);
    } else {
        printf("Resultado: 0x%08X\n", resultadoa);
    }


    #define SIZE 2

    // Section 1: Division Booth Test
    BigInt_t number_example1 = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t number_example2 = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t cociente_booth = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t residuo_booth = {calloc(SIZE, sizeof(subsize_t)), SIZE};

    number_example1.number[0] = 1;
    number_example2.number[0] = 3;

    // Call the division function
    div_booth(&number_example1, &number_example2, &cociente_booth, &residuo_booth);

    // Print the quotient and remainder in hexadecimal
    printf("Cociente: 0x");
    hex_dump_BigInt(&cociente_booth);
    printf("\n");

    printf("Residuo: 0x");
    hex_dump_BigInt(&residuo_booth);
    printf("\n");

    // Section 2: Float Grande Division Test
    float_grande resultado_div;
    BigInt_t number_example1_float = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t number_example2_float = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    number_example1_float.number[0] = 1;
    number_example2_float.number[0] = 3;
    resultado_div.number_float.number = calloc(SIZE, sizeof(subsize_t));
    resultado_div.number_float.size = SIZE;
    size_t desired_digits = 2; // I want 2 decimal digits
    
    div_to_float_big(&number_example1_float, &number_example2_float, &resultado_div, &desired_digits);
    printf("float__dump_BigInt: ");
    float__dump_BigInt(&resultado_div);
    printf("\n");

    // Section 3: Exponentiation Test
    BigInt_t base = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t exponente = {calloc(SIZE, sizeof(subsize_t)), SIZE};
    BigInt_t resultado1 = {calloc(SIZE, sizeof(subsize_t)), SIZE};

    base.number[0] = 2;
    exponente.number[0] = 11;

    printf("Ejemplo 1: Exponente chiquito (10)\n");
    pow_BigInt(&base, &exponente, &resultado1);  // reusing "base" as modulo
    printf("resultado1: 0x");
    hex_dump_BigInt(&resultado1);
    printf("\n");



    // Free allocated memory
    free(BigInt8096_1.number);
    free(BigInt8096_2.number);
    free(resultadoBigInt8096.number);
    free(BigInt320_1.number);
    free(BigInt320_2.number);
    free(resultadoBigInt320.number);
    free(base2.number);
    free(base.number);
    free(exponente.number);
    free(exponente2.number);
    free(m2.number);
    free(resultado2.number);
    free(resultado1.number);

    puts("\nPrograma finalizado.\n");
    return 0;
}
