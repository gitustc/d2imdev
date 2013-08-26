tank: main.o game.o sprite.o map.o 
	gcc *.o `pkg-config allgro --cflags --libs`
main.o:main.c
	gcc main.c -c -Wall
sprite.o:sprite.cpp sprite.hpp
	gcc sprite.c -c -Wall
map.o:map.c
	g++ map.c -c -Wall


clean:
	rm *.o -f
	rm *~ -f
	rm tank
