#include "load_file.h"
#include <stdio.h>
#include <stdlib.h>

size_t get_file_size(FILE *fp){
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp); 
  rewind(fp);
  return (size_t)size;
}

json_file_t load_json_content(FILE *fp, size_t file_size) {

  json_file_t loaded_file = {0};
  if (!fp) {
    return loaded_file; //          couldn't open the file
  }

  unsigned char *buffer = malloc(file_size);
  if (!buffer) {
    return loaded_file; //          couldn't locate memory
  }

  size_t bytes_read = fread(buffer, 1, file_size, fp);
  
  loaded_file.start = buffer; 
  loaded_file.offset = 0; 
  loaded_file.size = bytes_read;

  return loaded_file;
}


json_file_t load_json_file(char *filename){
  FILE *fp = fopen(filename, "rb");

  if (!fp){ //TO DO: implement a logs systems
    printf("Error: File doesn't exist\n");
    json_file_t empty = {0};
    empty.start = NULL;
    return empty;
  }

  size_t file_size = get_file_size(fp);
  json_file_t json_file = load_json_content(fp, file_size);
  fclose(fp);

  return json_file;
}
