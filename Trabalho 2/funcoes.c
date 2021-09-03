
#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 4096 
int32_t mem[MEM_SIZE];

int32_t lw(uint32_t address, int32_t kte){
        uint32_t word_address = address + kte; 

        if(word_address%4 == 0){ //Checa se o endereço é um múltiplo de 4
            int32_t data;
            int32_t *wordPointer = (int32_t *) mem; //Inicialização de um ponteiro que aponta para o início da MEM e anda de 32b em 32b

            data = wordPointer[(address + kte)/4];
            return data;
        }
        else{

            printf("Endereco invalido! Nao e multiplo de 4.\n"); //Mensagem de erro
            return 0; //Retorna zero
        }
}

//Segunda função - Lê um byte do vetor memória e retorna-o, estendendo o sinal para 32 bits. 
int32_t lb(uint32_t address, int32_t kte){
    uint32_t byte_addres = address + kte; 

    int8_t*byte_pointer = (int8_t*)mem; //Inicialização de um ponteiro que aponta para o início da MEM e anda de 8b em 8b
    int32_t byte = byte_pointer[byte_addres];
    

    return byte;
}

//Terceira função - Lê um byte do vetor memória e retorna-o como um número positivo.
int32_t lbu(uint32_t address, int32_t kte){
    uint32_t byte_addres = address + kte;

    uint8_t*byte_pointer = (uint8_t*)mem; 
    int32_t byte = byte_pointer[byte_addres];
    return byte;
}

//Quarta função - Escreve um inteiro alinhado na memória - endereços múltiplos de 4
void sw(uint32_t address, int32_t kte, int32_t dado){
    uint32_t word_addres = address + kte;

    if(word_addres%4==0){

        word_addres /= 4;  
        mem[word_addres] = dado; 
    }
    else{
        printf("Endereco invalido! Nao e multiplo de 4.\n"); 
    }
}

//Quinta função - Escreve um byte na memória. 
void sb(uint32_t address, int32_t kte, int8_t dado){
    uint32_t byte_addres = address + kte;

    int8_t*byte_pointer = (int8_t*)mem;
    byte_pointer[byte_addres] = dado; 
}