FLAG=-Wall -Wextra -Werror
INCLUDE_DIR=-I lib/
LIB_DIR=-I bin/

all: xeb main

xeb: toolbox lxer
	gcc $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) -llxer -c -o bin/xeb.o ./lib/xeb.c
	ar rc bin/libxeb.a bin/xeb.o
	ranlib bin/libxeb.a

lxer: 
	gcc $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) -c -o bin/lxer.o ./lib/lxer.c
	ar rc bin/liblxer.a bin/lxer.o
	ranlib bin/liblxer.a

toolbox:
	gcc $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) -c -o bin/toolbox.o lib/toolbox.c
	ar rc bin/libtoolbox.a bin/toolbox.o
	ranlib bin/libtoolbox.a

main:
	gcc $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) -lxeb -ltoolbox ./main.c -o xebc


