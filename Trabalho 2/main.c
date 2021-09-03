/**************************************************************************
  Organização e Arquitetura de Computadores - Turma C
  TRABALHO 2
  Aluno(a): Júlia Yuri Garcia Baba
  Matrícula: 190057921

  IDE : Visual Studios Code
  Compilador: Rplit
  Sistema operacional : Windows 10

 ***************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "funcoes.h"

#define set_bit(word, index, val) ((word & ~(1 << index)) | ((val&1) << index))

#define MEM_SIZE 4096 
int32_t mem[MEM_SIZE];

uint32_t    opcode,                    // codigo da operacao
            rs1,                    // indice registrador rs
            rs2,                    // indice registrador rt
            rd,                        // indice registrador rd
            shamt,                    // deslocamento
            funct3,                    // campos auxiliares
            funct7;                    // constante instrucao tipo J

uint32_t pc = 0,                        // contador de programa
         ri = 0,                        // registrador de intrucao
         sp = 0x3ffc,                        // stack pointe4r
         gp = 0x1800;                        // global pointer

int32_t imm;


enum OPCODES {
    LUI = 0x37,		AUIPC = 0x17,		// atribui 20 msbits
    ILType = 0x03,						// Load type
    BType = 0x63,						// branch condicional
    JAL = 0x6F,		JALR = 0x67,		// jumps
    StoreType = 0x23,					// store
    ILAType = 0x13,						// logico-aritmeticas com imediato
    RegType = 0x33,
    ECALL = 0x73
};

enum FUNCT3 {
    BEQ3=0,		BNE3=01,	BLT3=04,	BGE3=05,	BLTU3=0x06, BGEU3=07,
    LB3=0,		LH3=01,		LW3=02,		LBU3=04,	LHU3=05,
    SB3=0,		SH3=01,		SW3=02,
    ADDSUB3=0,	SLL3=01,	SLT3=02,	SLTU3=03,
    XOR3=04,	SR3=05,		OR3=06,		AND3=07,
    ADDI3=0,	ORI3=06,	SLTI3=02,	XORI3=04,	ANDI3=07,
    SLTIU3=03,	SLLI3=01,	SRI3=05
};

enum FUNCT7 {
    ADD7=0,	SUB7=0x20,	SRA7=0x20,	SRL7=0, SRLI7=0x00,	SRAI7=0x20
};

//
int32_t breg[32];
int end_of_program;
//


int32_t imm;

void fetch(){
    ri = lw(pc, 0);
    pc = pc + 4;
}


// LÊ OS ARQUIVOS
void load_mem(char *code, char *data) //antigo load_text
{
    FILE *arq;
    arq = fopen(code, "rb");
    
    int write_add = 0x0000;
    do{
        fread(mem + write_add, sizeof(int32_t), 1, arq);
        write_add++;
    }while(!feof(arq));

    fclose(arq);

    arq = fopen(data, "rb");

    write_add = 0x2000>>2;
    do{
        fread(mem + write_add, sizeof(int32_t), 1, arq);
        write_add++;
    }while(!feof(arq));

    fclose(arq);
}

void gera_imm(){
    switch(opcode){
        case LUI:
        case AUIPC:
            imm = (int32_t) ri >> 12;
            break;
        
        case JALR:
        case ILAType:
        case ILType:
            imm = (int32_t) ri >> 20;
            break;
            
        case JAL:
                        
            imm = set_bit(imm, 0, 0);

            for (int i=12; i <= 19; i++){
                imm = set_bit(imm, i, ri >> i);
            }

            imm = set_bit(imm, 11, ri >> 20);

            for (int i=1; i<=10; i++){
                imm = set_bit(imm, i, ri>>(i+20));
            }
             
            for (int i=20; i<=31; i++){
                imm = set_bit(imm, i, ri>>31);
            }

            break;
        
        case BType:

            imm = set_bit(imm, 0, 0);

            imm = set_bit(imm, 11, ri>>7);

            for (int i=1; i <= 4; i++){
                imm = set_bit(imm, i, ri >> (i+7));
            }

            for (int i=5; i <= 10; i++){
                imm = set_bit(imm, i, ri >> (i+20));
            }

            for (int i=12; i<=31; i++){
                imm = set_bit(imm, i, ri>>31);
            }

            break;

        case StoreType:

            for (int i=0; i <= 4; i++){
                imm = set_bit(imm, i, ri >> (i+7));
            }

            for (int i=5; i <= 11; i++){
                imm = set_bit(imm, i, ri >> (i+20));
            }

            for (int i=12; i<=31; i++){
                imm = set_bit(imm, i, ri>>31);
            }
            break;
    }
}

void decode(){

    opcode = ri & 0x7F;

    rd = (ri >> 7) & 0x1F;

    rs1 = (ri >> 15) & 0x1F;

    rs2 = (ri >> 20) & 0X1F;
    
    shamt = (ri >> 20) & 0x1F;

    funct3 = (ri >> 12) & 0x7;

    funct7 = (ri >> 25) & 0x7F;


    gera_imm();


}

void execute(){

//R -> ADD, SUB
    switch(opcode){
        case RegType:
            switch(funct3){
                case ADDSUB3:
                    switch(funct7){
                        case ADD7:
                            breg[rd]=breg[rs1]+breg[rs2];
                            break;
                        case SUB7:
                            breg[rd]=breg[rs1]-breg[rs2];
                            break;
                    }
                    break;
            }
            break;
    }

//R -> SRL, SRA
    switch (opcode){
        case RegType:
            switch(funct3){
                case SR3:
                    switch(funct7){
                        case SRL7:
                            breg[rd] = ((uint32_t)breg[rs1]) >> ((uint32_t)breg[rs2]); 
                            break;
                        case SRA7:
                            breg[rd] = breg[rs1] >> breg[rs2]; 
                            break;
                    }
                break;
            }
        break;
    }

//S -> SLL, SLT, SLTU, XOR, OR, AND
    switch(opcode){
        case RegType:
            switch(funct3){
                case SLL3:
                    breg[rd] = breg[rs1] << breg[rs2];
                    break;
                case SLT3:
                    breg[rd] = breg[rs1] < breg[rs2];
                    break;
                case SLTU3:
                    breg[rd] = ((uint32_t)breg[rs1]) < ((uint32_t) breg[rs2]);
                    break;
                case XOR3:
                    breg[rd] = breg[rs1] ^ breg[rs2];
                    break;
                case OR3:
                    breg[rd] = breg[rs1] | breg[rs2];
                    break;
                case AND3:
                    breg[rd] = breg[rs1] & breg[rs2];
                    break;
            }
            break;
    }

//S -> RLI, RAI
    switch(opcode){
        case ILAType:
            switch(funct3){
                case SR3:
                    switch(funct7){
                        case SRLI7:
                            breg[rd] = ((uint32_t)breg[rs1]) >> shamt;
                            break;
                        case SRAI7:
                            breg[rd] = breg[rs1] >> shamt;
                            break;
                    }
                break;
            }
            break;
    }

//S -> ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI
    switch(opcode){
        case ILAType:
            switch(funct3){
                case ADDI3:
                    breg[rd] = breg[rs1] + imm; 
                    break;
                case SLTI3:
                    breg[rd] = breg[rs1] < imm;
                    break;
                case SLTIU3:
                    breg[rd] = ((uint32_t)breg[rs1]) < ((uint32_t)imm);
                    break;
                case XORI3:
                    breg[rd] = breg[rs1] ^ imm;
                    break;
                case ORI3:
                    breg[rd] = breg[rs1] | imm;
                    break;
                case ANDI3:
                    breg[rd] = breg[rs1] & imm;
                    break;
                case SLLI3:
                    breg[rd] = breg[rs1] << shamt;
                    break;
            }
            break;
    }

//S -> SB, SH, SW
    switch(opcode){
        case StoreType:
            switch(funct3){
                case SB3:
                    sb(breg[rs1], imm, breg[rs2]);
                    break;
                case SW3:
                    sw(breg[rs1], imm, breg[rs2]);
                    break;
                }
            break;
    }

//B -> BEQ, BNEM BLT, BGE, BLTU, BGEU
    switch(opcode){
        case BType:
            switch(funct3){
                case BEQ3:
                    if (breg[rs1] == breg[rs2]) pc += imm - 4;
                    break;
                case BNE3:
                    if (breg[rs1] != breg[rs2]) pc += imm - 4;
                    break;
                case BLT3:
                    if (breg[rs1] < breg[rs2]) pc += imm - 4;
                    break;
                case BGE3:
                    if (breg[rs1] >= breg[rs2]) pc += imm - 4;
                    break;
                case BLTU3:
                    if ((uint32_t)breg[rs1] < (uint32_t)breg[rs2]) pc += (imm) - 4;
                    break;
                case BGEU3:
                    if ((uint32_t)breg[rs1] > (uint32_t)breg[rs2]) pc += (imm) - 4;
            }
            break;
    }

//JAL
    switch(opcode){
        case JAL:
            breg[rd] = pc; 
            pc += imm - 4;
            break;
    }

//JALR
    switch(opcode){
        case JALR:
            breg[rd] = pc; 
            pc = (breg[rs1]+imm)&(~1);
            break;
    }

//LUI
    switch(opcode){
        case LUI: 
            breg[rd] = imm << 12;
            break;
    }

//AUIPC
    switch(opcode){
        case AUIPC:
            breg[rd] = (pc-4) + (imm << 12);
            break;
    }

//ECALL
    switch(opcode){
        case ECALL:
            switch(breg[17]){
                case 1:
                    printf("%d", breg[10]);
                    break;

                case 4: ;
                    char * string = (char *) mem;
                    printf("%s", &string[breg[10]]);
                    break;

                case 10:
                    end_of_program = 1;
                    break;
            }
    }

//LB, LW, LBU
    switch(opcode){
        case ILType:
            switch(funct3){
                case LB3:
                    breg[rd] = lb(breg[rs1], imm);
                    break;
                case LW3:
                    breg[rd] = lw(breg[rs1], imm);
                    break;
                case LBU3:
                    breg[rd] = lbu(breg[rs1], imm);
                    break;
            }
            break;
    }
    breg[0] = 0;
}

void dump_mem(int start, int end, char format){
    int i;
    start /= 4;
    end /= 4;
    if((format == 'h') | (format == 'H')){
        for (i = start; i <= end; i++){
            printf("mem[%d] = %08X\n", i, mem[i]);
        }
    }
    if((format == 'd') | (format == 'D')){
        for (i = start; i <= end; i++){
            printf("mem[%d] = %d\n", i, mem[i]);
        }
    }
}

void dump_reg(char format){
    int i;
    for (i = 0; i < 32; i++)
    {
        printf(((format == 'h') | (format == 'H')) ? "$%d = %08X\n" : "$%d = %d\n", i, breg[i]);
    }
    printf(((format == 'h') | (format == 'H')) ? "pc = %08X\n" : "pc = %d\n", pc);
    printf(((format == 'h') | (format == 'H')) ? "ri = %08X\n" : "ri = %d\n", ri);
    printf(((format == 'h') | (format == 'H')) ? "sp = %08X\n" : "sp = %d\n", sp);
    printf(((format == 'h') | (format == 'H')) ? "gp = %08X\n" : "gp = %d\n", gp);
}


void step(){
    fetch();
    decode();
    execute();
}

void run(){
  int flag = 0;
    while(1){
        step();
        if(end_of_program || pc == 8000){
            break;
        }
    }
}

int main(){
    load_mem("code", "data");
    run();
}

