all: main

main: main.c list.c freq_table.c
	cc -o $@ $^

test: test.c list.c freq_table.c
	cc -o $@ $^
