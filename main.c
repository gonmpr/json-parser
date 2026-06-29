#include <stdio.h>
#include <stdlib.h>
#include "load_file.h"

int main(void){
  json_file_t json_file = load_json_file("json_test.json");
  if(!json_file.start){ printf("json not found\n"); }

  printf("json loaded successfully\n");

  free(json_file.start);
  return 0;

}
