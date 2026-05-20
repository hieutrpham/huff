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
#define MAX_LEN 4096

enum {
	ARG_ERR = 1,
	OPEN_ERR,
	READ_ERR,
	STAT_ERR,
	MALLOC_ERR,
};

typedef struct {
	char items[MAX_LEN];
	int len;
} StaticString;

typedef struct {
	char *items;
	int len;
	int cap;
} String;

typedef struct {
	char    c;
	size_t  freq;
} c_freq;

typedef struct {
	c_freq items[MAX_COUNT];
	size_t count;
} hist_arr;

typedef struct Node {
	struct Node *next;
	struct Node *prev;
	c_freq value;
} Node;

typedef struct TreeNode {
	struct TreeNode *l;
	struct TreeNode *r;
	struct TreeNode *next;
	struct TreeNode *prev;
	size_t weight;
	char c;
	size_t id;
} TreeNode;

typedef struct Queue {
	void    *back;
	void    *front;
	size_t  len;
} Queue;

Queue     *build_queue_from_table(hist_arr freq_table);
void      free_list(Node *root);
void      free_queue(Queue *q);
void      print_list(Node *root);
Node      *create_new_node(c_freq v);
void      enqueue_list(Queue *q, Node *n);
Node*     dequeue_list(Queue *q);
bool      exist(char c, hist_arr hist_a);
void      populate_table(char *buf, hist_arr *freq_table);
void      print_table(hist_arr table);
int       compar(const void *one, const void *two);
Queue     *init_queue();
bool      is_empty(Queue *q);
TreeNode  *build_huffman_tree(Queue *initial, Queue* combined);
c_freq    make_freq_test(int c, int freq);
void      print_tree(TreeNode* root, int level);
void      print_queue(const Queue *queue, const char *str);
void      graph_tree(TreeNode* root);
