#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
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

int main(void) {

  FILE *file_input = fopen("mem_in.txt", "r");
  if (file_input != NULL) {
    printf("file opened\n");
    int value;
    int i = 0;
    /*while (fscanf(file_input, "%02X", &value) != EOF) {
      index += 1;
      printf("%d\n", index);
      computer.MEM[index - 1] = value;
    }*/
    for (; i < MEMORY_SIZE; i++) {
      fscanf(file_input, "%02X", &value);
      computer.MEM[i] = value;
    }
  }

  while (computer.PC < MEMORY_SIZE && (computer.IR) != HALT_OPCODE) {
    fetchNextInstruction();
    printf("executing instruction: %02X at PC: %04X\n", computer.IR,
           computer.PC);
    executeInstruction();
  }

  FILE *file_output;
  file_output = fopen("mem_out.txt", "w");

  for (unsigned int i = 0; i < MEMORY_SIZE;) {
    for (int j = 0; j < 16; j++) {
      fprintf(file_output, "%02X ", (computer.MEM[i]));
      i++;
    }
    fprintf(file_output, "\n");
  }
  fclose(file_output);
  return 0;
}

void fetchNextInstruction(void) {
  computer.IR = computer.MEM[computer.PC];
  computer.CURRENT_INSTRUCTION_ADDR = computer.PC;
  computer.PC++;
  if (computer.IR & 0x80) {
    switch (computer.IR & 0x0c) { // DST of instruction
    case 0x00:
      switch (computer.IR & 0x03) {
      case 2:
        computer.PC++;
        break;
      case 3:
        computer.PC += 2;
        break;
      default:
        break;
      }
      break;
    case 0x04:
      switch (computer.IR & 0x03) {
      case 2:
        computer.PC++;
        break;
      case 3:
        computer.PC += 2;
        break;
      default:
        break;
      }
      break;
    case 0x08:
      switch (computer.IR & 0x03) {
      case 2:
      case 3:
        computer.PC += 2;
        break;
      default:
        break;
      }
      break;
    case 0x0c: // DST
      switch (computer.IR & 0x03) {
      case 0:
      case 1:
        computer.PC += 2;
        break;
      case 2:
        computer.PC += 3;
        break;
      case 3:
        computer.PC += 4;
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  } else if ((computer.IR & 0xf0) == 0) {
    switch (computer.IR & 0x7) {
    case 0:
    case 4:
    case 5:
      computer.PC += 2;
      break;
    case 1:
      computer.PC++;
    default:
      break;
    }

  } else if ((computer.IR & 0xf8) == 0x10) {
    computer.PC += 2;
  }
  computer.PC &= 0xffff;
}
void executeInstruction(void) {
  u_int16_t addr = getAddr();
  if ((computer.IR & 0x80) == 0x80) {
    int destination;
    int source;

    switch (computer.IR & 0x0c) {
    case 0x0:
      destination = computer.MEM[computer.MAR];
      break;
    case 0x4:
      destination = computer.ACC;
      break;
    case 0x8:
      destination = computer.MAR;
      break;
    case 0xc:
      destination = getAddr();
    }
    switch (computer.IR & 0x03) {
    case 0x0:
      source = computer.MEM[computer.MAR];
      break;
    case 0x1:
      source = computer.ACC;
      break;
    case 0x2:
      if ((computer.IR & 0x0c) == 0x8) {
        source = getImmediate();

      } else {
        source = getImmediateSmall();
      }
      break;
    case 0x3:
      if ((computer.IR & 0x0c) == 0x8) {
        addr = getAddr();
        source = ((computer.MEM[addr] << 8) + computer.MEM[addr + 1]);

      } else {
        source = getAddr();
      }
      break;
    }
    switch (computer.IR & 0x70) {
    case 0x00:
      destination &= source;
      break;
    case 0x10:
      destination |= source;
      break;
    case 0x20:
      destination ^= source;
      break;
    case 0x30:
      destination += source;
      break;
    case 0x40:
      destination -= source;
      break;
    case 0x50:
      destination++;
      break;
    case 0x60:
      destination--;
      break;
    case 0x70:
      destination = !destination;
      break;
    default:
      break;
    }

    switch (computer.IR & 0xc) {
    case 0x0:
      computer.MEM[computer.MAR] = destination & 0xff;
      break;
    case 0x4:
      computer.ACC = destination & 0xff;
      break;
    case 0x8:
      computer.MAR = destination & 0xffff;
      break;
    case 0xc:
      computer.MEM[getAddr()] = destination & 0xff;
      break;
    }
  } else if ((computer.IR & 0xf0) == 0) {
    if (computer.IR & 0x08) {
      if ((computer.IR & 0x04) == 0) {
        switch (computer.IR & 0x03) {
        case 0:
          computer.MEM[getAddr()] = computer.ACC;
          break;
        case 2:
          computer.MEM[computer.MAR] = computer.ACC;
        default:
          break;
        }
      } else {
        switch (computer.IR & 0x03) {
        case 0:
          computer.MEM[getAddr()] = (computer.MAR >> 8) & 0xff;
          computer.MEM[getAddr() + 1] = computer.MAR & 0xff;
          break;
        case 2:
          computer.MEM[computer.MAR] = (computer.MAR >> 8) & 0xff;
          computer.MEM[computer.MAR + 1] = computer.MAR & 0xff;
          break;
        default:
          break;
        }
      }
    } else {
      if ((computer.IR & 0x04) == 0) {
        switch (computer.IR & 0x03) {
        case 0:
          computer.ACC = computer.MEM[getAddr()];
          break;
        case 1:
          computer.ACC = computer.MEM[computer.CURRENT_INSTRUCTION_ADDR + 1];
          break;
        case 2:
          computer.ACC = computer.MEM[computer.MAR];
          break;
        default:
          break;
        }
      } else {
        int mar = computer.MAR;
        switch (computer.IR & 0x03) {
        case 0:
          computer.MAR = computer.MEM[getAddr()];
          computer.MAR <<= 8;
          computer.MAR += computer.MEM[getAddr() + 1];
          break;
        case 1:
          computer.MAR = computer.MEM[computer.CURRENT_INSTRUCTION_ADDR + 1];
          computer.MAR <<= 8;
          computer.MAR += computer.MEM[computer.CURRENT_INSTRUCTION_ADDR + 2];
          break;
        case 2:
          computer.MAR = computer.MEM[mar];
          computer.MAR <<= 8;
          computer.MAR += computer.MEM[mar + 1];
          break;
        default:
          break;
        }
      }
    }
  } else if ((computer.IR & 0xF8) == 0x10) {
    addr = getAddr();
    switch (computer.IR & 0x07) {
    case 0:
      computer.PC = addr;
      break;
    case 1:
      if (computer.ACC == 0)
        computer.PC = addr;
      break;
    case 2:
      if (computer.ACC != 0)
        computer.PC = addr;
      break;
    case 3:
      if ((computer.ACC & 0x80) != 0)
        computer.PC = addr;
      break;
    case 4:
      if (((computer.ACC & 0x80) != 0) || (computer.ACC == 0))
        computer.PC = addr;
      break;
    case 5:
      if (((computer.ACC & 0x80) == 0) && (computer.ACC != 0))
        computer.PC = addr;
      break;
    case 6:
      if ((computer.ACC & 0x80) == 0)
        computer.PC = addr;
      break;
    default:
      break;
    }
  } else {
  }
}

u_int16_t getAddr() {
  Opcode_addr *addrIns =
      (Opcode_addr *)(computer.MEM + computer.CURRENT_INSTRUCTION_ADDR);
  return addrIns->ADDR;
}
u_int16_t getImmediate() {
  Opcode_immediate_data_double *addrIns =
      (Opcode_immediate_data_double *)(computer.MEM + computer.PC);
  return addrIns->IMMED_DATA;
}
u_int8_t getImmediateSmall() {
  Opcode_immediate_data *addrIns =
      (Opcode_immediate_data *)(computer.MEM + computer.PC);
  return addrIns->IMMED_DATA;
}
