#define LXER_IMPLEMENTATION

#include "lxer.h"


void lxer_start_lexing(lxer_head* lh, char * source){
  if(DEBUG) DINFO("Start lexing", NULL);
  lh->source = source;
  lh->source_len = strlen(source);
  
  size_t array_size = 24;
  token_slice **cache_mem = (token_slice**)arena_alloc(&lh->lxer_ah,sizeof(token_slice*)*array_size);
  size_t array_qt = 0;
  char * buffer = (char*)arena_alloc(&lh->lxer_ah,sizeof(char)*32);
  bool ignore_lex;

  for(size_t i=0;i<lh->source_len;i++){
    char* tracker = &lh->source[i];
    token_slice* tl;
    for(size_t j=0;j<TOKEN_TABLE_END; j++){
      LXR_TOKENS token = token_array[j];
      switch(token){
        case TAG_MATH_END:
        case TAG_TYPE_END:
        case TAG_COMMENT_END:
        case TAG_SEP_END:
        case TAG_BRK_END:
        case TAG_STATEMENT_END:
          ignore_lex = true;
          break;
        default:
          ignore_lex = false;
          break;
      } 
      //////////////////////////////////////////
      if(!ignore_lex){
        size_t ws = strlen(token_table_lh[token]);
        buffer[0] = '\0';
        strcat(buffer, tracker);
        buffer[ws] = '\0';
        if(!ignore_lex && strcmp(buffer,token_table_lh[token]) == 0){
          tl = (token_slice*)arena_alloc(&lh->lxer_ah,sizeof(token_slice));
          tl->token = token;
          tl->byte_pointer = tracker;
          cache_mem[array_qt] = tl;
          array_qt+=1;
          if(array_qt == array_size){
            size_t old_size = array_size;
            array_size = array_size*2;
            token_slice** n_cache_mem = (token_slice**)arena_alloc(&lh->lxer_ah,sizeof(token_slice*)*array_size);
            for(size_t z=0;z<old_size;z++){
              n_cache_mem[z] = cache_mem[z];
            }
            cache_mem = n_cache_mem;
          }
        }

      }
      ////////////////////////////////////////
    }
  }
  lh->stream_out = cache_mem;
  lh->stream_out_len = array_qt;
  return;
}


void lxer_get_lxer_content(lxer_head*lh){
  NOTY("LXER","Tokenzer output: ", NULL);
  for(size_t i=0;i<lh->stream_out_len;i++){
    LXR_TOKENS tok = lh->stream_out[i]->token;
    char* pointer = lh->stream_out[i]->byte_pointer;
    printf("\ttoken found at byte_stream[%ld]: token_tablep[%d] -> %s\n", pointer-lh->source, tok,token_table_lh[tok]);
  }
}

bool lxer_next_token(lxer_head*lh){
  lh->lxer_tracker+=1;
  if(lh->lxer_tracker == lh->stream_out_len) {
    return false;
  }
  return true;
}


LXR_TOKENS lxer_get_current_token(lxer_head*lh){
  return lh->stream_out[lh->lxer_tracker]->token;
}

void lxer_set_new_target(lxer_head* lh, char* new_line){
  for(size_t i=lh->lxer_tracker;i<lh->stream_out_len; i++){
    if(lh->stream_out[i]->byte_pointer > new_line){
      lh->lxer_tracker = i;
      break;
    }
  }
}


char* lxer_get_current_pointer(lxer_head*lh){
  return lh->stream_out[lh->lxer_tracker]->byte_pointer;
}

bool lxer_is_math(LXR_TOKENS token){
  if(token < TAG_MATH_END && token >= 0) return true;
  return false;
}
bool lxer_is_comment(LXR_TOKENS token){
  if(token < TAG_COMMENT_END && token > TAG_MATH_END) return true;
  return false;
}

bool lxer_is_type(LXR_TOKENS token){
  if(token > TAG_COMMENT_END && token < TAG_TYPE_END) return true;
  return false;
}

bool lxer_is_sep(LXR_TOKENS token){
  if(token > TAG_TYPE_END && token < TAG_SEP_END) return true;
  return false;
}
bool lxer_is_brk(LXR_TOKENS token){
  if(token > TAG_SEP_END && token < TAG_BRK_END) return true;
  return false;
}

bool lxer_is_statement(LXR_TOKENS token){
  if(token > TAG_BRK_END && token < TAG_STATEMENT_END) return true;
  return false;
}

bool lxer_is_misc(LXR_TOKENS token){
  if(token < TOKEN_TABLE_END && token > TAG_STATEMENT_END) return true;
  return false;
}

bool lxer_math_expect_math(lxer_head*lh){
  if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_math_expect_comment(lxer_head*lh){ 
  if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false;
}

bool lxer_math_expect_type(lxer_head*lh){
  if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_math_expect_sep(lxer_head*lh){ 
  if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_math_expect_brk(lxer_head*lh){
 if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_math_expect_statement(lxer_head*lh){ 
 if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_math_expect_misc(lxer_head*lh){
  if( 
      lxer_is_math(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_comment_expect_math(lxer_head*lh){
  if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_comment_expect_comment(lxer_head*lh){
  if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_comment_expect_type(lxer_head*lh){
  if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_comment_expect_sep(lxer_head*lh){
 if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } return false; 
}

bool lxer_comment_expect_brk(lxer_head*lh){
 if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } return false; 
}

bool lxer_comment_expect_statement(lxer_head*lh){
 if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_comment_expect_misc(lxer_head*lh){
  if( 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_type_expect_math(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_type_expect_comment(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_type_expect_type(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_type_expect_sep(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_type_expect_brk(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }return false; 
}

bool lxer_type_expect_statement(lxer_head*lh){
    if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }return false; 
}

bool lxer_type_expect_misc(lxer_head*lh){
  if( 
      lxer_is_type(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_sep_expect_math(lxer_head*lh){ 
  if( 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_sep_expect_comment(lxer_head*lh){
  if( 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_sep_expect_type(lxer_head*lh){ 
  if( 
      lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
      lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}
bool lxer_sep_expect_sep(lxer_head*lh){ 
  if( 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}


bool lxer_sep_expect_brk(lxer_head*lh){ 
  if( 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_sep_expect_statement(lxer_head*lh){
  if( 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_sep_expect_misc(lxer_head*lh){ 
  if( 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false;
}



bool lxer_brk_expect_math(lxer_head*lh){ 
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_brk_expect_comment(lxer_head*lh){ 
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_brk_expect_type(lxer_head*lh){
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false;
}


bool lxer_brk_expect_sep(lxer_head*lh){
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_brk_expect_brk(lxer_head*lh){ 
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_brk_expect_statement(lxer_head*lh){
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_brk_expect_misc(lxer_head*lh){ 
  if( 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}



bool lxer_statement_expect_math(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}

bool lxer_statement_expect_comment(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false;
}

bool lxer_statement_expect_type(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}
bool lxer_statement_expect_sep(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}
bool lxer_statement_expect_brk(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}
bool lxer_statement_expect_statement(lxer_head*lh){
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}


bool lxer_statement_expect_misc(lxer_head*lh){ 
  if( 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }
  return false; 
}



bool lxer_misc_expect_math(lxer_head*lh){ 
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_math(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}

bool lxer_misc_expect_comment(lxer_head*lh){
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_comment(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }  
  return false; 
}

bool lxer_misc_expect_type(lxer_head*lh){ 
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_type(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}

bool lxer_misc_expect_sep(lxer_head*lh){
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_sep(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}

bool lxer_misc_expect_brk(lxer_head*lh){ 
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_brk(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  }  
  return false; 
}

bool lxer_misc_expect_statement(lxer_head*lh){ 
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_statement(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}

bool lxer_misc_expect_misc(lxer_head*lh){ 
  if( 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker]->token) && 
    lxer_is_misc(lh->stream_out[lh->lxer_tracker+1]->token)
  ){
    return true;
  } 
  return false; 
}


char* lxer_get_rh(lxer_head* lh, bool reverse){
  char* buffer = (char*)arena_alloc(&lh->lxer_ah, sizeof(char)*256);

  size_t tracker = lh->lxer_tracker;
  if(reverse) tracker-=1;

  char*pointer = lh->stream_out[tracker]->byte_pointer + strlen(token_table_lh[lh->stream_out[tracker]->token]);
  while(*pointer < '0') pointer+=1;
  size_t word_len = 0;
  while(pointer[word_len] != ' '  && &pointer[word_len] < lh->stream_out[tracker+1]->byte_pointer) word_len+=1;
  
  memcpy(&buffer[0],pointer, word_len);
  buffer[word_len] = '\0';
  
  return buffer;
}


char** lxer_get_rh_lh(lxer_head*lh){
  char** buffer_array = (char**)arena_alloc(&lh->lxer_ah, sizeof(char*)*2);

  buffer_array[0] = lxer_get_rh(lh, false); 
  buffer_array[1] = lxer_get_rh(lh, true);

  return buffer_array;
}
