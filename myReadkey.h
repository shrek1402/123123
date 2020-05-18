#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>

enum keys : uint64_t{
  up = 4283163, 
  down = 4348699,
  right = 4414235,
  left = 4479771,
  f5 = 542058306331,
  f6 = 542091860763,
  save = 115,
  load = 108,
  enter = 10,
  run = 114,
  step = 116,
  reset = 105,
};

int rk_readkey (uint64_t*);

//1-kanon 0-nekanon
int rk_mytermregime (int regime = 1, int echo = 1, cc_t vtime = 0, cc_t vmin = 0); 

int rk_mytermsave (void);  //сохраняет текущие параметры терминала;
int rk_mytermrestore (void);  //восстанавливает сохраннные параметры терминала;