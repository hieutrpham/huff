#include "main.h"

int main() {
	{
		hist_arr freq_table = {0};
		Queue *q = build_queue(freq_table);
		print_list(q->back);
		printf("front: %c, back: %c\n", q->front->value.c, q->back->value.c);

		Node *new = create_new_node((c_freq){.c = 'o', .freq = 100});
		Node *new1 = create_new_node((c_freq){.c = '#', .freq = 201});
		queue(q, new);
		queue(q, new1);
		Node *f = dequeue(q);
		Node *f1 = dequeue(q);

		printf("-------------------------------\n");
		print_list(q->back);
		printf("front: %c, back: %c\n", q->front->value.c, q->back->value.c);
		free_queue(q);
	}
}
