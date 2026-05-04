#include "main.h"

int main() {
	{
		hist_arr freq_table = {0};
		char buf[] = "oooiiu";
		populate_table(buf, &freq_table);
		assert(freq_table.count > 0);
		Queue *q = build_queue(freq_table);
		assert(q);
		assert(q->back);
		assert(q->front);
		assert(q->back->value.c == 'o');
		assert(q->front->value.c == 'u');

		Node *new = create_new_node((c_freq){.c = 'o', .freq = 100});
		assert(new);
		Node *new1 = create_new_node((c_freq){.c = '#', .freq = 201});
		assert(new1);
		queue(q, new);
		assert(q->back->value.c == 'o');
		assert(q->back->value.freq == 100);
		queue(q, new1);
		assert(q->back->value.c == '#');
		assert(q->back->value.freq == 201);
		Node *f = dequeue(q);
		assert(f->value.c == 'u');
		Node *f1 = dequeue(q);
		assert(f1->value.c == 'i');
		free_queue(q);
		fprintf(stderr, "LOG: all tests passed!\n");
	}
}
