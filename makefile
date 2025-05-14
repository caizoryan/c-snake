build: main.c
	clang -lncurses main.c cJSON.c -o build && ./build
