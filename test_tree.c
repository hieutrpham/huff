#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	{
		Queue *initial = init_queue();
		Node *new = create_new_node(make_freq_test('o', 1));
		Node *new1 = create_new_node(make_freq_test('#', 2));
		enqueue_list(initial, new);
		enqueue_list(initial, new1);
		assert(initial->len == 2);
		Queue *combined = init_queue();
		assert(combined->len == 0);
		TreeNode *tree = build_huffman_tree(initial, combined);
		assert(is_empty(initial));
		assert(is_empty(combined));
		assert(tree->weight == 3);
		assert(tree->l->weight == 1);
		assert(tree->r->weight == 2);
	}
	{
		Queue *initial = init_queue();
		Node *new = create_new_node(make_freq_test('o', 1));
		Node *new1 = create_new_node(make_freq_test('o', 1));
		Node *new2 = create_new_node(make_freq_test('#', 2));
		Node *new3 = create_new_node(make_freq_test('i', 3));
		enqueue_list(initial, new);
		enqueue_list(initial, new1);
		enqueue_list(initial, new2);
		enqueue_list(initial, new3);

		// 3 -> 2 -> 1 -> 1
		Queue *combined = init_queue();
		assert(combined->len == 0);
		TreeNode *tree = build_huffman_tree(initial, combined);
		assert(is_empty(initial));
		assert(is_empty(combined));
		assert(tree->weight == 7);
		assert(tree->l);
		assert(tree->r);
		assert(tree->l->weight == 3);
		assert(tree->r->weight == 4);
		assert(tree->r->l);
		assert(tree->r->l->weight == 2);
		assert(tree->r->r->weight == 2);
	}
	fprintf(stderr, "LOG: all test_tree passed!\n");
	return 0;
}
