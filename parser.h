#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "load_file.h"

typedef struct json_value_t json_value_t;
typedef struct json_array_t json_array_t;
typedef struct json_object_t json_object_t;
typedef struct json_pair_t json_pair_t;

typedef enum {
  JSON_PARSING_ERROR,

  JSON_NULL,
  JSON_BOOL,
  JSON_NUMBER,
  JSON_ARRAY,
  JSON_STRING,
  JSON_OBJECT,
} json_data_type;

struct json_value_t {
    json_data_type type;

    union {
        double        number;
        int           boolean;
        char          *string;
        json_array_t  *array;
        json_object_t *object;
    };
};

struct json_array_t {
    json_value_t *items;
    size_t       count;
    size_t       capacity;
};

struct json_pair_t {
    char         *key;
    json_value_t value;
};

struct json_object_t {
    json_pair_t  *pairs;
    size_t       count;
    size_t       capacity;
};

/* parser */
json_value_t parse_value(json_file_t *file);
json_value_t parse_array(json_file_t *file);
json_value_t parse_object(json_file_t *file);
json_value_t parse_string(json_file_t *file);
json_value_t parse_number(json_file_t *file);
json_value_t parse_bool(json_file_t *file);
json_value_t parse_null(json_file_t *file);

char *parse_key(json_file_t *file);

void skip_whitespace(json_file_t *file);
bool expect_char(json_file_t *file, char expected);

/* arrays */
json_array_t *json_array_new(size_t capacity);
bool json_array_push(json_array_t *array, json_value_t value);
void json_array_free(json_array_t *array);

/* objects */
json_object_t *json_object_new(size_t capacity);
bool json_object_push(json_object_t *object, json_pair_t pair);
void json_object_free(json_object_t *object);

/* free */
void json_value_free(json_value_t *value);
