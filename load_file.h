#pragma once
#include <stddef.h>

typedef struct {
  char *start;
  char *cursor;
  char *end;
} json_file_t;


json_file_t load_json_file(const char *filename);
