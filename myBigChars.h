#pragma once
#include "myTerm.h"
#include <vector>
enum psevdoChars : char{
  left_up = 'l',
  left_down = 'm',
  right_up = 'k',
  right_down = 'j',
  ckboard = 'a',
  vertical = 'x',
  horizon = 'q'
};



int bc_printA(const char *str);
int bc_box(int x, int y, int cols, int rows);
int bc_printbigchar(uint64_t bitmask, int x, int y, colors fgcolor, colors bgcolor);
uint64_t bc_setbigcharpos (uint64_t* big, int x, int y, int value);
int bc_getbigcharpos(uint64_t* big, int x, int y, int* value);
int bc_bigcharwrite(int fd, int* big, int count);
int bc_bigcharread(int fd, int* big, int need_count, int* count);

