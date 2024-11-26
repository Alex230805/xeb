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


int lxer_load_symb(char **symbols, size_t symbols_length){
 if(symbols == NULL || symbols_length < 1){
    lxer_noty_error("Symbols pointer is null or no element in the array\n");
    return 1;
  } 
  l.symbols = symbols;
  l.symbols_length = symbols_length;
 
  return 0;

}

int lxer_load_keyw(char **keywords, size_t keywords_length){
 if(keywords == NULL || keywords_length < 1){
    lxer_noty_error("Symbols pointer is null or no element in the array\n");
    return 1;
  } 
  l.keywords = keywords;
  l.keywords_length = keywords_length;
  
  return 0;
}


int lxer_load_definitions(char **definitions, size_t definitions_length){
  if(definitions == NULL || definitions_length < 1){
    lxer_noty_error("Definitions pointer is null or no element in the array\n");
    return 1;
  } 

  l.definitions = definitions;
  l.definitions_length = definitions_length;
  
  return 0;
}

int lxer_load_types(char **types, size_t types_length){
  if(types == NULL || types_length < 1){
    lxer_noty_error("Types pointer is null or no element in the array\n");
    return 1;
  }

  l.types = types;
  l.types_length = types_length;
  
  return 0;
}


int lxer_get_file_size(){
  return l.file_word->len;
}

char* lxer_get_file_ptr(){
  return l.file_word->string;
}

int lxer_load_operators(char **operators, size_t operators_length){
  if(operators == NULL || operators_length < 1){
    lxer_noty_error("Operators pointer is null or no element in the array\n");
    return 1;
  }

  l.operators = operators;
  l.operators_length = operators_length;

  return 0;
}

int lxer_load_comments(char **comments, size_t comments_length){
  if(comments == NULL || comments_length < 1){
    lxer_noty_error("Comments pointer is null or no element in the array\n");
    return 1;
  }

  l.comments = comments;
  l.comments_length = comments_length;
  
  return 0;
}


int lxer_load_separators(char **separators, size_t separators_length){
  if(separators == NULL || separators_length < 1){
    lxer_noty_error("Separators pointer is null or no element in the array\n");
    return 1;
  }

  l.separators = separators;
  l.separators_length = separators_length;
  
  return 0;
}


int lxer_load_literals(char **literals, size_t literals_length){
  if(literals == NULL || literals_length < 1){
    lxer_noty_error("Literals pointer is null or no element in the array\n");
    return 1;
  }

  l.literals = literals;
  l.literals_length = literals_length;
  
  return 0;
}

int lxer_load_alphabet( char **symbols, size_t symbols_length,
                        char **keywords, size_t keywords_length,
                        char **definitions, size_t definitions_length,
                        char **types, size_t types_length,
                        char **operators, size_t operators_length,
                        char **comments, size_t comments_length,
                        char **separators, size_t separators_length,
                       char **literals, size_t literals_length){

  if(
      lxer_load_symb(symbols,symbols_length) == 1 ||
      lxer_load_definitions(definitions, definitions_length) == 1 ||
      lxer_load_keyw(keywords, keywords_length) == 1 ||
      lxer_load_types(types, types_length) == 1 ||
      lxer_load_operators(operators, operators_length) == 1 ||
      lxer_load_comments(comments, comments_length) == 1 ||
      lxer_load_separators(separators, separators_length) == 1 ||
      lxer_load_literals(literals, literals_length) == 1
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

void lxer_next_token(){
  TODO("Check for end_of_token and for anythings but 0x21\n");
  while(l.file_word->string[l.current_pointer] < 0x21){
    l.current_pointer += 1;
  }
}


String_builder* lxer_check_for_phrase(char*keyword, int start_point){
  String_builder*sb = NULL;
  MALLOC(sizeof(String_builder), sb, String_builder*);

  bool end = false;  
  char buffer[256];
  int i=1;
  sb->len = 6969;
    
  while(!end){
    memcpy(&buffer[0], &l.file_word->string[l.current_pointer+start_point], i);
    buffer[i] = '\0';
    if(strcpy(buffer, keyword) == 0){
      end = true;
    }else{
      i+=1;
    }
  }

  MALLOC(sizeof(char)*i+1, sb->string, char*);
  sb->len = i+1;
  strcpy(sb->string, buffer);

  return sb;
}

// lexer token relation

bool lxer_left_expect_right(char**arr_1, size_t arr_1_length, char** arr_2, size_t arr_2_length){
  bool end = false;
  size_t i=0;
  String_builder *sb = NULL;
  char*buffer;
  MALLOC(sizeof(char)*256, buffer, char*);

  while(!end && i < arr_1_length){
    sb = lxer_check_for_phrase(arr_1[i], l.current_pointer);  
    if(sb->len == 6969){
      i+=1;
    }else{
      end = true;
    }
  }
  if(i == arr_1_length){
    lxer_noty_error("Unable to find token in specific location\n");
    return false;
  }

  lxer_next_token();

  strcpy(buffer, sb->string);
  i = 0;
  end = false;

  while(!end && i < arr_2_length){
    sb = lxer_check_for_phrase(arr_2[i], l.current_pointer);  
    if(sb->len == 6969){
      i+=1;
    }else{
      end = true;
    }
  }

  if(i == arr_2_length){
    lxer_noty_error("No relation between selected symbol ");
    fprintf(stderr, "%s and any keyword\n", buffer);
    return false;
  }
  return true;

}


/*
  *
  * symbol -> any
  *
  * */

bool lxer_symbol_expect_keyword(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.keywords, l.keywords_length);  
}

bool lxer_symbol_expect_definition(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.definitions, l.definitions_length);  
}

bool lxer_symbol_expect_type(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.types, l.types_length);  
}

bool lxer_symbol_expect_operator(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.operators, l.operators_length);  
}

bool lxer_symbol_expect_comment(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.comments, l.comments_length);  
}

bool lxer_symbol_expect_separator(){
  return lxer_left_expect_right(l.symbols, l.symbols_length, l.separators, l.separators_length);  
}



/*
  *
  * keyword -> any
  *
  * */


bool lxer_keyword_expect_symbol(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.symbols, l.symbols_length);  
}

bool lxer_keyword_expect_definition(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.definitions, l.definitions_length);  

}

bool lxer_keyword_expect_type(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.types, l.types_length);  

}

bool lxer_keyword_expect_operator(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.operators, l.operators_length);  

}

bool lxer_keyword_expect_comment(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.comments, l.comments_length);  

}

bool lxer_keyword_expect_separator(){
  return lxer_left_expect_right(l.keywords, l.keywords_length, l.separators, l.separators_length);  

}



/*
  *
  * definition -> any
  *
  * */



bool lxer_definition_expect_symbol(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.symbols, l.symbols_length);
}

bool lxer_definition_expect_keyword(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.keywords, l.keywords_length);  
  
}

bool lxer_definition_expect_types(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.types, l.types_length);  

}

bool lxer_definition_expect_operator(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.operators, l.operators_length);  

}

bool lxer_definition_expect_comment(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.comments, l.comments_length);  

}

bool lxer_definition_expect_separator(){
  return lxer_left_expect_right(l.definitions, l.definitions_length, l.separators, l.separators_length);  

}

bool lxer_literal_expect_symbol(){
  bool ret =  lxer_left_expect_right(l.literals, l.literals_length, l.symbols, l.symbols_length);

  if(!ret){
    TODO("Implement search for different token after the literal");

  }

}

bool lxer_literal_expect_keyword(){
  bool ret = lxer_left_expect_right(l.literals, l.literals_length, l.keywords, l.keywords_length);

}

bool lxer_literal_expect_definition(){
  bool ret = lxer_left_expect_right(l.literals, l.literals_length, l.definitions, l.definitions_length);

}

bool lxer_literal_expect_types(){
  bool ret = lxer_left_expect_right(l.literals, l.literals_length, l.types, l.tyles_length);
}

bool lxer_literal_expect_operator(){
  bool ret = lxer_left_expect_right(l.literals, l.literals_length, l.operators, l.operators_length);
}

bool lxer_literal_expect_separator(){
  bool ret = lxer_left_expect_right(l.literals, l.literals_length, l.separators, l.separators_length);
}

bool lxer_symbol_expect_literal();
bool lxer_keyword_expect_literal();
bool lxer_definition_expect_literal();
bool lxer_type_expect_literal();
bool lxer_operator_expect_literal();
bool lxer_separator_expect_literal();




/*
  *
  * type -> any
  *
  * */



bool lxer_type_expect_symbol(){
  return lxer_left_expect_right(l.types,l.types_length, l.symbols, l.symbols_length);
}

bool lxer_type_expect_keyword(){
  return lxer_left_expect_right(l.types,l.types_length, l.keywords, l.keywords_length);
} 

bool lxer_type_expect_definition(){
  return lxer_left_expect_right(l.types,l.types_length, l.definitions, l.definitions_length);
} 

bool lxer_type_expect_operator(){
  return lxer_left_expect_right(l.types,l.types_length, l.operators, l.operators_length);
} 

bool lxer_type_expect_comments(){
  return lxer_left_expect_right(l.types,l.types_length, l.comments, l.comments_length);
} 

bool lxer_type_expect_separator(){
  return lxer_left_expect_right(l.types,l.types_length, l.separators, l.separators_length);
} 



/*
  *
  * operator -> any
  *
  * */


bool lxer_operator_expect_symbol(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.symbols, l.symbols_length);
} 

bool lxer_operator_expect_keyword(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.keywords, l.keywords_length);
}  

bool lxer_operator_expect_definition(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.definitions, l.definitions_length);
}  

bool lxer_operator_expect_type(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.types, l.types_length);
}  

bool lxer_operator_expect_comment(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.comments, l.comments_length);
}  

bool lxer_operator_expect_separator(){
  return lxer_left_expect_right(l.operators,l.operators_length, l.separators, l.separators_length);
}  


/*
  *
  * separator -> any
  *
  * */


bool lxer_separator_expect_symbol(){
  return lxer_left_expect_right(l.separators, l.separators_length,  l.symbols, l.symbols_length);
}  

bool lxer_separator_expect_keyword() {
  return lxer_left_expect_right(l.separators, l.separators_length,  l.keywords, l.keywords_length);
}   

bool lxer_separator_expect_definition(){
  return lxer_left_expect_right(l.separators, l.separators_length,  l.definitions, l.definitions_length);
}   

bool lxer_separator_expect_type(){
  return lxer_left_expect_right(l.separators, l.separators_length,  l.types, l.types_length);
}  

bool lxer_separator_expect_operator(){
  return lxer_left_expect_right(l.separators, l.separators_length,  l.operators, l.operators_length);
}  

bool lxer_separator_expect_comment() {
  return lxer_left_expect_right(l.separators, l.separators_length,  l.comments, l.comments_length);
}  


Array* lxer_locate_occurences(char*word){
  Array* arr;
  char*switcher = l.file_word->string;

  char buff[256];

  strcpy(buff, word);
  buff[strlen(word)] = ' ';
  buff[strlen(word)+1] = '\0';

  array_new(arr);
  for(size_t i=0;i<l.file_word->len;i++){
    char*pointer = strstr(&switcher[0], word);  
    if(pointer != NULL){
      array_push(arr, (void*)pointer);
      switcher = &l.file_word->string[(int)(pointer-l.file_word->string+1)];
    }
  }
  return arr;
}


size_t lxer_get_pointer(){
  return l.current_pointer;
}

