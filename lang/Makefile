.PHONY = build, clean

SRC = dict.c lang.c list.c stack.c word.c
OBJ = $(SRC:.c=.o)

build:
	$(CC) $(FLAGS) $(SRC) -c
	ar rs liblang.a $(OBJ)
	rm -f $(OBJ)

clean:
	rm -f liblang.a