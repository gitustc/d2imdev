CC		=	gcc -Wall -g
CFLAGS  =   -I ../../allegro/allegro4/include
LIBS    =   ../../allegro/allegro4/lib/*.a -lm -lpthread -lX11 -lXcursor -lXcursor -ldl
dt1show:dt1show.c
	$(CC) dt1show.c $(CFLAGS) $(LIBS) -o ../bin/dt1show
	#make -C ../bin show
clean:
	rm ../bin/dt1show *.o -f
