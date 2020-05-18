#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>
#include <sys/time.h>
#include "myReadkey.h"
#include "sat.h"





/*overflow : 1;
  zero_devision : 1;
  out_bounds_memory : 1;
  ignoring_clock_pulses : 1;
  incorrect_command : 1;*/
typedef struct RegisterOfFlags {
  uint8_t overflow : 1;
  uint8_t zero_devision : 1;
  uint8_t out_bounds_memory : 1;
  uint8_t ignoring_clock_pulses : 1;
  uint8_t incorrect_command : 1;
};

enum enumRegistrOfFlags {
  overflow = 1,
  zero_devision,
  out_bounds_memory,
  ignoring_clock_pulses,
  incorrect_command
};

/*OUT_BOUNDS : 1; // �������� ������� �� ������ ���� 1
  recognize_commands : 1;
  opcode : 7;
  operand : 7;
*/
/*struct Cell {
  uint16_t OUT_BOUNDS : 1;  // �������� ������� �� ������ ���� 1
  uint16_t recognize_commands : 1;
  uint16_t opcode : 7;
  uint16_t operand : 7;

  // constructor
  Cell() {
    recognize_commands = 0;
    opcode = 0;
    operand = 0;
  }
};*/
typedef int16_t Cell;

constexpr auto NUMBERS_OF_CELLS = 100;
constexpr auto SIZE_CELL = sizeof(Cell);
extern uint64_t instructionCounter = 0; // TODO: ���������, ���� ������
extern uint64_t accumulator = 0;

uint8_t bit1 = 0x01;  // ������������ ������� ��� 0000 0001
uint8_t bit2 = 0x02;  // ������������ ������� ��� 0000 0010
uint8_t bit3 = 0x04;  // ������������ ������� ��� 0000 0100
uint8_t bit4 = 0x08;  // ������������ ������� ��� 0000 1000
uint8_t bit5 = 0x10;  // ������������ ������� ��� 0001 0000
uint8_t bit6 = 0x20;  // ������������ ������� ��� 0010 0000
uint8_t bit7 = 0x40;  // ������������ ������� ��� 0100 0100
uint8_t bit8 = 0x80;  // ������������ ������� ��� 1000 0000

uint16_t bit9 = 0x100;  // ����������������� ������� ��� 0000 0001 0000 0000
uint16_t bit10 = 0x200;  // ����������������� ������� ��� 0000 0010 0000 0000
uint16_t bit11 = 0x400;  // ����������������� ������� ��� 0000 0100 0000 0000
uint16_t bit12 = 0x800;  // ����������������� ������� ��� 0000 1000 0000 0000
uint16_t bit13 = 0x1000;  // ����������������� ������� ��� 0001 0000 0000 0000
uint16_t bit14 = 0x2000;  // ����������������� ������� ��� 0010 0000 0000 0000
uint16_t bit15 = 0x4000;  // ����������������� ������� ��� 0100 0000 0000 0000
uint16_t bit16 = 0x8000;  // ����������������� ������� ��� 1000 0000 0000 0000

// ����������������� ������� ��� 1000 0000 0000 0000
uint16_t bit_OUT_BOUNDS = bit16;  

// ����������������� ������� ��� 0100 0000 0000 0000
uint16_t bit_recognize_comand = bit15;  

// ����������������� ������� ��� 0011 1111 1000 0000
uint16_t bit_opcode = bit14 + bit13 + bit12 + bit11 + bit10 + bit9 + bit8; 

 // ����������������� ������� ��� 0000 0000 0111 1111
uint8_t bit_operand = bit7 + bit6 + bit5 + bit4 + bit3 + bit2 + bit1;  


void sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int* value);
int sc_memorySave(const char* filename);
int sc_memoryLoad(const char* filename);
int sc_regInit(void);
int sc_regSet(enumRegistrOfFlags nReg, int value);
int sc_regGet(enumRegistrOfFlags nReg, int* value);
int sc_commandEncode(int command, int operand, Cell* cell);
int sc_commandDecode(Cell cell, int* command, int* operand);

std::vector<Cell*> memory(NUMBERS_OF_CELLS);
RegisterOfFlags registrOfFlags;


/*�������������� ����������� ������ Simple Computer,
������� ���� � ������� ������� ��������.
� �������� ������������ ������ ������������ ������
����� �����, ������������ ���������� � ������ ����������.
������ ������� ����� 100 ���������.*/
void sc_memoryInit() {
  for (size_t i = 0; i < NUMBERS_OF_CELLS; i++) {
    memory.at(i) = new Cell;
    *memory.at(i) = 0;
  }
}

/*������ �������� ��������� ������ ������ ��� value.
���� ����� ������� �� ���������� �������, ��
��������������� ���� ������ �� ������� ������
� ������ ������� ������������ � �������;*/
int sc_memorySet(int address, int value) {
  if (address >= NUMBERS_OF_CELLS) {
    registrOfFlags.out_bounds_memory = 1;
    return 123;  //���������� ����� ����������
  }
  *memory.at(address) = value;
  
  return 0;
}

/*���������� �������� ��������� ������ ������ � value.
���� ����� ������� �� ���������� �������, ��
��������������� ���� ������ �� ������� ������
� ������ ������� ������������ � �������.
�������� value � ���� ������ �� ����������.*/
int sc_memoryGet(int address, int* value) {
  if (address >= NUMBERS_OF_CELLS) {
    registrOfFlags.out_bounds_memory = 1;
    return 123;  //���������� ����� ����������
  }

  *value = *memory.at(address);
  return 0;
}

/*��������� ���������� ������ � ����
� �������� ����*/
int sc_memorySave(const char* filename) {
  std::ofstream fileOut(filename, std::ios::binary);

  if (!fileOut.is_open()) {
    std::cout << "file not found";
    return 123;
  }

  for (auto it = memory.begin(); it != memory.end(); it++) {
    fileOut.write((char*)(*it), SIZE_CELL);
  }

  fileOut.close();
  return 0;
}

/*��������� �� ���������� ����� ����������
    ����������� ������*/
int sc_memoryLoad(const char* filename) {
  
  std::ifstream fil;
  fil.open("sat.o", std::ios::binary);  // ������� ���� ��� ������
  if (!fil.is_open()) {
    std::cout << "file not found";
    return 123;
  }

  com temp;
  while (fil.read((char*)(&temp), sizeof(com))){
    //*memory.at(temp.n) = temp.operand;  
    sc_commandEncode(temp.command, temp.operand, memory.at(temp.n));
  }
  fil.close();
  return 0;
}

int sc_regInit(void) {
  registrOfFlags.overflow = 0;
  registrOfFlags.zero_devision = 0;
  registrOfFlags.out_bounds_memory = 0;
  registrOfFlags.ignoring_clock_pulses = 0;
  registrOfFlags.incorrect_command = 0;

  return 0;
}

/*������������� �������� ���������� �������� ������.
��� ������� ��������� ������ ������ �������������� �����,
���������� ���������(#define).���� ������ ������������
����� �������� ��� ������������ ��������, �� �������
����������� � �������.*/
int sc_regSet(enumRegistrOfFlags nReg, int value) {
  if (nReg > 5 || nReg < 1 || value > 1 || value < 0)
    if (nReg == 1) {
    registrOfFlags.overflow = value;
  } else if (nReg == 2) {
    registrOfFlags.zero_devision = value;
  } else if (nReg == 3) {
    registrOfFlags.out_bounds_memory = value;
  } else if (nReg == 4) {
    registrOfFlags.ignoring_clock_pulses = value;
  } else if (nReg == 5) {
    registrOfFlags.incorrect_command = value;
  } else if (nReg > 5 || nReg < 1 || value > 1 || value < 0) {
    return 123;
  }
  return 0;
}

/*���������� �������� ����������
      �����.���� ������ ������������ ����� ��������,
      �� ������� ����������� � �������.*/
int sc_regGet(enumRegistrOfFlags nReg, int* value) {
  if (nReg == 1) {
    *value = registrOfFlags.overflow;
  } else if (nReg == 2) {
    *value = registrOfFlags.zero_devision;
  } else if (nReg == 3) {
    *value = registrOfFlags.out_bounds_memory;
  } else if (nReg == 4) {
    *value = registrOfFlags.ignoring_clock_pulses;
  } else if (nReg == 5) {
    *value = registrOfFlags.incorrect_command;
  } else if (nReg > 5 || nReg < 1) {
    return 123;
  }
  return 0;
}

/*�������� ������� � ��������� ������� � ���������
� �������� ��������� � value.���� �������
������������ �������� ��� ������� ��� ��������,
�� ������� ����������� � �������.� ���� ������
�������� value �� ����������*/
int sc_commandEncode(int command, int operand, Cell* cell) {
  if(command == 80){ // '='
  	*cell = operand * -1;
  } else{
  *cell = ~bit_recognize_comand;
  *cell &= command;
  *cell &= operand;
  }
  return 0;
}

/*���������� �������� ��� ������� Simple Computer.
���� ������������� ����������, �� ��������������� ����
���������� ������� � ������� ����������� � �������*/
int sc_commandDecode(Cell cell, int* command, int* operand) {
  *command = cell & bit_opcode;
  *operand = cell & bit_operand;
  if (cell & bit_recognize_comand == 1){
    registrOfFlags.incorrect_command = 1;
    registrOfFlags.ignoring_clock_pulses = 1;
    return -1;
  }
  return 0;
}






