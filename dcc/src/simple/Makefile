CC		=	gcc -Wall
CFLAGS  =   -I ../../../allegro/allegro4/include -I./mpq -Wall -g
LIBS    =   ../../../allegro/allegro4/lib/*.a -lm -lpthread -lX11 -lXcursor -lXcursor -ldl -lGL
OBJS    =   dcc_decoder.o dcc_extract.o
all:$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS)
test:
	./a.out FATRLITA1HTH.dcc
clean:
	rm *.o a.out -rf
$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
