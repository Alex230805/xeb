#define LXER_IMPLEMENTATION

#include "lxer.h"


void lxer_start_lexing(lxer_head* lh, char * source){
  if(DEBUG) DINFO("Start lexing", NULL);
  lh->source = source;
  lh->source_len = strlen(source);
  
  size_t array_size = 24;
  token_slice **cache_mem = (token_slice**)arena_alloc(&lh->lxer_ah,sizeof(token_slice*)*array_size);
  size_t array_qt = 0;
  bool end = false;
  char * buffer = (char*)arena_alloc(&lh->lxer_ah,sizeof(char)*32);
  
  for(size_t i=0;i<lh->source_len;i++){
    char* tracker = &lh->source[i];
    token_slice* tl;
    for(size_t j=0;j<TOKEN_TABLE_END && !end; j++){
      LXR_TOKENS token = token_array[j];
      size_t ws = strlen(token_table_lh[token]);
      buffer[0] = '\0';
      strcat(buffer, tracker);
      buffer[ws] = '\0';
      //////////////////////////////////////////
      if(strcmp(buffer,token_table_lh[token]) == 0){
        if(DEBUG) DINFO("tracker point at %ld trigger symbol %s", tracker+i - tracker, token_table_lh[token]);
        tl = (token_slice*)arena_alloc(&lh->lxer_ah,sizeof(token_slice));
        tl->token = token;
        tl->byte_pointer = tracker;
        cache_mem[array_qt] = tl;
        array_qt+=1;
        //end = true;
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
      ////////////////////////////////////////
    }
    end = false;
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
    printf("token found at byte_stream[%ld]: token_tablep[%d] -> %s\n", pointer-lh->source, tok,token_table_lh[tok]);
  }
}
