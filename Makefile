all: main

main: main.c list.c freq_table.c tree.c
	cc -g -o $@ $^

test_list: test_queue.c list.c freq_table.c
	cc -g -o $@ $^
	./test_list

test_tree: test_tree.c list.c tree.c freq_table.c
	cc -g -o $@ $^
	./test_tree
