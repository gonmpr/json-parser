#include "parser.h"
#include <string.h>
#include <stdlib.h>


void parse_key();

void parse_json_object();
json_value_t parse_array(json_file_t *file){
  json_array_t array = {0};



}




/*
  2. parse_array simple: [1, true, "hola"]
  3. parse_object simple: {"a": 1}
  4. liberar memoria recursivamente
*/

json_value_t parse_value(json_file_t *file){

  file->cursor = skip_whitespace(file->cursor);

  json_value_t value = {0};

  switch(*(file->cursor)){

    case '{':
      break;
    case '[':
      break;

    case 'n':
      value = parse_null(file);
      break;

    case '"':
      value = parse_string(file);
      break;

    case 't':
    case 'f':
      value = parse_bool(file);
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
      value = parse_number(file);
      break;
    default:
      value.type = JSON_PARSING_ERROR;
      break;
  }

  return value;
}






json_value_t parse_string(json_file_t *file){
  json_value_t new_str;
  new_str.type = JSON_STRING;

  char *string_start = ++file->cursor;
  while(file->cursor < file->end && *(file->cursor) != '"' ){
    if(*(file->cursor) == '\\') file->cursor++; // TODO: escape symbols
    file->cursor++;
  }
  
  if(file->cursor == file->end){
    new_str.type = JSON_PARSING_ERROR;
    return new_str;
  }

  size_t str_size = file->cursor - string_start;

  char *str_ptr = malloc(str_size + 1);               //ALLOCATION
  if(!str_ptr){
    new_str.type = JSON_PARSING_ERROR;
    file->cursor = file->end;
    return new_str;
  }

  for(size_t i = 0; i < str_size; i++){
    str_ptr[i] = *(string_start + i);
  }
  str_ptr[str_size] = '\0';
  new_str.string = str_ptr;

  file->cursor++;
  return new_str;
}



json_value_t parse_number(json_file_t *file){

  json_value_t new_number;
  new_number.type = JSON_NUMBER;
  char *number_end;

  new_number.number = strtod(file->cursor, &number_end);
  if(file->cursor == number_end){
    new_number.type = JSON_PARSING_ERROR;
    file->cursor = file->end;
    return new_number;
  }

  file->cursor = number_end;
  return new_number;
}


json_value_t parse_bool(json_file_t *file){
  json_value_t new_bool; 
  new_bool.type = JSON_BOOL;

  if(!strncmp(file->cursor, "true", 4)){
    new_bool.boolean = 1;
    file->cursor += strlen("true");
    return new_bool;
  }

  if(!strncmp(file->cursor, "false", 5)){
    new_bool.boolean = 0;
    file->cursor += strlen("false");
    return new_bool;
  }

  new_bool.type = JSON_PARSING_ERROR;
  file->cursor = file->end;
  return new_bool;
}




json_value_t parse_null(json_file_t *file){
  json_value_t new_null; 
  new_null.boolean = 0;

  if(!strncmp(file->cursor, "null", 4)){
    new_null.type = JSON_NULL;
    file->cursor += strlen("null");
    return new_null;
  }

  new_null.type = JSON_PARSING_ERROR;
  file->cursor = file->end;
  return new_null;

}

//auxiliares parsers
char *skip_whitespace(char *steps){
  char *new_steps = steps;
  while(*new_steps == ' ' || *new_steps == '\t' || *new_steps == '\n')
    new_steps++;
  return new_steps;
}

bool expect_char(json_file_t *file, char expected){
  if (*(file->cursor) == expected){
    file->cursor++;
    return 1;
  }
  return 0;
}



//auxiliares arrays
json_array_t *json_array_new(size_t capacity){
  json_array_t *new_arr = malloc(sizeof(json_array_t));
  if(!new_arr) return NULL;

  json_value_t *items = capacity == 0 ? NULL : 
                        malloc(sizeof(json_value_t) * capacity);
  if(capacity > 0 && !items){
    free(new_arr);
    return NULL;
  }

  *new_arr = (json_array_t){.items = items, .count = 0, .capacity = capacity};
  return new_arr;

}

bool json_array_push(json_array_t *array, json_value_t value){
  if(!array) return 0;

  if(array->count == array->capacity){
    size_t new_capacity = array->capacity == 0 ? 4 : array->capacity * 2;
    json_value_t *new_items = realloc(array->items, 
                              sizeof(json_value_t) * new_capacity);
    if(!new_items) return 0;

    array->items = new_items;
    array->capacity = new_capacity;
  }

  array->items[array->count] = value;
  array->count++;
  return 1;
}

void json_array_free(json_array_t *array);
/*
json_array_free
  libera todos los values internos
  libera items
  libera el array
*/

    
//auxiliares objetos

/*
json_object_new
json_object_push
json_object_free
*/

//json_value_free
