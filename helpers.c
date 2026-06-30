#include "helpers.h"

long search_char(json_file_t json_file, char symbol){
  for(long pos_offset = 0; pos_offset < json_file.size; pos_offset++){
    if (*(json_file+pos_offset) == symbol) return pos_offset;
  }
  return (long)(-1);

}
