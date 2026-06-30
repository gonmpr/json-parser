#include "parser.h"

void parse_json_object();
void parse_key();
void parse_value();

// especialistas
void parse_array();
void parse_string();
void parse_number();
void parse_bool();
void parse_null();

//utils
void skip_whitespace();
void expect_char(char c);

