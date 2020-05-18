CFLAGS = -std=c++17 -Werror -pedantic-errors -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo
OBJ = g++ $(CFLAGS) -c $< -o $@
TEST = g++ $(CFLAGS) -c $< -o $@
allFolders  = #folder1 folder2 folder3 folder4 folder5
allO = main.o #build/src/kmenu.o build/src/typing_tutor.o build/src/Numerical_simulator.o
allTests = #build/test/reaction.o build/test/getXX.o build/test/getYY.o build/test/coordinate_test.o build/test/result_test.o build/test/num_tests.o build/test/printWelcomePanel.o


.PHONY: clean

##############################
all: clear lib libBC libRK             # 
##############################

build: $(allFolders) executable

clear:
	clear

lib: 
	g++ -c myTerm.cpp -o myTerm.o
	ar rc libmyTerm.a myTerm.o

libBC: 
	g++ -c myBigChars.cpp -o myBigChars.o
	ar rc libmyBigChars.a myBigChars.o

libRK:
	g++ -c $(CFLAGS) myReadkey.cpp -o myReadkey.o
	ar rc libmyReadkey.a myReadkey.o
	g++ main.cpp libmyTerm.a libmyBigChars.a libmyReadkey.a

disk: 
	g++ -c myTerm.cpp -o myTerm.o
	ar rc libmyTerm.a myTerm.o

sat:
	g++ -o sat.out sat.cpp 


############################  FOLDER  ############################

folder1:	
	mkdir -p build	
	
############################  BUILD  ############################

executable: $(allO)
	g++ $(CFLAGS) $^ -o $@ 
	
run: ./a
	
clean:
	rm -r build
	rm -r bin

help:
	@echo "=========================HELP THERE!========================="
	@echo "|                                                           |"
	@echo "|  1. Build project: Make build                             |"
	@echo "|  2. Test: Make test                                       |"
	@echo "|  3. Claen: make clean                                     |"
	@echo "|                                                           |"
	@echo "============================================================="