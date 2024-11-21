#define XEB_C
#include "xeb.h"



void xeb_preprocessor(){
  
  namespaces_occ = lxer_locate_occurences(definitions[FUNCTION]);
  function_definitions_occ = lxer_locate_occurences(definitions[DEFINITION]);
  linker_reference_occ = lxer_locate_occurences(definitions[IMPORT]);
  comments_position_occ = lxer_locate_occurences(comments[DOUBLE_DASH]);

  array_new(namespaces);
  array_new(function_definitions);
  array_new(linker_reference);
  array_new(comments_position);
  
  size_t j = 0;
  bool end = false;
  char*buffer = NULL;

  /* get function name */

  for(size_t i=0;i<namespaces_occ->nelem;i++){
    char*pointer;
    array_get(namespaces_occ, i, pointer);
    while(!end){
      if((pointer[j] >= 0x61 && pointer[j] <= 0x7a) || (pointer[j] >= 0x41 && pointer[j] <= 0x5a)){
        j+=1;
      }else{
        end = true;
      }
    }
    MALLOC(sizeof(char)*j, buffer, char*);
    memcpy(buffer, pointer, j);
    buffer[j] = '\0';

    array_push(namespaces, buffer);
    buffer = NULL;
  }

  j = 0;
  end = false;

  TODO("Implement the preprocessor part to get function definition, linker reference with the import statement and exclude comments by saving the start and the end location\n");

}

void xeb_lexer(){
 if(lxer_load_alphabet(
    symbols, SYM_COUNT, 
    keywords, KWD_COUNT,
    definitions, DEF_COUNT,
    types, TYP_COUNT,
    comments, CMT_COUNT,
    operators, OPR_COUNT,
    separators, SPR_COUNT
  ) != 0){
    lxer_noty_error("Unable to load alphabet, failed on lexer startup not in actual code compilation\n");
    exit(0);
  }

  TODO("Complete lexer usage in xeb_lexer");
  return;
  
  while(!lxer_eof()){
  }

}

int xeb_compiler(String_builder*path,String_builder*output_path){

  TODO("Complete the compiler");
  char*ptr = strstr(path->string, ".x");
  if(ptr == NULL) return -1;

  UNUSED(output_path);
  //UNUSED(path);
  
  lxer_read_entire_file(path->string);
  xeb_preprocessor();
  xeb_lexer(); 

  return 0;
}
