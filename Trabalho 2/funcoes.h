#include <stdio.h>
#include <stdint.h>

#ifndef __FUNCOES_H__
#define __FUNCOES_H__

//Declaração das funções
int32_t lw(uint32_t adress, int32_t kte);
int32_t lb(uint32_t address, int32_t kte); 
int32_t lbu(uint32_t address, int32_t kte);
void sw(uint32_t address, int32_t kte, int32_t dado);
void sb(uint32_t address, int32_t kte, int8_t dado);

#endif