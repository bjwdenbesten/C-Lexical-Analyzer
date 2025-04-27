CC=gcc
CFLAGS = -Werror

lexer: lexer.c main.c
	$(CC) $(CFLAGS) -o lexer lexer.c main.c

clean:
	rm lexer
