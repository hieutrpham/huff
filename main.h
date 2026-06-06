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
#define MAX_LEN 4096*100
#define ERR(err)           \
	do {                   \
		fprintf(stderr, "%m\n");\
		exit(err);         \
	} while(0)

typedef char boolean;

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
	uint  freq;
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

typedef struct {
	unsigned char c;
	unsigned char encoded_str[256];
} Map;

typedef struct {
	Map maps[256];
	size_t len;
} Maps;

typedef struct Queue {
	void    *back;
	void    *front;
	size_t  len;
} Queue;

boolean   exist(char, hist_arr*);
boolean   is_empty(Queue *);
c_freq    make_freq_test(int, int);
char      *read_entire_file(size_t file_size, const char *);
int       compar(const void *, const void *);
Node      *create_new_node(c_freq);
Node*     dequeue_list(Queue *);
Queue     *build_queue_from_table(hist_arr*);
Queue     *init_queue();
size_t    get_file_size(const char *);
TreeNode  *build_huffman_tree(Queue *, Queue*);
void      compress(hist_arr *, StaticString *, FILE *);
void      enqueue_list(Queue *, Node *);
void      fill_encoded_file(size_t, Maps*, StaticString*, char*);
void      fill_maps(TreeNode *, Maps *);
void      free_list(Node *);
void      free_queue(Queue *);
void      graph_tree(TreeNode*);
void      populate_map(TreeNode*, char *, Maps *, int);
void      populate_table(char *, hist_arr *);
void      print_list(Node *);
void      print_map(Maps *);
void      print_queue(const Queue*, const char*);
void      print_table(hist_arr);
void      print_tree(TreeNode*, int);
void      stringify_mapping(Maps *, StaticString *);
uint      parse_header(const char *buf, hist_arr *freq_table);
TreeNode  *get_tree(hist_arr *freq_table);
FILE      *build_outfile(const char *file_name, const char *ext);
void      decompress(TreeNode *tree, const char *buf, const uint curr_ptr, FILE *outfile);
