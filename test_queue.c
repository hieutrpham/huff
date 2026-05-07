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
		enqueue_node(q, new);
		Node *back = q->back;
		Node *front = q->front;
		assert(back->value.c == 'o');
		assert(back->value.freq == 100);
		assert(back->next == NULL);

		assert(front->value.c == 'o');
		assert(front->value.freq == 100);
		assert(front->next == NULL);

		enqueue_node(q, new1);
		back = q->back;
		front = q->front;
		assert(back->value.c == '#');
		assert(back->value.freq == 200);
		assert(front->value.c == 'o');
		assert(front->value.freq == 100);

		Node *n = create_new_node(make_freq_test('i', 300));
		enqueue_node(q, n);
		back = q->back;
		front = q->front;
		assert(back->value.c == 'i');

		Node *f = dequeue_node(q);
		back = q->back;
		front = q->front;
		// q has only 1 node left
		assert(f->value.c == 'o');
		assert(front->value.c == '#');
		assert(back->value.c == 'i');
		assert(front->next == NULL);
		Node *f1 = dequeue_node(q);
		back = q->back;
		front = q->front;
		assert(f1->value.c == '#');
		assert(front->value.c == 'i');
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
		Node *back = q->back;
		Node *front = q->front;
		assert(back->value.c == 'o');
		assert(front->value.c == 'u');

		Node *new = create_new_node((c_freq){.c = 'j', .freq = 100});
		enqueue_node(q, new);
		back = q->back;
		front = q->front;

		Node *new1 = create_new_node((c_freq){.c = '#', .freq = 201});
		assert(back->value.c == 'j');
		assert(back->value.freq == 100);
		enqueue_node(q, new1);
		back = q->back;
		front = q->front;
		assert(back->value.c == '#');
		assert(back->value.freq == 201);
		Node *f = dequeue_node(q);
		assert(f->value.c == 'u');
		Node *f1 = dequeue_node(q);
		assert(f1->value.c == 'i');
		free_queue(q);
	}
	fprintf(stderr, "LOG: all test_queue passed!\n");
}
