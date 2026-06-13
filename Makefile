CFLAGS = -Wall -Werror -Wextra

all: main

main: main.c list.c freq_table.c tree.c arena.c
	cc $(CFLAGS) -g -o $@ $^

graph: main.c list.c freq_table.c tree.c
	cc $(CFLAGS) -DGRAPH -g -o $@ $^

test_all: test_list test_tree

test_list: test_queue.c list.c freq_table.c
	cc -g -o $@ $^
	./test_list

test_tree: test_tree.c list.c tree.c freq_table.c
	cc -g -o $@ $^
	./test_tree
