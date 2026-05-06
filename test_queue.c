#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

c_freq make_freq_test(int c, int freq) {
	return (c_freq){.c = c, .freq = freq};
}

int main() {
	{
		Queue *q = init_queue();
		Node *new = create_new_node(make_freq_test('o', 100));
		Node *new1 = create_new_node(make_freq_test('#', 200));
		assert(new->next == NULL);
		enqueue(q, new);
		assert(q->back->value.c == 'o');
		assert(q->back->value.freq == 100);
		assert(q->back->next == NULL);

		assert(q->front->value.c == 'o');
		assert(q->front->value.freq == 100);
		assert(q->front->next == NULL);

		enqueue(q, new1);
		assert(q->back->value.c == '#');
		assert(q->back->value.freq == 200);
		assert(q->front->value.c == 'o');
		assert(q->front->value.freq == 100);

		// i -> # -> o
		Node *n = create_new_node(make_freq_test('i', 300));
		enqueue(q, n);
		assert(q->back->value.c == 'i');

		Node *f = dequeue(q);
		// q has only 1 node left
		assert(f->value.c == 'o');
		assert(q->front->value.c == '#');
		assert(q->back->value.c == 'i');
		assert(q->front->next == NULL);
		Node *f1 = dequeue(q);
		assert(f1->value.c == '#');
		assert(q->front->value.c == 'i');
		free(f);
		free(f1);
		// free(q);
		free_queue(q);
	}
	{
		hist_arr freq_table = {0};
		char buf[] = "oooiiu";
		populate_table(buf, &freq_table);
		assert(freq_table.count > 0);
		Queue *q = build_queue_from_table(freq_table);
		assert(q->back->value.c == 'o');
		assert(q->front->value.c == 'u');

		Node *new = create_new_node((c_freq){.c = 'j', .freq = 100});
		enqueue(q, new);

		Node *new1 = create_new_node((c_freq){.c = '#', .freq = 201});
		assert(q->back->value.c == 'j');
		assert(q->back->value.freq == 100);
		enqueue(q, new1);
		assert(q->back->value.c == '#');
		assert(q->back->value.freq == 201);
		Node *f = dequeue(q);
		assert(f->value.c == 'u');
		Node *f1 = dequeue(q);
		assert(f1->value.c == 'i');
		free_queue(q);
	}
	fprintf(stderr, "LOG: all test_queue passed!\n");
}
