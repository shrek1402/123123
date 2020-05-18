#include <iostream>
#include <string>
#include "myTerm.h"
#include "myReadkey.h"

termios tsaved, changeterminal;

int rk_mytermregime (int regime, int echo, cc_t vtime, cc_t vmin){ // sigint !!!
  struct termios initialrsettings, newrsettings;
  
  tcgetattr(fileno(stdin), &initialrsettings);
  newrsettings = initialrsettings;

  if(regime == 1){  // канонический(enter)
    newrsettings.c_lflag |= static_cast<tcflag_t>(ICANON);
    newrsettings.c_cc[VTIME] = 0;
    newrsettings.c_cc[VMIN] = 0;
  } else if(regime == 0){
    newrsettings.c_cc[VTIME] = vtime;
    newrsettings.c_cc[VMIN] = vmin;
    newrsettings.c_lflag &= static_cast<tcflag_t>(~ICANON);
  }

  if(echo == 1){  // отображаются
    newrsettings.c_lflag |= static_cast<tcflag_t>(ECHO);
  } else if(echo == 0){
    newrsettings.c_lflag &= static_cast<tcflag_t>(~ECHO);
  }
  
  if (tcsetattr(fileno(stdin), TCSANOW, &newrsettings) != 0) {
    fprintf(stderr, "Could not set attributes\n");
    return 1;
  } else {
    return 0;
  }
}

//  ----------------------
int rk_readkey (uint64_t* KEY){
  union {
    char buf[8];
    uint64_t buf_value = 0;
  };
  
  rk_mytermregime (0, 0, static_cast<cc_t>(0.1f), 0);
  read(STDIN_FILENO, buf, 4);
  rk_mytermregime (1,0);

  *KEY = buf_value;
  return 0;
}


int rk_mytermsave (void){
  tcgetattr (fileno(stdin), &changeterminal);
  return 0;
}

int rk_mytermrestore (void){
  tcsetattr (fileno(stdin), TCSAFLUSH, &changeterminal);
  return 0;
}