CC     = gcc -Wall -g -std=c89 -pedantic -O2
CFLAGS = -I/usr/local/include
LIBS   = -L/usr/local/lib -lm -lpthread -lX11 -lXcursor -lXcursor -ldl -lGL -lallegro -lallegro_font -lallegro_ttf -liksemel

OBJS   = main.o d2im.o
all:$(OBJS)
	$(CC) $(OBJS)  $(CFLAGS) $(LIBS) -o ../bin/d2im
test:
	make d2im_test -C ../bin
clean:
	rm  *.o ../bin/d2im -f
$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@






