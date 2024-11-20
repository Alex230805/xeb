
all: xeb main
	rm bin/*.o

xeb: toolbox lxer
	cc -Wall -L ./bin -I./lib -l lxer.a -l toolbox.a -c -o bin/xeb.o ./lib/xeb.c
	ar rc bin/xeb.a bin/xeb.o
	ranlib bin/xeb.a

lxer: 
	cc -Wall -L ./bin -I ./lib -c -o bin/lxer.o ./lib/lxer.c
	ar rc bin/lxer.a bin/lxer.o
	ranlib bin/lxer.a

toolbox:
	cc -Wall -c -o bin/toolbox.o ./lib/toolbox.c
	ar rc bin/toolbox.a bin/toolbox.o
	ranlib bin/toolbox.a

main:
	cc -Wall -Werror -L./bin -I./lib -l xeb.a -l toolbox.a  ./main.c -o xebc

