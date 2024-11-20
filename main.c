#include <stdio.h>
#include <stdlib.h>

#include "xeb.h"
#include "toolbox.h"

tb_gc general_gc = {0};

void print_helper(){
  printf("Xeb compiler usage:\n");
  printf("executable structure: xeb [flag1] {flag argument} [flag2] .... [flagN] {flag argument} filename.x\n\n");
  printf("\t\tcompile a simple file: xeb [filename].x\n");
  printf("\t\tspecify output path:   xeb -o /my/path/file.name [filename].x\n\n");
  printf("\nLIST OF FLAGS:\n\n");
  printf("-h or --help: get helper\n");
  printf("-o or --output: specify output name\n");
}

int main(int argc, char** argv){
  init_gc(&general_gc);

  String_builder filename;
  String_builder output_path;

  if(argc < 2){
    fprintf(stderr, "No input or flag provided\n");
    print_helper();
  }else{
    int i=1;
    while(i < argc){
      if(strcmp(argv[i], "-o") == 0){
        if(i+1 <= argc-1){
          MALLOC(sizeof(char)*strlen(argv[i+1]), output_path.string, char*);

          strcpy(output_path.string, argv[i+1]);
          output_path.len = strlen(output_path.string);
          i+=1;
        }else{
        printf("Missing argument for [-o] flag, you must provide an output name\n");
          print_helper();
          return 1;
        }
      }else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
        print_helper();
        return 0;
      }else if(i+1 == argc){
        MALLOC(sizeof(char)*strlen(argv[i]), filename.string,char*);
        strcpy(filename.string, argv[i]);
        filename.len = strlen(filename.string);
      }else{
        printf("Wrong argument, try to take a look at the helper by using [-h] or [--help] flag and address your issue\n");
        print_helper();
        return 1;
      }
      i+=1;
    }
  }
  xeb_compiler(&filename, &output_path);

  free_trash(&general_gc);
  return 0;
}
