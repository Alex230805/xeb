#define LXER_IMPLEMENTATION

#include "lxer.h"

void lxer_noty_error(const char*str){
  fprintf(stderr,"[ERROR]: %s", str);
}
void lxer_noty_warn(const char*str){
  printf("[WARNING]: %s", str);

}
void lxer_noty_printf(const char*str){
  printf("[PRINT]: %s", str);
}


// loading process 


int lxer_load_symb(char *symbols, size_t symbols_length){
 if(symbols == NULL || symbols_length < 1){
    lxer_noty_error("Symbols pointer is null or no element in the array\n");
    return 1;
  } 
  l.symbols = symbols;
  l.symbols_length = symbols_length;
 
  return 0;

}

int lxer_load_keyw(char *keywords, size_t keywords_length){
 if(keywords == NULL || keywords_length < 1){
    lxer_noty_error("Symbols pointer is null or no element in the array\n");
    return 1;
  } 
  l.keywords = keywords;
  l.keywords_length = keywords_length;
  
  return 0;
}


int lxer_load_definitions(char *definitions, size_t definitions_length){
  if(definitions == NULL || definitions_length < 1){
    lxer_noty_error("Definitions pointer is null or no element in the array\n");
    return 1;
  } 

  l.definitions = definitions;
  l.definitions_length = definitions_length;
  
  return 0;
}

int lxer_load_types(char *types, size_t types_length){
  if(types == NULL || types_length < 1){
    lxer_noty_error("Types pointer is null or no element in the array\n");
    return 1;
  }

  l.types = types;
  l.types_length = types_length;
  
  return 0;
}


int lxer_load_operators(char *operators, size_t operators_length){
  if(operators == NULL || operators_length < 1){
    lxer_noty_error("Operators pointer is null or no element in the array\n");
    return 1;
  }

  l.operators = operators;
  l.operators_length = operators_length;

  return 0;
}

int lxer_load_comments(char *comments, size_t comments_length){
  if(comments == NULL || comments_length < 1){
    lxer_noty_error("Comments pointer is null or no element in the array\n");
    return 1;
  }

  l.comments = comments;
  l.comments_length = comments_length;
  
  return 0;
}


int lxer_load_separators(char *separators, size_t separators_length){
  if(separators == NULL || separators_length < 1){
    lxer_noty_error("Separators pointer is null or no element in the array\n");
    return 1;
  }

  l.separators = separators;
  l.separators_length = separators_length;
  
  return 0;
}

int lxer_load_alphabet( char *symbols, size_t symbols_length,
                        char *keywords, size_t keywords_length,
                        char *definitions, size_t definitions_length,
                        char *types, size_t types_length,
                        char *operators, size_t operators_length,
                        char *comments, size_t comments_length,
                        char *separators, size_t separators_length ){

  if(
      lxer_load_symb(symbols,symbols_length) == 1 ||
      lxer_load_definitions(definitions, definitions_length) == 1 ||
      lxer_load_keyw(keywords, keywords_length) == 1 ||
      lxer_load_types(types, types_length) == 1 ||
      lxer_load_operators(operators, operators_length) == 1 ||
      lxer_load_comments(comments, comments_length) == 1 ||
      lxer_load_separators(separators, separators_length) == 1
  ){
    lxer_noty_error("An error occurred during the loading alphabet process\n");
    exit(1);
  }
  return 0;
}



void lxer_read_entire_file(const char* filepath){
  FILE* fp;

  String_builder *sb = NULL;
  MALLOC(sizeof(String_builder), sb, String_builder*);

  if((fp = fopen(filepath, "r")) == NULL){
    lxer_noty_error("Unable to open file path due to: ");
    fprintf(stderr, "%s", strerror(errno));
    exit(1);
  } 
  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  MALLOC(sizeof(char)*len+1, sb->string, char*);
  sb->len = len;
  fread(&sb->string[0], 1, len+1, fp);
  sb->string[len] = '\0';
  fclose(fp);
  l.file_word = sb;
}

char lxer_get_token(){
  char c;
  while(l.file_word->string[l.current_pointer] != '\0' && l.file_word->string[l.current_pointer] == ' ' &&  l.current_pointer < l.file_word->len){
    l.current_pointer += 1;
  }
  c = l.file_word->string[l.current_pointer];
  l.current_pointer+=1;
  return c;
}

String_builder* lxer_get_phrase(){
  size_t i = 0;
  String_builder* sb = NULL;
  bool started = false;
  bool end = false;

  MALLOC(sizeof(String_builder), sb, String_builder*);
  while(l.file_word->string[l.current_pointer+i] != '\0' && l.current_pointer+i < l.file_word->len && !end){
    if(l.file_word->string[l.current_pointer+i] == ' '){
      l.current_pointer+=1;
      if(started) end = true;
    }else{
      i+=1;
      started = true;
    }
  }
  MALLOC(sizeof(char)*i+2,sb->string, char*);
  sb->len = i+2;
  memcpy(&l.file_word->string[l.current_pointer], sb->string, i+1);
  sb->string[i+1] = '\0';

  l.current_pointer += i+1;

  return sb;
}


bool lxer_eof(){
  if(l.file_word->string[l.current_pointer] == '\0'){
    return true;
  }
  return false;
}


