#ifndef LXER_H
#define LXER_H 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <toolbox.h>
#include <stdbool.h>


#define TYPE void*

#include <Array.h>

typedef struct{
  char **symbols;
  size_t symbols_length;

  char **keywords;
  size_t keywords_length;

  char **definitions;
  size_t definitions_length;

  char **types;
  size_t types_length;

  char **operators;
  size_t operators_length;

  char **comments;
  size_t comments_length;

  char **separators;
  size_t separators_length;

  char** literals;
  size_t literals_length;

  String_builder *file_word;
  size_t current_pointer;

}lxer;

static lxer l = {0};

#define lxer_notef(arg) printf(#arg);

// lxer symbols definition 

int lxer_load_symb(char **symbols, size_t symbols_length);
int lxer_load_keyw(char **keywords, size_t keywords_length);
int lxer_load_definitions(char **definitions, size_t definitions_length);
int lxer_load_types(char **types, size_t types_length);
int lxer_load_operators(char **operators, size_t operators_length);
int lxer_load_separators(char **separators, size_t separators_length);
int lxer_load_comments(char **comments, size_t comments_length);

int lxer_load_literals(char **literals, size_t literals_length);

int lxer_load_alphabet( char **symbols, size_t symbols_length,
                        char **keywords, size_t keywords_length,
                        char **definitions, size_t definitions_length,
                        char **types, size_t types_length,
                        char **operators, size_t operators_length,
                        char **comments, size_t comments_length,
                        char **separators, size_t separators_length,
                        char **literals, size_t literals_length);

// error, warning and notification 

void lxer_noty_error(const char*str);
void lxer_noty_warn(const char*str);
void lxer_noty_printf(const char*str);

// file loading

void lxer_read_entire_file(const char* filepath);
char lxer_get_token();
String_builder* lxer_get_phrase();
String_builder* lxer_check_for_phrase(char*keyword, int start_point);

bool lxer_eof();
char* lxer_get_file_ptr();
size_t lxer_get_total_file_line();
size_t lxer_get_file_line_from_ptr(char*str);

int lxer_get_file_size();
void lxer_next_token();
size_t lxer_get_pointer();

Array* lxer_locate_occurences(char*word);
char* lxer_get_first_valid_location();
// lexer token relation


bool lxer_left_expect_right(char**arr_1, size_t arr_1_length,char** arr_2, size_t arr_2_length);

bool lxer_symbol_expect_keyword();
bool lxer_symbol_expect_definition();
bool lxer_symbol_expect_type();
bool lxer_symbol_expect_operator();
bool lxer_symbol_expect_comment();
bool lxer_symbol_expect_separator();

bool lxer_keyword_expect_symbol();
bool lxer_keyword_expect_definition();
bool lxer_keyword_expect_type();
bool lxer_keyword_expect_operator();
bool lxer_keyword_expect_comment();
bool lxer_keyword_expect_separator();

bool lxer_definition_expect_symbol();
bool lxer_definition_expect_keyword();
bool lxer_definition_expect_types();
bool lxer_definition_expect_operator();
bool lxer_definition_expect_comment();
bool lxer_definition_expect_separator();

bool lxer_type_expect_symbol();
bool lxer_type_expect_keyword();
bool lxer_type_expect_definition();
bool lxer_type_expect_operator();
bool lxer_type_expect_comments();
bool lxer_type_expect_separator();

bool lxer_operator_expect_symbol();
bool lxer_operator_expect_keyword();
bool lxer_operator_expect_definition();
bool lxer_operator_expect_type();
bool lxer_operator_expect_comment();
bool lxer_operator_expect_separator();

bool lxer_separator_expect_symbol();
bool lxer_separator_expect_keyword();
bool lxer_separator_expect_definition();
bool lxer_separator_expect_type();
bool lxer_separator_expect_operator();
bool lxer_separator_expect_comment();

//Array* lxer_check_namespace();

bool lxer_literal_expect_symbol();
bool lxer_literal_expect_keyword();
bool lxer_literal_expect_definition();
bool lxer_literal_expect_types();
bool lxer_literal_expect_operator();
bool lxer_literal_expect_separator();

bool lxer_symbol_expect_literal();
bool lxer_keyword_expect_literal();
bool lxer_definition_expect_literal();
bool lxer_type_expect_literal();
bool lxer_operator_expect_literal();
bool lxer_separator_expect_literal();


#ifndef LXER_IMPLEMENTATION
#define LXER_IMPLEMENTATION
#endif



#endif // lxer
