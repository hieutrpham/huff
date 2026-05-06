all: main

main: main.c list.c freq_table.c tree.c
	cc -g -o $@ $^

test: test_queue.c list.c freq_table.c
	cc -g -o $@ $^
