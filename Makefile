FLAG=-Wall -Wextra -Werror
INCLUDE_DIR=-I lib/
LIB_DIR=-L bin/

all: xeb main
	gcc $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) bin/main.o -o xebc -llxer -lxeb -ltoolbox

xeb: toolbox lxer
	gcc $(FLAG) $(INCLUDE_DIR) -c -o bin/xeb.o ./lib/xeb.c
	ar -src bin/libxeb.a bin/xeb.o
	ranlib bin/libxeb.a

lxer: 
	gcc $(FLAG) $(INCLUDE_DIR) -c -o bin/lxer.o ./lib/lxer.c
	ar -src bin/liblxer.a bin/lxer.o
	ranlib bin/liblxer.a

toolbox:
	gcc $(FLAG) $(INCLUDE_DIR) -c -o bin/toolbox.o lib/toolbox.c
	ar -src bin/libtoolbox.a bin/toolbox.o
	ranlib bin/libtoolbox.a

main:
	gcc $(FLAG) $(INCLUDE_DIR) -c ./main.c -o bin/main.o


