#pragma once
#include "globals.h"
#include <stddef.h>

typedef struct {
  u8 *start;
  size_t offset;
  size_t size;
} json_file_t;


json_file_t load_json_file(char *filename);
