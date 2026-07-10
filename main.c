#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"
#include "parser.h"

static void print_json_value(json_value_t *value);

static void print_json_array(json_array_t *array)
{
    if (!array) {
        printf("[]");
        return;
    }

    printf("[");
    for (size_t i = 0; i < array->count; i++) {
        print_json_value(&array->items[i]);

        if (i + 1 < array->count) {
            printf(", ");
        }
    }
    printf("]");
}

static void print_json_object(json_object_t *object)
{
    if (!object) {
        printf("{}");
        return;
    }

    printf("{");
    for (size_t i = 0; i < object->count; i++) {
        printf("\"%s\": ", object->pairs[i].key);
        print_json_value(&object->pairs[i].value);

        if (i + 1 < object->count) {
            printf(", ");
        }
    }
    printf("}");
}

static void print_json_value(json_value_t *value)
{
    if (!value) {
        printf("<null pointer>");
        return;
    }

    switch (value->type) {
        case JSON_NULL:
            printf("null");
            break;

        case JSON_BOOL:
            printf("%s", value->boolean ? "true" : "false");
            break;

        case JSON_NUMBER:
            printf("%.17g", value->number);
            break;

        case JSON_STRING:
            printf("\"%s\"", value->string);
            break;

        case JSON_ARRAY:
            print_json_array(value->array);
            break;

        case JSON_OBJECT:
            print_json_object(value->object);
            break;

        case JSON_PARSING_ERROR:
            printf("<JSON_PARSING_ERROR>");
            break;
    }
}

int main(void){
  json_file_t json_file = load_json_file("json_test.json");
  if(!json_file.start){ 
    printf("json not found\n"); 
    return 1;
  }

  printf("json loaded successfully\n");

  json_value_t json_obj = parse_value(&json_file);

  print_json_value(&json_obj);
  printf("\n");

  json_value_free(&json_obj);
  free(json_file.start);
  return 0;
}
