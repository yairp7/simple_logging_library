all:
	gcc -g -fPIC -Wall -Werror -Wextra -pedantic logging.c -shared -o bin/liblogging.so
	gcc -g tester.c -o bin/tester