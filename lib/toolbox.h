#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <stdio.h>
#include <stdlib.h>



#define MALLOC(args,ret_ptr, cast)\
    ret_ptr = (cast)malloc(args);\
    push_address(&general_gc, (void*)ret_ptr);

#define UNUSED(x) (void)(x);
#define TODO(string) printf("Function or operation under development: "string"\n");
#define FATAL() printf("Operation not permitted\n"); exit(1);


#define POOL_SIZE 1024

typedef struct{
  char* string;
  size_t len;
}String_builder;


typedef struct{
  void** address;
  int size;
  int pointer;
}tb_gc;

extern tb_gc general_gc;

void init_gc(tb_gc * gc);
void free_trash(tb_gc * gc);
void push_address(tb_gc*gc, void* address);


#ifndef TOOLBOX_IMPLEMENTATION
#define TOOLBOX_IMPLEMENTATION

#endif

#endif // toolbox
