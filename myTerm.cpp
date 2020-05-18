#include "myTerm.h"
#include <sys/ioctl.h>

//производит очистку и перемещение курсора в левый
//верхний угол экрана
int mt_clrscr() {
  std::cout << "\E[H\E[J";
  std::cout << "\E[1;1H";
  return 0;
}

int mt_gotoXY(int row, int cols){
  printf("\E[%i;%iH", row, cols);
  return 0;
}

int mt_getscreensize (int* rows, int* cols){
  struct winsize w;

  if(!ioctl(1, TIOCGWINSZ, &w))
  {
    *rows = w.ws_row;
    *cols = w.ws_col;
  }
  return 0;
}

int mt_setfgcolor (colors color){
  printf("\E[3%im", color);
  return 0;
}

int mt_setbgcolor(colors color){
  printf("\E[4%im", color);
  return 0;
}