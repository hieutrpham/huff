#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_COUNT 256

enum {
	ARG_ERR = 1,
	OPEN_ERR,
	READ_ERR,
	STAT_ERR,
	MALLOC_ERR,
};

typedef struct {
	char c;
	size_t freq;
} c_freq;

typedef struct {
	c_freq items[MAX_COUNT];
	size_t count;
} hist_arr;

typedef struct Node {
	c_freq value;
	struct Node *next;
	struct Node *prev;
} Node;

Node *build_queue(hist_arr freq_table);
void free_list(Node *root);
void print_list(Node *root);
