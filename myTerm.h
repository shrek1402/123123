#pragma once
#include <iostream>
#include <stdio.h>

enum colors {
  BLACK,
  RED,
  GREEN,
  BROWN,
  BLUE,
  VIOLET,
  CIAN,
  GRAY,
  STANDART = 9,
};

//производит очистку и перемещение курсора в левый
//верхний угол экрана
extern int mt_clrscr();

int mt_gotoXY(int row, int cols);

int mt_getscreensize (int* rows, int* cols);

int mt_setfgcolor (colors color);

int mt_setbgcolor(colors color);