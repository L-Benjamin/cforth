.PHONY = build, clean

build:
	flex forth.l
	bison forth.y -d
	$(CC) $(FLAGS) forth.tab.c -o forth -I ../lang ../lang/liblang.a

clean:
	rm -f lex.yy.c forth.tab.c forth.tab.h forth
