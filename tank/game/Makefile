main: main.o game.o sprite.o map.o 
	g++ *.o -I./hsl -L./hsl  -lpthread -lhsl -lX11  -o  main -Wall -lrt
main.o:main.cpp
	g++ main.cpp -c -Wall
sprite.o:sprite.cpp sprite.hpp
	g++ sprite.cpp -c -Wall
map.o:map.cpp map.hpp
	g++ map.cpp -c -Wall


clean:
	rm *.o -f
	rm *~ -f
	rm main

