#include "myBigChars.h"
#include <stdint.h>

int bc_box(int x, int y, int cols, int rows){
std::cout << "\E(0";  //вкл режим псевдографики

  //печатает вертикальные линии
  for(size_t i = y+1; i < y + rows - 1; ++i){
    mt_gotoXY(i,x);
    std::cout << (char)vertical;

    mt_gotoXY(i, x + cols - 1);
    std::cout << (char)vertical; // static_cast
  }

//печатает горизонтальные линии
  for(size_t i = x+1; i < x + cols - 1; ++i){
    mt_gotoXY(y,i);
    
    std::cout << (char)horizon;

    mt_gotoXY(y + rows - 1, i);
    std::cout << (char)horizon; // static_cast
  }
  //печатает уголки
    
    mt_gotoXY(y,x);
    std::cout << (char)left_up; // static_cast

    mt_gotoXY(y, x + cols - 1);
    std::cout << (char)right_up; // static_cast
 
    mt_gotoXY(y + rows - 1, x);
    std::cout << (char)left_down; // static_cast

    mt_gotoXY(y + rows - 1, x + cols - 1);
    std::cout << (char)right_down; // static_cast
    
    std::cout << "\E(B"; //выкл
  return 0;
}

int bc_printbigchar(uint64_t bitmask, int x, int y, colors fgcolor, colors bgcolor){
  mt_setbgcolor (bgcolor);
  mt_setfgcolor (fgcolor);
  for(size_t i = 0; i < 8; ++i){
    for(size_t j = 0; j < 8; ++j){
      mt_gotoXY(y+i,x+j);
      if((bitmask >> (63-(i*8 + j))) & 1){
        std::cout << "\E(0";
        std::cout << (char)ckboard;
        std::cout << "\E(B";
      } else {
        std::cout << " ";
      }
    }
  }
  return 0;
}

uint64_t bc_setbigcharpos (uint64_t *big, int x, int y, int value){

  if (value == 1){
    *big |= ((uint64_t)1 << (y*8 + (7-x)));
  } else if (value == 0){
    *big &= ~((uint64_t)1 << (y*8 + (7-x)));
  } else{
    return 1;
  }
  
  return 0;
}

//возвращает значение позиции в «большом символе»
//в строке x и столбце y;
int bc_getbigcharpos(uint64_t* big, int x, int y, int* value){
  if((*big >> (y*8 + (7-x))) & 1){
    *value = 1;
  } else {
    *value = 0;
  }
  return 0;
}

//записывает заданное число «больших символов» в файл. 
int bc_bigcharwrite(int fd, int* big, int count){
  return 0;
}
int bc_bigcharread(int fd, int* big, int need_count, int* count){return 0;}
