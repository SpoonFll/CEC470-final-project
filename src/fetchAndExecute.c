#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define MEMORY_SIZE 65536
#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

struct Machine {
  u_int16_t PC;
  u_int16_t CURRENT_INSTRUCTION_ADDR;
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
u_int16_t getAddr(void);
u_int16_t getImmediate(void);
u_int8_t getImmediateSmall(void);

int main(void) {}

void fetchNextInstruction(void) {
  computer.IR = computer.MEM[computer.PC];
  computer.CURRENT_INSTRUCTION_ADDR = computer.PC;
  computer.PC++;
  if (computer.IR & 0x80) {
    switch (computer.IR & 0x0c) { // DST of instruction
    case 0x00:
      break;
    case 0x04:
      break;
    case 0x08:
      break;
    case 0x0c: // DST
      break;
    default:
      break;
    }
  } else if ((computer.IR & 0xf0) == 0) {
    switch (computer.IR & 0x7) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    default:
      break;
    }
  } else if ((computer.IR & 0xf8) == 0x10) {
    computer.PC += 2;
  }
}
void executeInstruction(void) {
  u_int16_t addr = getAddr();
  if ((computer.IR & 0xF8) == 0x10) {
  }
  if (computer.IR == HALT_OPCODE) {
    return;
  }
}
u_int16_t getAddr() {
  Opcode_addr *addrIns =
      (Opcode_addr *)(computer.MEM + computer.CURRENT_INSTRUCTION_ADDR);
  return addrIns->ADDR;
}
u_int16_t getImmediate() {
  Opcode_immediate_data_double *addrIns =
      (Opcode_immediate_data_double *)(computer.MEM +
                                       computer.CURRENT_INSTRUCTION_ADDR);
  return addrIns->IMMED_DATA;
}
u_int8_t getImmediateSmall() {
  Opcode_immediate_data *addrIns =
      (Opcode_immediate_data *)(computer.MEM +
                                computer.CURRENT_INSTRUCTION_ADDR);
  return addrIns->IMMED_DATA;
}
