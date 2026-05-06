all: main

main: main.c list.c freq_table.c
	cc -g -o $@ $^

test: test.c list.c freq_table.c
	cc -g -o $@ $^
