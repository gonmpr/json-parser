all:
	gcc -Wall -Wextra -Werror -std=c99 main.c load_file.c parser.c -o jparse
