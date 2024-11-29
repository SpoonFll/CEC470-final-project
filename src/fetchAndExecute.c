#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define MEMORY_SIZE 65536
#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

struct Machine {
  u_int16_t PC;
  u_int8_t IR;
  u_int16_t MAR;
  u_int8_t ACC;
  u_int8_t MEM[MEMORY_SIZE];
} computer;

typedef struct OPCODE_ONLY {
  u_int8_t OPCODE;
} Opcode_no_values;

typedef struct OPCODE_IMMEDIATE {
  u_int8_t OPCODE;
  u_int8_t IMMED_DATA;
} Opcode_immediate_data;

typedef struct OPCODE_IMMEDIATE_BIG {
  u_int8_t OPCODE;
  u_int16_t IMMED_DATA;
} Opcode_immediate_data_double;

typedef struct OPCODE_ADDR {
  u_int8_t OPCODE;
  u_int16_t ADDR;
} Opcode_addr;

void fetchNextInstruction(void);
void executeInstruction(void);

int main(void) {}

void fetchNextInstruction(void) {
  computer.IR = computer.MEM[computer.PC];
  computer.PC++;
}
void executeInstruction(void) {}
