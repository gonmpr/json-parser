#include "parser.h"
#include <string.h>
#include <stdlib.h>



json_value_t parse_value(json_file_t *file){
  skip_whitespace(file);

  json_value_t value = {0};

  switch(*(file->cursor)){

    case '{':
      value = parse_object(file);
      break;

    case '[':
      value = parse_array(file);
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


json_value_t parse_array(json_file_t *file){
  if(!expect_char(file, '[')) 
    return (json_value_t){0};
  if(expect_char(file, ']')) 
    return (json_value_t){.type=JSON_ARRAY,.array = json_array_new(0)};

  json_array_t *new_array = json_array_new(4);
  if (!new_array)
      return (json_value_t){ .type = JSON_PARSING_ERROR };
  skip_whitespace(file);

  while(true){
    json_value_t new_value = parse_value(file);
    json_array_push(new_array, new_value);

    if(expect_char(file, ']')) break;
    else if(expect_char(file, ',')) continue;
    else {
      json_array_free(new_array);
      return (json_value_t){.type=JSON_PARSING_ERROR};
    }
  }

  json_value_t array_value = {0};
  array_value.type = JSON_ARRAY;
  array_value.array = new_array;
  return array_value;

}




json_value_t parse_object(json_file_t *file){
  if(!expect_char(file, '{')) 
    return (json_value_t){0};
  if(expect_char(file, '}')) 
    return (json_value_t){.type=JSON_OBJECT,.object = json_object_new(0)};

  json_object_t *new_object = json_object_new(4);
  if (!new_object)
      return (json_value_t){ .type = JSON_PARSING_ERROR };

  skip_whitespace(file);

  while(true){
    char *new_key = parse_key(file);  
    if(!new_key || !expect_char(file, ':')){
      free(new_key);
      json_object_free(new_object);
      return (json_value_t){.type=JSON_PARSING_ERROR};
    }

    json_value_t new_value = parse_value(file);
    json_pair_t new_pair = (json_pair_t){.key = new_key, .value = new_value};

    json_object_push(new_object, new_pair);

    if(expect_char(file, '}')) break;
    else if(expect_char(file, ',')) continue;
    else{
      json_object_free(new_object);
      return (json_value_t){.type=JSON_PARSING_ERROR};
    }
  }

  json_value_t object_value = {0};
  object_value.type = JSON_OBJECT;
  object_value.object = new_object;
  return object_value;
}



char *parse_key(json_file_t *file){
  skip_whitespace(file);
  json_value_t value = parse_string(file);
  if(value.type != JSON_STRING)
    return NULL;
  return value.string;
}



json_value_t parse_string(json_file_t *file){
  json_value_t new_str;
  new_str.type = JSON_STRING;

  skip_whitespace(file);

  if (*file->cursor != '"') {
    new_str.type = JSON_PARSING_ERROR;
    return new_str;
  }

  char *string_start = ++file->cursor;

  while(file->cursor < file->end && *(file->cursor) != '"' ){
    if(*(file->cursor) == '\\') file->cursor++;
    file->cursor++;
  }

  if(file->cursor == file->end){
    new_str.type = JSON_PARSING_ERROR;
    return new_str;
  }

  size_t str_size = file->cursor - string_start;

  char *str_ptr = malloc(str_size + 1);
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
void skip_whitespace(json_file_t *file){
  while(*file->cursor == ' ' || *file->cursor == '\t' 
        || *file->cursor == '\n' || *file->cursor == '\r')
    file->cursor++;
}

bool expect_char(json_file_t *file, char expected){
  skip_whitespace(file);
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


    
//auxiliares objetos

json_object_t *json_object_new(size_t capacity){

  json_object_t *new_obj = malloc(sizeof(json_object_t));
  if(!new_obj) return NULL;

  json_pair_t *new_pairs = capacity == 0 ? NULL : 
                           malloc(sizeof(json_pair_t) * capacity);
  if(capacity > 0 && !new_pairs){
    free(new_obj);
    return NULL;
  }

  *new_obj = (json_object_t){.pairs = new_pairs, 
                             .count = 0, 
                             .capacity = capacity};
  
  return new_obj;
}


bool json_object_push(json_object_t *object, json_pair_t pair){
  if(!object) return 0;

  if(object->count == object->capacity){
    size_t new_capacity = object->capacity == 0 ? 4 : object->capacity * 2;
    json_pair_t *new_pairs = realloc(object->pairs, 
                              sizeof(json_pair_t) * new_capacity);
    if(!new_pairs) return 0;

    object->pairs = new_pairs;
    object->capacity = new_capacity;
  }

  object->pairs[object->count] = pair;
  object->count++;
  return 1;

}


// liberadores 

void json_value_free(json_value_t *value){
  if(!value) return;

  switch(value->type){

    case JSON_ARRAY:
      json_array_free(value->array);
      break;

    case JSON_OBJECT:
      json_object_free(value->object);
      break;

    case JSON_STRING:
      free(value->string);
      break;

    default:
      break;
  }

}


void json_array_free(json_array_t *array){
  if(!array) return;
  for(size_t i = 0; i < array->count; i++){
    json_value_free(&(array->items[i]));
  }
  free(array->items);
  free(array);
}

void json_object_free(json_object_t *object){
  if(!object) return;
  for(size_t i = 0; i < object->count; i++){
    json_value_free(&((object->pairs[i]).value));
    free((object->pairs[i]).key);
  }
  free(object->pairs);
  free(object);
}
