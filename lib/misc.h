#ifndef MISC_H
#define MISC_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define u8t uint8_t
#define u16t uint16_t
#define u32t uint32_t
#define u64t uint64_t
#define any void*

#define DEBUG true 
#define POOL_SIZE 1024

#if !(defined(PAGE_SIZE) || defined(PAGE_NUMBER))
  
  #define PAGE_SIZE 64 // in bytes   
  #define PAGE_NUMBER 5 // number of pages per arena. The length of the arena would be PAGE_SIZE*PAGE_NUMBER.

#endif 


typedef struct{
  void** address;
  int size;
  int pointer;
}tb_gc;

extern tb_gc general_gc;

typedef struct{
  char*string;
  int len;
}StringBuilder;
 
typedef struct Arena_alloc{
  struct Arena_alloc* next;
  int obj; // number or object allocated in memory
  int pages;  // number of pages created in memory
  int free_pages; // number of free pages
  size_t* arena_start_ptr; // arena start pointer
  int page_size; // page size 
  int cursor; // cursor to navigate the allocated page in the array
  size_t** pages_pointers; // page array reference
  bool* allocated_page; // page flags
}Arena_alloc;

typedef struct{
  int arena_count;
  Arena_alloc* first_arena;
  Arena_alloc* swap;
  Arena_alloc* cursor;
}Arena_header;



#define TODO(string,...) \
  fprintf(stdout,"[TODO]: "string"\n",__VA_ARGS__);

#define ERROR(string,...) \
  fprintf(stderr,"[ERROR]: "string"\n",__VA_ARGS__); \
  exit(1);

#define DINFO(string, ...) \
  fprintf(stdout, "[DEBUG]: "string"\n", __VA_ARGS__); 

#define NOTY(noty,string, ...) \
  printf("["noty"]: "string"\n", __VA_ARGS__);

#define WARNING(string,...) \
  printf("[WARNING]: "string"\n", __VA_ARGS__);

#define ARENA_ERROR(string) \
  fprintf(stderr,"[ARENA ALLOCATOR ERROR]: "string"\n");



#ifdef GC_IMP

#define MALLOC(args,ret_ptr, cast)\
    ret_ptr = (cast)malloc(args);\
    gc_push(&general_gc, (void*)ret_ptr);


void gc_init(tb_gc * gc);
void gc_free(tb_gc * gc);
void gc_push(tb_gc*gc, void* address);

#endif

// arena allocator
void arena_create(Arena_header* arenah, int page_size, int page_count);
void* arena_alloc(Arena_header* arenah ,size_t size);
void arena_free_area(Arena_alloc* arena);
void arena_free(Arena_header *arenah);


// hex digit converter
u8t hexStringConverter(char string[]);
u8t hexDigitConverter(char s);



// file 
void write_file(StringBuilder *sb, char *path);
StringBuilder* read_file(Arena_header *ah,const char*path);


// static declaration
extern Arena_header arenah;


#ifndef MISC_IMP
#define MISC_IMP

#endif

#endif 
