CC		=	gcc
CFLAGS  =   -Wall -g
LIBS    =   -lm
OBJS    =   dcc_decoder.o dcc_extract.o
all:$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS)
test:
	./a.out FATRLITA1HTH.dcc
clean:
	rm *.o a.out -rf
$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
