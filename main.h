#pragma once

#include <stddef.h>
#include <stdint.h>
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
#define ERR(err)                                          \
	do {                                                  \
		fprintf(stderr, "%s:%d:%m\n", __FILE__, __LINE__);\
		exit(err);                                        \
	} while(0)

#define ARENA_CAP 1024*1024
#define STRING_CAP 1024

typedef uint8_t boolean;

enum {
	ARG_ERR = 1,
	OPEN_ERR,
	READ_ERR,
	STAT_ERR,
	MALLOC_ERR,
};

struct arena {
	uint8_t  *items;
	uint32_t len;
	uint32_t cap;
};

typedef struct {
	char *items;
	uint32_t len;
	uint32_t cap;
} String;

typedef struct {
	uint32_t freq;
	uint8_t  c;
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
	uint8_t c;
	size_t id;
} TreeNode;

typedef struct {
	char c;
	char encoded_str[256];
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

boolean       exist(uint8_t c, hist_arr *hist_a);
boolean       is_empty(Queue *);
c_freq        make_freq_test(int, int);
uint8_t       *read_entire_file(size_t file_size, const char *);
int           compar(const void *, const void *);
Node          *create_new_node(struct arena *a, c_freq);
Node*         dequeue_list(Queue *);
Queue         *build_queue_from_table(struct arena *a, hist_arr*);
Queue         *init_queue(struct arena *a);
size_t        get_file_size(const char *);
TreeNode      *build_huffman_tree(struct arena *a, Queue *, Queue*);
void          compress(hist_arr *, String *, FILE *);
void          enqueue_list(Queue *, Node *);
void          fill_encoded_file(size_t, Maps*, String*, uint8_t*);
void          fill_maps(TreeNode *, Maps *);
void          free_list(Node *);
void          free_queue(Queue *);
void          graph_tree(TreeNode*);
void          populate_map(TreeNode*, char *, Maps *, int);
void          populate_table(uint8_t *, hist_arr *);
void          print_list(Node *);
void          print_map(Maps *);
void          print_queue(const Queue*, const char*);
void          print_table(hist_arr*);
void          print_tree(TreeNode*, int);
void          stringify_mapping(Maps *, String *);
uint32_t      parse_header(const uint8_t *buf, hist_arr *freq_table);
TreeNode      *get_tree(struct arena *a, hist_arr *freq_table);
FILE          *build_outfile(const char *file_name);
void          decompress(TreeNode *tree, const uint8_t *buf, uint curr_ptr, size_t file_size, FILE *outfile);
struct arena  *arena_init(uint32_t capacity);
void          arena_destroy(struct arena* a);
void          *arena_malloc(struct arena* a, uint32_t size);
void          string_append(String *str, const char *ext);
void          string_destroy(String *str);
String        *string_init(uint32_t size);
char          *build_outfile_name(const char *file_name, const char *ext);
void          report(size_t file_size, size_t outfile_size);
