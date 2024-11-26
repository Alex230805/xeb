#define XEB_C
#include "xeb.h"

void xeb_error(const char*text){
  fprintf(stderr,"[Compiler ERROR]: %s", text);
}

void xeb_warn(const char*text){
  fprintf(stderr,"[Compiler WARNING]: %s", text);
}

void xeb_preprocessor(){
  
  bool have_main = false;

  namespaces_occ = lxer_locate_occurences(definitions[FUNCTION]);
  linker_reference_occ = lxer_locate_occurences(definitions[IMPORT]);
  comments_position_occ = lxer_locate_occurences(comments[DOUBLE_DASH]);

  array_new(namespaces);
  array_new(linker_reference);
  array_new(comments_position);
  
  size_t j = 0;
  bool end = false;
  char*pointer = NULL;

  /* get function name 
   *
   *  NOTE: this is not the  parsing for function function reference 
   *  or definition. It save only the namespace reference and 
   *  next im the lexing process it will be used to check 
   *  the function definition or function call or function 
   *  implementation etc etc
   * */



  for(size_t i=0;i<namespaces_occ->nelem;i++){
    pointer = NULL;
    end = false;
    j = 0;

    array_get(namespaces_occ, i, pointer);
    pointer+=strlen(definitions[FUNCTION]);
    while(pointer[j] <= ' '){
      j+=1;
    }

    pointer += j;
    j = 0;

    while(!end){
      if((pointer[j] >= 0x61 && pointer[j] <= 0x7a) || (pointer[j] >= 0x41 && pointer[j] <= 0x5a) || pointer[j] == 0x5f){
        j+=1;
      }else{
        end = true;
      }
    }

    /* create box element to save function name and the row where it is located */

    box* b = NULL;

    MALLOC(sizeof(box), b, box*);
    MALLOC(sizeof(String_builder), b->word, String_builder*);
    MALLOC(sizeof(char)*j, b->word->string, char*);

    b->word->len = j;
    memcpy(&b->word->string[0], pointer, j);
    b->word->string[j] = '\0';
    
    array_get(namespaces_occ, i, b->position);

    if(strcmp(b->word->string,"main") == 0) have_main = true;
    array_push(namespaces, b);
  }

  if(!have_main){
    xeb_error("Undefined reference to main function\n");
    return;
  }

  j = 0;
  end = false;

  /*  identify comment location */

  for(size_t i=0;i<comments_position_occ->nelem; i++){
    pointer = NULL;
    end = false;
    j=0;

    array_get(comments_position_occ, i, pointer);
    
    while(!end){
      if(pointer[j] != '\n'){
        j+=1;
      }else{
        end = true;
      }
    }

    range * r;
    MALLOC(sizeof(range), r, range*);
    r->start = pointer;
    r->end = pointer+j+1;
    array_push(comments_position, r);
  }

  TODO("Finish lxer_reference and function_definition in xeb preprocessor");

}

void xeb_lexer(){
 if(lxer_load_alphabet(
    symbols, SYM_COUNT, 
    keywords, KWD_COUNT,
    definitions, DEF_COUNT,
    types, TYP_COUNT,
    comments, CMT_COUNT,
    operators, OPR_COUNT,
    separators, SPR_COUNT,
    literals, LTR_COUNT
  ) != 0){
    lxer_noty_error("Unable to load alphabet, failed on lexer startup not in actual code compilation\n");
    return;
  }

  TODO("Complete lexer usage in xeb_lexer");
  return;
  
  while(!lxer_eof()){
     
  

    lxer_next_token();
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
