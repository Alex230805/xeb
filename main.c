#include <stdio.h>
#include <stdlib.h>

#include <xeb.h>
#include <misc.h>


#define CF_OUTPUT_FILE "-o" 
#define CF_HELPER_0   "-h"
#define CF_HELPER_1   "--help"
#define CF_BUILDER_INTEGRATION_0 "-eb"
#define CF_BUILDER_INTEGRATION_1 "--external-builder"
#define CF_SOURCE_FILE_0 "-sc"
#define CF_SOURCE_FILE_1 "--source-code"
#define CF_MODULE_PATH_0 "-mp"
#define CF_MODULE_PATH_1 "--module-path"


#define CF_DEFAULT_OUTPUT_FILENAME "a.out"
#define CF_DEFAULT_INPUT_FILE_NAME "main.x"
#define CF_DEFAULT_MODULE_PATH "./module"

Arena_header ah = {0};


void xebc_wrong_flag_error(char*argv){
  fprintf(stderr, "Error: wrong flag usage, type '--help' to get more information about how to use '%s'\n", argv);
  return;
}


void xebc_helper(){
  printf("Implement helper\n");
  return;
}


int main(int argc, char** argv){
  
  char*output_file_name = (char*)arena_alloc(&ah, sizeof(char)*256);
  char*source_file_path = (char*)arena_alloc(&ah, sizeof(char)*256);
  char*module_path = (char*)arena_alloc(&ah, sizeof(char)*256);
  strcpy(output_file_name, CF_DEFAULT_OUTPUT_FILENAME);
  strcpy(source_file_path, CF_DEFAULT_INPUT_FILE_NAME);
  strcpy(module_path, CF_DEFAULT_MODULE_PATH);
  bool external_builder_integration = false;
  bool file_provided = false;

  if(argc > 1 && (strcmp(argv[1], CF_HELPER_0) == 0 || strcmp(argv[1], CF_HELPER_1) == 0)){
    //helper integration
    xebc_helper();
  }else{
    // flag search
    for(int i=1;i<argc; i++){
      // output file flag
      if(strcmp(argv[i], CF_OUTPUT_FILE) == 0){
        if(i+1 < argc){
          strcpy(output_file_name, argv[i+1]);
        }else{
          xebc_wrong_flag_error(argv[i]);
        }
      }
      
      // external builder integration
      if(strcmp(argv[i], CF_BUILDER_INTEGRATION_0) == 0 ||  strcmp(argv[i], CF_BUILDER_INTEGRATION_1) == 0){
          external_builder_integration = true; 
      }
    
      // source file flag
      if(strcmp(argv[i], CF_SOURCE_FILE_0) == 0 || strcmp(argv[i], CF_SOURCE_FILE_1) == 0){
        if(i+1 < argc){
          strcpy(source_file_path, argv[i+1]);
          file_provided = true;
        }else{
          xebc_wrong_flag_error(argv[i]);
        }
      }

      // module path loader

      if(strcmp(argv[i], CF_MODULE_PATH_0) == 0 || strcmp(argv[i], CF_MODULE_PATH_0) == 0){
        if(i+1 < argc){
          strcpy(module_path, argv[i+1]);
        }else{
          xebc_wrong_flag_error(argv[i]);
        }
      }

    }
    for(int i=0;i<argc;i++){
      if(!file_provided && strstr(argv[i], ".x") != NULL){
        fprintf(stderr,"Warning: the source code file must be provided with the '%s', otherwhise the compiler will search the first file in the current path that is named '%s'. Try the '%s' flag for more informations\n\n", CF_SOURCE_FILE_0, CF_DEFAULT_INPUT_FILE_NAME, CF_HELPER_0);
      }
    }
    if(!xeb_load_file(source_file_path)){
      fprintf(stderr, "Error: could not find a valid source file, did you insert the right path for any file with a '*.x' extension?\n");
    }else{
      xeb_error_init_handler();
      if(external_builder_integration) xeb_error_open_public_hoterror_broadcaster();
      xeb_load_output_filename(output_file_name);
      xeb_start_compiler(module_path);
      xeb_error_report();
      xeb_close_compiler();       
    }
  }
  return 0;
}
