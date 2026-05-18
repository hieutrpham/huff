#include "main.h"

size_t get_file_size(const char *file_name) {
	struct stat fs;

	int file_stat = stat(file_name, &fs);
	if (file_stat < 0) {
		fprintf(stderr, "ERR: stat file failed\n");
		return STAT_ERR;
	}
	return fs.st_size;
}

int main(int ac, char **av) {
	if (ac != 2) {
		fprintf(stderr, "Usage: ./main <file_name>\n");
		return ARG_ERR;
	}

	const char *file_name = av[1];
	size_t file_size = get_file_size(file_name);

	int file_fd = open(file_name, O_RDONLY);
	if (file_fd < 0) {
		fprintf(stderr, "ERR: open file failed\n");
		return OPEN_ERR;
	}

	char *buf = calloc(file_size, sizeof(*buf));
	ssize_t bytes_read = read(file_fd, buf, file_size);
	if (bytes_read < 0) {
		fprintf(stderr, "ERR: read file failed\n");
		return READ_ERR;
	}

	hist_arr freq_table = {0};
	populate_table(buf, &freq_table);
	// print_table(freq_table);

	Queue *initial_q = build_queue_from_table(freq_table);
	Queue *combine_q = init_queue();
	// print_queue(initial_q, "initial");

	TreeNode *tree = build_huffman_tree(initial_q, combine_q);
	graph_tree(tree);
	// print_tree(tree);
	close(file_fd);
}
