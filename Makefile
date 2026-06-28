CFLAGS = -Wall -Werror -Wextra
CC = c++ -std=c++17
# CC = cc

all: main

main: main.c list.c freq_table.c tree.c arena.c string.c
	$(CC) $(CFLAGS) -g -o $@ $^

graph: main.c list.c freq_table.c tree.c
	$(CC) $(CFLAGS) -DGRAPH -g -o $@ $^

test_all: test_list test_tree

test_list: test_queue.c list.c freq_table.c
	$(CC) -g -o $@ $^
	./test_list

test_tree: test_tree.c list.c tree.c freq_table.c
	$(CC) -g -o $@ $^
	./test_tree
