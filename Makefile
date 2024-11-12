

all: xeb main

xeb:
	gcc -Wall -Werror -L./bin -I./lib -c -o bin/xeb.o ./lib/xeb.c


main:
	gcc -Wall -Werror -L./bin -I./lib ./main.c -o xeb
