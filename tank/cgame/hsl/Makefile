libhsl.a:window.o image.o base.o event.o font.o timer.o sound.o
	ar crv libhsl.a *.o
window.o:window.hpp window.cpp event.hpp event.cpp
	g++ -c window.cpp 
image.o:image.cpp image.hpp
	g++ -c image.cpp 
base.o:base.cpp
	g++ base.cpp -c 
event.o:event.cpp event.hpp
	g++ event.cpp -c 
font.o:font.hpp font.cpp
	g++ font.cpp -c 
timer.o:timer.cpp timer.hpp
	g++ timer.cpp -c
sound.o:sound.hpp sound.cpp
	g++ sound.cpp -c
clean:
	rm *.o
	rm *~
	rm main

	
