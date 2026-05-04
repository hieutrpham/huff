#include "main.h"

void print_table(hist_arr table) {
	for (int i = 0; i < table.count; ++i) {
		if (table.items[i].c != '\n') // skip new line since it will mess up the stdout
			printf("char: %c, freq: %zu\n", table.items[i].c, table.items[i].freq);
		else
			printf("new line freq: %zu\n", table.items[i].freq);
	}
}

bool exist(char c, hist_arr hist_a) {
	for (int i = 0; i < hist_a.count; ++i) {
		if (hist_a.items[i].c == c)
			return true;
	}
	return false;
}

int compar(const void *one, const void *two) {
	const c_freq *c1 = one;
	const c_freq *c2 = two;
	return c1->freq <= c2->freq;
}

int main(int ac, char **av) {
	if (ac != 2) {
		fprintf(stderr, "Usage: <./main> <file name>\n");
		return ARG_ERR;
	}
	char *file_name = av[1];
	int file_fd = open(file_name, O_RDONLY);
	if (file_fd < 0) {
		fprintf(stderr, "ERR: open file failed\n");
		return OPEN_ERR;
	}

	// read the file content in a buffer TODO: make it read any size file
	char buf[4096];
	memset(buf, 0, sizeof(buf));
	ssize_t bytes_read = read(file_fd, buf, sizeof(buf));
	if (bytes_read < 0) {
		fprintf(stderr, "ERR: read file failed\n");
		return READ_ERR;
	}

	struct stat fs;
	int file_stat = stat(file_name, &fs);
	if (file_stat < 0) {
		fprintf(stderr, "ERR: stat file failed\n");
		return STAT_ERR;
	}

	// NOTE: getting the frequency table
	hist_arr freq_table = {0};

	for (int i = 0; buf[i]; ++i) {
		if (exist(buf[i], freq_table)) {
			for (int j = 0; j < freq_table.count; ++j) {
				if (freq_table.items[j].c == buf[i])
					freq_table.items[j].freq++;
			}
		}
		else
			freq_table.items[freq_table.count++] = (c_freq){.c = buf[i], .freq = 1};
	}

	// NOTE: sort the frequency table descendingly
	qsort(freq_table.items, freq_table.count, sizeof(freq_table.items[0]), compar);

	// printf("-------------------------------\n");
	// print_table(freq_table);

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
	close(file_fd);
}
