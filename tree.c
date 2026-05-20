#include "main.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

TreeNode *init_tree_node() {
	TreeNode *n = malloc(sizeof(*n));
	assert(n);

	static size_t tree_node_id = 0;
	n->l = NULL;
	n->r = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->weight = 0;
	n->c = 0;
	tree_node_id++;
	n->id = tree_node_id;
	return n;
}

void enqueue_tree(Queue *q, TreeNode *n) {
	assert(n);

	if (is_empty(q)) {
		q->back = n;
		q->front = n;
		q->len++;
		return;
	}

	TreeNode *old_back = q->back;
	n->next = old_back; // new node is back of queue
	q->back = n;
	old_back->prev = n; // back of queue prev node is new node
	if (q->len <= 1)
		q->front = old_back; // front of queue is back if queue len <= 1
	q->len++;
}

TreeNode* dequeue_tree(Queue *q) {
	assert(q);
	assert(q->len > 0);
	TreeNode* front = q->front;
	assert(front);
	TreeNode* back = q->back;
	assert(back);

	if (q->len == 1) {
		assert(q->front == q->back);
		TreeNode *n = q->back;
		q->back = NULL;
		q->front = NULL;
		q->len--;
		return n;
	}

	TreeNode *new_front = front->prev;
	q->front = new_front;
	q->len--;
	return front;
}

TreeNode *create_tree_node_from_list_node(Node* n) {
	TreeNode *tree_n = init_tree_node();
	tree_n->weight = n->value.freq;
	tree_n->c = n->value.c;
	return tree_n;
}

void pop_first_two_initial(Queue *initial, Queue* combined) {
	Node *i_node = dequeue_list(initial);
	TreeNode *new_tree_node = init_tree_node();
	new_tree_node->l = create_tree_node_from_list_node(i_node);

	i_node = dequeue_list(initial);
	new_tree_node->r = create_tree_node_from_list_node(i_node);
	new_tree_node->weight = new_tree_node->l->weight + new_tree_node->r->weight;

	enqueue_tree(combined, new_tree_node);
}

void pop_first_two_combined(Queue *initial, Queue* combined) {
	TreeNode *new_tree_node = init_tree_node();
	TreeNode *temp = dequeue_tree(combined);
	new_tree_node->l = temp;

	temp = dequeue_tree(combined);
	new_tree_node->r = temp;
	new_tree_node->weight = new_tree_node->l->weight + new_tree_node->r->weight;
	enqueue_tree(combined, new_tree_node);
}

void pop_each(Queue *initial, Queue* combined) {
	TreeNode *new_tree_node = init_tree_node();
	Node *temp = dequeue_list(initial);
	new_tree_node->l = create_tree_node_from_list_node(temp);

	TreeNode* temp_tree = dequeue_tree(combined);
	new_tree_node->r = temp_tree;
	new_tree_node->weight = new_tree_node->l->weight + new_tree_node->r->weight;
	enqueue_tree(combined, new_tree_node);
}

void graph_tree(TreeNode* root)
{
#ifdef GRAPH
	printf("graph {\n");
	printf("node [shape=record]\n");
	printf("%zu -- {%zu %zu}\n", root->id, root->l->id, root->r->id);
#endif
	print_tree(root, 0);

#ifdef GRAPH
	printf("}\n");
#endif
}

void print_tree(TreeNode* root, int level)
{
	TreeNode *temp = root;

	if (!temp)
		return;
	if (temp->l) {
#ifdef GRAPH
			printf("%zu -- {", temp->l->id);
			if (temp->l->l)
				printf("%zu ", temp->l->l->id);
			if (temp->l->r)
				printf("%zu", temp->l->r->id);
			printf("}\n");
			if (temp->l->c) {
				printf("%zu ", temp->l->id);
				printf("[color=red, label=\"{%d|%zu}\"]\n", temp->l->c, temp->l->weight);
			}
			print_tree(temp->l, level + 1);
#else
			for (int i = 0; i < level; ++i)
				printf("    ");
			printf("char: %d, weight: %zu, level: %d\n", temp->l->c, temp->l->weight, level);
			print_tree(temp->l, level + 1);
#endif
	}
	if (temp->r) {
#ifdef GRAPH
			printf("%zu -- {", temp->r->id);
			if (temp->r->l)
				printf("%zu ", temp->r->l->id);
			if (temp->r->r)
				printf("%zu", temp->r->r->id);
			printf("}\n");
			if (temp->r->c) {
				printf("%zu ", temp->r->id);
				printf("[color=red, label=\"{%d|%zu}\"]\n", temp->r->c, temp->r->weight);
			}
			print_tree(temp->r, level + 1);
#else
			for (int i = 0; i < level; ++i)
				printf("    ");
			printf("char: %d, weight: %zu, level: %d\n", temp->r->c, temp->r->weight, level);
			print_tree(temp->r, level + 1);
#endif
		}
}

TreeNode *build_huffman_tree(Queue *initial, Queue* combined) {
	while (initial->len + combined->len >= 2) {
		if (is_empty(combined)) {
			pop_first_two_initial(initial, combined);
			if (is_empty(initial))
				break;
		}

		if (is_empty(initial)) {
			pop_first_two_combined(initial, combined);
			if (combined->len == 1)
				break;
		}

		assert(!(is_empty(initial) && is_empty(combined)));

		// NOTE: find 2 nodes with the lowest weights
		TreeNode *combined_front = combined->front;
		Node *initial_front = initial->front;
		if (initial->len >= 2 && combined_front->weight >= initial_front->value.freq && combined_front->weight >= initial_front->prev->value.freq) {
			pop_first_two_initial(initial, combined);
		} else if (initial->len > 0 && combined->len > 0) {
			pop_each(initial, combined);
		}
	}

	assert(is_empty(initial));
	assert(combined->len + initial->len == 1);
	return dequeue_tree(combined);
}
