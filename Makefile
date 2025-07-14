FLAG=-Wall -Wextra 
INCLUDE_DIR=-I./lib/
LIB_DIR=-L./bin/
COMP=clang

main: ./main.c xeb
	$(COMP) $(FLAG) $(INCLUDE_DIR) $(LIB_DIR) ./main.c -o xebc -lXeb

xeb: xeb_inst cbox lxer ./lib/xeb.c ./lib/xeb.h 
	$(COMP) $(FLAG) $(INCLUDE_DIR) -c -o bin/xeb.o ./lib/xeb.c
	ar rc ./bin/libXeb.a ./bin/*.o

xeb_inst: ./lib/xeb_inst.c ./lib/xeb_inst.h 
	$(COMP) $(FLAG) $(INCLUDE_DIR) -c -o bin/xeb_inst.o $<

lxer: ./lib/lxer.c ./lib/lxer.h
	$(COMP) $(FLAG) $(INCLUDE_DIR) -c -o bin/lxer.o ./lib/lxer.c

cbox: ./lib/misc.c ./lib/misc.h
	$(COMP) $(FLAG) $(INCLUDE_DIR) -c -o bin/misc.o lib/misc.c
