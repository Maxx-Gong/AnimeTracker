objects = main.obj func.obj

MAKE = make
COMPILER = g++ -O3 -Wall
DEL = del

main.exe : $(objects) main.h Makefile
	$(COMPILER) -o main.exe $(objects) main.h


%.obj : %.cpp Makefile
	$(COMPILER) -c $*.cpp -o $*.obj

run :
	$(MAKE) main.exe
	main.exe

clean :
	-$(DEL) *.obj
