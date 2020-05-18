#include <iostream>
#include <fstream>
#include <map>
#include "sat.h"
using namespace std;




int main(int argc, char* argv[]){
  map <string, int> :: iterator it;
  std::map <string,int> Map = {{ "READ", 10 },
                                { "WRITE", 11 },
                                { "LOAD", 20 },
                                { "STORE", 21 },
								{ "ADD", 30},
								{ "SUB", 31},
								{ "DIVIDE", 32 },
								{ "MUL", 33},
								{ "JUMP", 40},
								{ "JNEG", 41},
								{ "JZ", 42},
								{ "HALT", 43},
								{ "LOGLC", 67},
								{ "=", 80} };
  string s;
  ifstream file(argv[1]);
  std::ofstream fileOut(argv[2], ios::binary);
   
  while(getline(file, s)){
    string n, command, operand;
	int i = 0;
	n+= s[0]; 
	n+= s[1];
	for(i = 3; i < s.size() && s[i] != ' '; i++){
	  command += s[i];
	}

	for(i; i < s.size() && s[i] != ';'; i++){
	  operand += s[i];
	}

	com temp;
	temp.n = atoi(n.c_str());
	temp.command = Map.find(command)->second;
	temp.operand = atoi(operand.c_str());
	fileOut.write((char*)&temp, sizeof(com));
  }
  fileOut.close();

	std::ifstream fil;
  fil.open("sat.o", std::ios::binary);  // открыли файл для чтения

  com temp;
  while (!fil.eof()){
	if(fil.read((char*)(&temp), sizeof(com)))
    std::cout << temp.command << " ";
  }
  fil.close();

   
   file.close(); 
  return 0;
}