CC		=	gcc -Wall
CFLAGS  =   -I ../../allegro/allegro4/include -I./mpq -Wall -g
LIBS    =   ../../allegro/allegro4/lib/*.a -lm -lpthread -lX11 -lXcursor -lXcursor -ldl -lGL
OBJS    =   dccinfo.o merge_dcc.o
all:$(OBJS)
	$(CC) $(OBJS)  $(CFLAGS) $(LIBS) -o ../merge_dcc
clean:
	rm  *.o ../merge_dcc
$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
