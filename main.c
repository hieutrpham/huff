#include "main.h"

void print_table(hist_arr table) {
	for (int i = 0; i < table.count; ++i) {
		if (table.hist[i].c != '\n')
			printf("char: %c, freq: %zu\n", table.hist[i].c, table.hist[i].freq);
		else
			printf("new line freq: %zu\n", table.hist[i].freq);
	}
}

bool exist(char c, hist_arr hist_a) {
	for (int i = 0; i < hist_a.count; ++i) {
		if (hist_a.hist[i].c == c)
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

	printf("file %s contains: %s\n", file_name, buf);

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
				if (freq_table.hist[j].c == buf[i])
					freq_table.hist[j].freq++;
			}
		}
		else
			freq_table.hist[freq_table.count++] = (c_freq){.c = buf[i], .freq = 1};
	}

	// NOTE: sort the frequency table descendingly
	qsort(freq_table.hist, freq_table.count, sizeof(freq_table.hist[0]), compar);

	printf("-------------------------------\n");
	print_table(freq_table);

	// TODO: implement a priority queue
	// * linked list version:
	// * loop the freq_table again and create a note for each histogram
	close(file_fd);
}
