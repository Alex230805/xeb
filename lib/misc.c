#define MISC_IMP

#include "misc.h"

#ifdef GC_IMP

void gc_init(tb_gc *gc){
  gc->address = (void**)malloc(sizeof(void*)*POOL_SIZE);
  gc->size = POOL_SIZE;
  gc->pointer = 0;
}

void gc_free(tb_gc*gc){
  for(int i=0;i<gc->pointer;i++){
    free(gc->address[i]);
    gc->address[i] = NULL;
  }
}

void gc_push(tb_gc*gc, void* address){
  if(gc->pointer+1 >= POOL_SIZE){
    fprintf(stderr, "garbage collector full of trash man, check your code, this is unacceptable!\n");
    exit(1);
  }else{
    gc->address[gc->pointer] = address;
    gc->pointer += 1;
  }
}

#endif

StringBuilder* read_file(char*path){
  if(DEBUG) DINFO("Reading file", NULL); 
  StringBuilder *sb;
  sb = (StringBuilder*)malloc(sizeof(StringBuilder));
  FILE * fp;
  fp = fopen(path, "r");
  int len = 0;
  if(fp == NULL){
    fprintf(stderr, "Unable to open instruction file: %s : %d\n", strerror(errno), errno);
    exit(1);
  }
  fseek(fp, 0, SEEK_END);
  sb->len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  //MALLOC(sizeof(char)*sb->len, sb->string, char*);
  sb->string = (char*)malloc(sizeof(char)*sb->len);
  fread(sb->string,sizeof(char), sb->len,fp);
  sb->string[len] = '\0'; 
  fclose(fp);
  return sb;
}

void write_file(StringBuilder *sb, char *path){
  if(DEBUG) DINFO("Writing file", NULL); 
  FILE * fp;
  fp = fopen(path, "w");
  if(fp == NULL){
    fprintf(stderr, "Unable to open instruction file: %s : %d\n", strerror(errno), errno);
    exit(1);
  }
  fwrite(sb->string, 1, sb->len, fp);
  fclose(fp);
}


u8t hexDigitConverter(char s){
    if(isdigit(s)){
        return s - '0';
    }else{
         return toupper(s) - 'A' + 10;
    }
}

u8t hexStringConverter(char string[]){
    uint8_t HexString = 0;
    int len = strlen(string);
    for(int i=0;i<len;i++){
        if(!isxdigit(string[i]))
            return -1;
       int cache = hexDigitConverter(string[i]);
       HexString = (HexString << 4) | cache; 
    }
    return HexString;
} 


void arena_create(Arena_header* arenah, int page_size, int page_count){
  
  Arena_alloc* arena = (Arena_alloc*)malloc(sizeof(Arena_alloc));
  arena->obj = 0;
  arena->pages = page_count;
  arena->free_pages = page_count;
  arena->arena_start_ptr = malloc(sizeof(size_t)*page_size*page_count);
  arena->page_size = page_size;
  arena->pages_pointers = malloc(sizeof(size_t*)*page_count);
  arena->allocated_page = malloc(sizeof(bool)*page_count);
  arena->next = NULL;

  for(int i=0;i<page_count;i++){
    arena->allocated_page[i] = false;
  }
  for(int i=0;i<page_count;i++){
    arena->pages_pointers[i] = &arena->arena_start_ptr[(i*page_size)];
  }

  arena->cursor = 0; 
  arenah->swap = arenah->first_arena;
  
  if(arenah->arena_count == 0 && arenah->swap != NULL){
    while ( arenah->swap->next != NULL) arenah->swap = arenah->swap->next;
    arenah->swap->next = arena;
    arenah->swap = NULL;
    arenah->arena_count += 1;
    arenah->cursor = arena;
  }else{
    arenah->first_arena = arena;
    arenah->swap = NULL;
    arenah->arena_count = 1;
    arenah->cursor = arena;
  }  
}

void* arena_alloc(Arena_header* arenah, size_t size){
  if(arenah->arena_count < 1){
    arena_create(arenah,PAGE_SIZE, PAGE_NUMBER);
  }

  Arena_alloc* arena = arenah->cursor;

  if(arena->free_pages < 1){
    int page_number = (int)size/(arena->page_size*arena->pages);
    if(page_number > 0){
      int pages = arena->pages;
      while(pages <= page_number){
        pages = pages*2;
      }
      arena_create(arenah, arena->page_size, pages);
    }else{
      arena_create(arenah, arena->page_size, arena->pages);
    }
    arena = arenah->cursor;
  }
  void* pointer = NULL; 

  int number_of_required_pages = (int)( size / arena->page_size) + 1;
  int i = 0;
  for(i=0; i < number_of_required_pages;i++){
    arena->allocated_page[arena->cursor+i] = true;
  }
  
  pointer = (void*)arena->pages_pointers[arena->cursor];
  arena->cursor = (arena->cursor + i) + 1;
  arena->free_pages -= (i+1);

  return pointer;
}

void arena_free_area(Arena_alloc* arena){
  free(arena->allocated_page);
  arena->allocated_page = NULL;
  free(arena->pages_pointers);
  arena->pages_pointers = NULL;
  free(arena->arena_start_ptr);
  arena->pages_pointers = NULL;
  free(arena);
  return;
}


void arena_free(Arena_header *arenah){
  Arena_alloc* a = arenah->first_arena;
  if(arenah->first_arena->next != NULL){
    arenah->first_arena = arenah->first_arena->next;
    arena_free(arenah);
  }
  arena_free_area(a);
  a = NULL;
  return;
}
