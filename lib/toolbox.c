#define TOOLBOX_IMPLEMENTATION

#include "toolbox.h"

void init_gc(tb_gc *gc){
  gc->address = (void**)malloc(sizeof(void*)*POOL_SIZE);
  gc->size = POOL_SIZE;
  gc->pointer = 0;
}

void free_trash(tb_gc*gc){
  for(int i=0;i<gc->pointer;i++){
    free(gc->address[i]);
    gc->address[i] = NULL;
  }
}

void push_address(tb_gc*gc, void* address){
  if(gc->pointer+1 >= POOL_SIZE){
    fprintf(stderr, "garbage collector full of trash man, check your code, this is unacceptable!\n");
    exit(1);
  }else{
    gc->address[gc->pointer] = address;
    gc->pointer += 1;
  }
}
