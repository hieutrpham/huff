#include "main.h"

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

	hist_arr freq_table = {0};
	populate_table(buf, &freq_table);
	// print_table(freq_table);

	Queue *initial_q = build_queue_from_table(freq_table);
	Queue *combine_q = init_queue();
	print_queue(initial_q);

	TreeNode *tree = build_huffman_tree(initial_q, combine_q);
	close(file_fd);
}
