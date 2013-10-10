CC     = gcc -Wall
CFLAGS = -I ../../allegro/allegro5_0_10/include/allegro5/
LIBS   = ../../allegro/allegro5/lib/*.a -lm -lpthread -lX11 -lXcursor -lXcursor -ldl -lGL
OBJS   = main.o gewrp.o glb.o
all:$(OBJS)
	$(CC) $(OBJS)  $(CFLAGS) $(LIBS) -o ../bin/d2im
test:
	make d2im_test -C ../bin
clean:
	rm  *.o ../bin/d2im -f
$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
