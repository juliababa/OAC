/**************************************************************************
  Organização e Arquitetura de Computadores - Turma C
  TRABALHO 1: MEMÓRIA DO RISC-V
  Aluno(a): Júlia Yuri Garcia Baba
  Matrícula: 190057921

  IDE : Visual Studios Code
  Compilador: Online GDB
  Sistema operacional : Windows 10

  O primeiro trabalho consiste em simular a memória do RISC-V atráves de cinco
  funções, que estão descritas na documentação do código.
 ***************************************************************************/
#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 4096 
int32_t mem[MEM_SIZE]; 

void teste2(uint32_t address, uint32_t wSize);

//Declaração das funções
int32_t lw(uint32_t adress, int32_t kte);
int32_t lb(uint32_t address, int32_t kte); 
int32_t lbu(uint32_t address, int32_t kte);
void sw(uint32_t address, int32_t kte, int32_t dado);
void sb(uint32_t address, int32_t kte, int8_t dado);

/****************************
TESTES PROPOSTOS NO ENUNCIADO
*****************************/

void teste(){
printf("\n**TESTES PROPOSTOS**\n\n");
//Teste 1 e 2 - Inicia a memória e imprimi o conteúdo em formato hexa
    sb(0, 0, 0x04);
    sb(0, 1, 0x03);
    sb(0, 2, 0x02);
    sb(0, 3, 0x01);
    sb(4, 0, 0xFF);
    sb(4, 1, 0xFE);
    sb(4, 2, 0xFD);
    sb(4, 3, 0xFC);
    sw(12, 0, 0xFF);
    sw(16, 0, 0xFFFF);
    sw(20, 0, 0xFFFFFFFF);
    sw(24, 0, 0x80000000);

    teste2(0, 7);

//Teste 3 - Lê os dados e imprimi em hexadecimal
    printf("\n\nLOAD\n\n");
    lb(4,0);
    lb(4,1);
    lb(4,2);
    lb(4,3);

    printf("\n\nLOAD U\n\n");
    lbu(4,0);
    lbu(4,1);
    lbu(4,2);
    lbu(4,3);

    printf("\n\nLOAD WORD\n\n");
    lw(12,0);
    lw(16,0);
    lw(20,0);
}

/****************************
TESTES EXTRAS
*****************************/

void aluno_teste(){;
printf("\n**TESTE EXTRA**\n\n");

int i;
for (i = 0; i < 40; i += 4) {
        sw(i, 0, 0);
    }

//Teste semelhante ao 1 e 2 
    sb(0, 0, 0x10);
    sb(2, 0, 0x01);

    sb(0, 10, 0x01);

    sb(4, 11, 0xDF);

    sb(0, 5, 0xAB);
    sb(0, 6, 0xBC);
    sb(0, 7, 0xCD);

    sb(10, 7, 0xA7);
    sb(10, 10, 0xA7);
    sb(20, 9, 0xAC);
    sb(32, 0, 0x99);

    sw(20, 6, 0xA1B2C3D4);
    sw(36, 0, 0x679BCDEF);

    teste2(0, 10);

//Teste semelhante ao 3
    printf("\n\nLOAD\n\n");
    lb(28, 4);
    lb(20, 0);

    printf("\n\nLOAD U\n\n");
    lbu(10, 10);

    printf("\n\nLOAD WORD\n\n");
    lw(36, 0);
}

int main() {
//Chama os testes propostos nos enunciados
    teste();

//Chama os testes propostos pelo aluno 
    aluno_teste(); 
}


//Essa é uma função auxiliar feita para imprimir a saída do teste 2 da forma pedida
void teste2(uint32_t address, uint32_t nWords) {

    if ((address) % 4 != 0) {

        printf("Endereco invalido! Nao e multiplo de 4.\n");
    }
    else {

        uint8_t i;
        for (i = 0; i < nWords; i++){
            printf("mem[%d] = %08X\n", i, mem[address]);
            address += 1;
        }
    }
}

//Primeira função - Múltiplo de 4
int32_t lw(uint32_t address, int32_t kte){
        uint32_t word_address = address + kte; 

        if(word_address%4 == 0){ //Checa se o endereço é um múltiplo de 4
            int32_t data;
            int32_t *wordPointer = (int32_t *) mem; //Inicialização de um ponteiro que aponta para o início da MEM e anda de 32b em 32b

            data = wordPointer[(address + kte)/4];
            printf("-- 0x%08X\n",data);
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
    printf("-- 0x%02X\n", (uint8_t) byte);

    return byte;
}

//Terceira função - Lê um byte do vetor memória e retorna-o como um número positivo.
int32_t lbu(uint32_t address, int32_t kte){
    uint32_t byte_addres = address + kte;

    int8_t*byte_pointer = (int8_t*)mem; 
    int32_t byte = byte_pointer[byte_addres];
    printf("-- 0x%02X\n", (uint8_t) byte);
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