
all: xeb main

xeb: toolbox lxer
	cc -Wall -L ./bin/ -Ilib/ -llxer.o -ltoolbox.o -c -o bin/xeb.o ./lib/xeb.c

lxer: 
	cc -Wall -L ./bin/ -I ./lib/ -c -o bin/lxer.a ./lib/lxer.c

toolbox:
	cc -Wall -c -o bin/toolbox.o ./lib/toolbox.c

main:
	cc -Wall -Werror -Lbin/ -Ilib/ -lxeb.o -ltoolbox.o  ./main.c -o xeb

