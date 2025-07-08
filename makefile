make:
	gcc mygrep.c -o mygrep -Wall -Wextra -fsanitize=address -g