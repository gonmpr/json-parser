#include "parser.h"
#include <string.h>

void parse_json_object();
void parse_key();
void parse_value();

void parse_array();

json_value_t parse_string(json_file_t *file){
  json_value_t new_str;
  new_str.type = JSON_STRING;

  size_t str_size = 0;
  file->offset++;
  while(str_size < file->size && *(file->start + file->offset) != '"') 
    str_size++;

  char *str_ptr = malloc(str_size + 1);
  if(!str_ptr){
    new_str.type = JSON_PARSING_ERROR;
    file->offset = size;
    return new_str;
  }

  for(int i = 0; i < str_size - 1; i++){
    str_ptr[i] = *(file->start + file->offset + i);
  }
  str_ptr[str_size] = '\0';

  new_str.string = str_ptr;
  file->offset += str_size;
  return new_str;
}



json_value_t parse_number(json_file_t *file){
  size_t old_offset = file->offset;

  json_value_t new_number;
  new_number.type = JSON_NUMBER;
  char *number_end;

  new_number.number = strtod((file->start + file->offset), number_end);

  
  if(file->offset + old_offset == file->start + file->offset){
    new_number.type = JSON_PARSING_ERROR;
    file->offset = size;
    return new_number;
  }

  file->offset = number_end - file->start;
  return new_number;
}




json_value_t parse_bool(json_file_t *file){
  json_value_t new_bool; 
  new_bool.type = JSON_BOOL;

  if(!strncmp(file->start + file->offset, "true", 4)){
    new_bool.boolean = 1;
    file->offset += strlen("true");
    return new_bool;
  }

  if(!strncmp(file->start + file->offset, "false", 5)){
    new_bool.boolean = 0;
    file->offset += strlen("false");
    return new_bool;
  }

  new_bool.type = JSON_PARSING_ERROR;
  file->offset = size;
  return new_bool;
}




json_value_t parse_null(json_file_t *file){
  json_value_t new_null; 
  new_null.boolean = 0;

  if(!strncmp(file->start + file->offset, "null", 4)){
    new_null.type = JSON_NULL;
    file->offset += strlen("null");
    return new_null;
  }

  new_null.type = JSON_PARSING_ERROR;
  file->offset = size;
  return new_null;

}


char *skip_whitespace(char *steps){
  char *new_steps = steps;
  while(*new_steps == ' ' || *new_steps == '\t' || *new_steps == '\n')
    new_steps++;
  return new_steps;
}

bool expect_char(char *chr, char expected){
  if (*chr == expected) return 1;
  return 0;
}


