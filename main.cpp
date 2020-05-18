#include "Header.h"
#include "myBigChars.h"
#include <iostream>
#include <iomanip>

uint16_t code;
struct Cell_coord{
  uint8_t x = 0; // range 0 - 99
  uint8_t y = 0;
}cell_coord, temp_coord;

extern std::vector <uint64_t> num = {
0xFFC3C3C3C3C3C3FF,
0x187818181818187E,
0x3CC3030F3CF0C0FF,
0x3CC3030C0C03C33C,
0xC3C3C3FF03030303,
0xFFC0C0FF0303C3FF,
0xFFC0C0FFC3C3C3FF,
0xFF03030C30C0C0C0,
0x3CC3C33CC3C3C33C,
0xFFC3C3C3FF0303FF,
0x00003CFFFF3C0000,
0x000000FFFF000000, // TODO minus
};


void printMainTable();

void printCell(colors color, Cell_coord cell_coord){
  mt_setbgcolor(color);
  mt_gotoXY(cell_coord.y+2,(cell_coord.x)*6+2);
  if (*memory.at((cell_coord.y)*10 + cell_coord.x) >= 0){
    std::cout << "+" << std::setfill('0') << std::setw(4) 
            << *memory.at((cell_coord.y)*10 + cell_coord.x);
  } else {
    std::cout << "-" << std::setfill('0') << std::setw(4) 
            << (*memory.at((cell_coord.y)*10 + cell_coord.x)) * -1;
  }
  mt_setbgcolor (STANDART);
}

void sigHandler(int sigNum){
	int value_regGet = 0;
	sc_regGet(ignoring_clock_pulses, &value_regGet);
	
	switch(sigNum){
	  case SIGUSR1:
		registrOfFlags.ignoring_clock_pulses = 1;
		  instructionCounter = 0;
		  accumulator = 0; 
		  sc_regSet(ignoring_clock_pulses, 1);
	 	  cell_coord.x = 0;
		  cell_coord.y = 0; 
		printMainTable();
		printCell(CIAN, cell_coord);
	    break;

	  case SIGALRM:
		registrOfFlags.ignoring_clock_pulses = 0;
		if (instructionCounter < 99){
		  instructionCounter++;
		  if(cell_coord.y % 9 != 0 || cell_coord.y == 0){
		    cell_coord.y++;
  		  } else {
			cell_coord.y = 0;
		    cell_coord.x++;
		  }
          printMainTable();
		  printCell(CIAN, cell_coord);
		}
	    break;
	}
}

void printMainTable(){
  mt_setbgcolor (STANDART);
  mt_setfgcolor (STANDART);
  mt_clrscr();
  
  bc_box(1,1,61,12); //Рамка памяти
  mt_gotoXY(1,29);
  std::cout << "Memory";

  bc_box(62,1,19,3); //Рамка аккумулятора
  mt_gotoXY(1,66);
  std::cout << "Accumulator";

  bc_box(62,4,19,3); //Рамка инструкция
  mt_gotoXY(4,63);
  std::cout << "Instruction Count";  

  bc_box(62,7,19,3); //Рамка код операции
  mt_gotoXY(7,67);
  std::cout << "Operation";

  bc_box(62,10,19,3); //Рамка флагов
  mt_gotoXY(10,69);
  std::cout << "Flags";
  mt_gotoXY(11,63);
  (registrOfFlags.overflow == 1) ? std::cout << "    П " : std::cout << "    * ";
  (registrOfFlags.zero_devision == 1) ? std::cout << "0 " : std::cout << "* ";
  (registrOfFlags.out_bounds_memory == 1) ? std::cout << "M " : std::cout << "* "; 
  (registrOfFlags.ignoring_clock_pulses == 1) ? std::cout << "T " : std::cout << "* ";
  (registrOfFlags.incorrect_command == 1) ? std::cout << "E " : std::cout << "* ";
  
  bc_box(1,13,46,10); //Рамка больших чисел
  
  for (size_t i = 1; i <= 10; ++i){
    for (size_t j = 1; j <= 10; ++j){
     mt_gotoXY(i+1,(j-1)*6+2);
      Cell_coord temp;
  	  temp.x = j-1;
      temp.y = i-1;
      printCell(STANDART, temp);
    }
  }

  int16_t temp = *memory.at((cell_coord.y)*10 + cell_coord.x);
  if(temp >= 0){
    bc_printbigchar(num[10], 2, 14, CIAN, STANDART);
  }else{
	temp *= -1;
    bc_printbigchar(num[11], 2, 14, CIAN, STANDART);
  }
  bc_printbigchar(num[temp%10], 38, 14, CIAN, STANDART);
  temp /= 10;
  bc_printbigchar(num[temp%10], 29, 14, CIAN, STANDART);
  temp /= 10;
  bc_printbigchar(num[temp%10], 20, 14, CIAN, STANDART);
  temp /= 10;
  bc_printbigchar(num[temp%10], 11, 14, CIAN, STANDART);
  mt_setbgcolor (STANDART);
  mt_setfgcolor (STANDART);

  bc_box(47,13,40,10); //Рамка клавиш
  mt_gotoXY(13,62);
  std::cout << "Keys";
  mt_gotoXY(14,48);
  std::cout << "l - load";
  mt_gotoXY(15,48);
  std::cout << "s - save";
  mt_gotoXY(16,48);
  std::cout << "r - run";
  mt_gotoXY(17,48);
  std::cout << "t - step";
  mt_gotoXY(18,48);
  std::cout << "i - reset";
  mt_gotoXY(19,48);
  std::cout << "F5 - accumulator";
  mt_gotoXY(20,48);
  std::cout << "F6 - instruction counter";
}

int ALU (int opcode, int operand){

  return 0;
}

int CU (void){
int command, operand;
  sc_commandDecode(*memory.at(instructionCounter), &command, &operand);
  ALU(command, operand);
}


////////////////////////////// main ///////////////////////////////

int main() {
  registrOfFlags.ignoring_clock_pulses = 1;

  sc_memoryInit();

  sc_memoryLoad("sat.o");
  printMainTable();

  mt_gotoXY(23,1);
  printCell(CIAN, cell_coord);
  mt_setbgcolor (STANDART);
  mt_setfgcolor (STANDART);
  
uint64_t KEY = 0;  
while(KEY != enter){

    signal (SIGUSR1, sigHandler);
    signal (SIGALRM, sigHandler);
    struct itimerval beginVal, endVal;
    mt_gotoXY(5,68);
    std::cout << instructionCounter;

    lseek(fileno(stdin), SEEK_SET, 0);
    rk_readkey(&KEY);
    
    switch (KEY) {
      case up: if(cell_coord.y > 0){
	    	   printCell(STANDART, cell_coord);
        	   cell_coord.y--;
	           printCell(CIAN, cell_coord); }
      break;

      case down: if(cell_coord.y < 9){
	    		 printCell(STANDART, cell_coord);
        		 cell_coord.y++;
	    		 printCell(CIAN, cell_coord); }
	  break;

      case ::left: if(cell_coord.x > 0){
	    		 printCell(STANDART, cell_coord);
        		 cell_coord.x--;
	    		 printCell(CIAN, cell_coord); }
	  break;

      case ::right: if(cell_coord.x < 9){
	    		  printCell(STANDART, cell_coord);
        		  cell_coord.x++;
	    		  printCell(CIAN, cell_coord); }
	  break;

      case load:
	  	rk_mytermrestore();
	  	break;
        
      case save:
	  	rk_mytermsave();
      	break;

	  case run:
      	beginVal.it_value.tv_sec = 0;
		beginVal.it_value.tv_usec = 0;
		beginVal.it_interval.tv_sec = 0;
		beginVal.it_interval.tv_usec = 0;
		setitimer (ITIMER_REAL, &beginVal, &endVal);
		raise(SIGUSR1);
		break;

      case reset:
      	beginVal.it_value.tv_sec = 2;
		beginVal.it_value.tv_usec = 500;
		beginVal.it_interval.tv_sec = 2;
		beginVal.it_interval.tv_usec = 500;
		setitimer (ITIMER_REAL, &beginVal, &endVal);
		break;

	  case step:
		if(instructionCounter < 99){
		  instructionCounter++;
          printCell(STANDART, cell_coord);
		  if(cell_coord.y % 9 != 0 || cell_coord.y == 0){
		    cell_coord.y++;
  		  } else {
			cell_coord.y = 0;
		    cell_coord.x++;
		  }
		  printCell(CIAN, cell_coord);
		}

      default:
	  break;
    }
  }

  rk_mytermregime ();
  mt_gotoXY(2,69);
  mt_setbgcolor(STANDART);
  std::cin >> code;
  sc_memorySet(cell_coord.y*10 + cell_coord.x, code);
  printMainTable();
  printCell(CIAN, cell_coord);
  
  mt_gotoXY(23,1);
}













/////////////////////////////////////////


int READ(int nCell){ // 10 Ввод с терминала в указанную ячейку памяти с контролем переполнения
  if (nCell > 99 || nCell < 0){
	return -1;
  }

  rk_mytermregime ();
  mt_gotoXY(23,1);
  int code;
  std::cin >> code;
  if (code > 9999 || code < 9999)
	return -1;
  sc_memorySet(nCell, code); 
  return 0;
}

void WRITE(int nCell){ // 11 Вывод на терминал значения указанной ячейки памяти
  int temp;
  sc_memoryGet(nCell, &temp);
  mt_gotoXY(23,1);
  std::cout << temp;
}

void LOAD(int nCell){ // 20 Загрузка в аккумулятор значения из указанного адреса памяти
  accumulator = *memory.at(nCell);
}

void STORE(int nCell){ // 21 Выгрузка значения из аккумулятора по указанному адресу памяти
  *memory.at(nCell) = accumulator;
}

void ADD(int nCell){ // 30 Выполняет сложение слова в аккумуляторе и слова из указанной ячейки памяти(результат в аккумуляторе)
  accumulator += *memory.at(nCell);
}

void SUB(int nCell){ // 31 Вычитает из слова в аккумуляторе слово из указанной ячейки памяти(результат в аккумуляторе)
  accumulator -= *memory.at(nCell);
}

void DIVIDE(int nCell){ // 32 Выполняет деление слова в аккумуляторе на слово из указанной ячейки памяти(результат в аккумуляторе)
  accumulator /= *memory.at(nCell);
}

void MUL(int nCell){ // 33 Вычисляет произведение слова в аккумуляторе на слово из указанной ячейки памяти(результат в аккумуляторе)
  accumulator *= *memory.at(nCell);
}

void JUMP(int nCell){ // 40 Переход к указанному адресу памяти
  instructionCounter = nCell;
}

void JNEG(int nCell){ // 41 Переход к указанному адресу памяти, если в аккумуляторе находится отрицательное число
  if (accumulator < 0)
    instructionCounter = nCell;
}

void JZ(int nCell){ // 42 Переход к указанному адресу памяти, если в аккумуляторе находится нуль
  if (accumulator == 0)
    instructionCounter = nCell;
}

void HALT(){ // 43 Останов, выполняется при завершении работы программы

}

void LOGLC(int nCell){ // 67 Логический двоичный сдвиг содержимого указанного участка памяти влево на количество разрядов, указанное в аккумуляторе (результат в аккумуляторе)
  accumulator = *memory.at(nCell) << accumulator;
}