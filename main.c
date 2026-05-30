#include "main.h"
#include <stdio.h>

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

	char *buf = calloc(file_size + 1, sizeof(*buf));
	if (!buf) {
		fprintf(stderr, "ERR: memory allocation failed\n");
		return MALLOC_ERR;
	}

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
	if (!tree) {
		fprintf(stderr, "ERR: invalid tree\n");
		return MALLOC_ERR;
	}

	// graph_tree(tree);
	char str[32] = {0};
	Maps maps = {0};
	populate_map(tree, str, &maps, 0);

	// TODO: use dynamic array instead
	StaticString encoded_file = {0};

	for (int i = 0; i < file_size; ++i) {
		for (int j = 0; j < maps.len; ++j) {
			if (buf[i] == maps.maps[j].c) {
				size_t length = strlen((const char*)maps.maps[j].encoded_str);
				memcpy(
					&encoded_file.items[encoded_file.len],
					maps.maps[j].encoded_str,
					length);
				encoded_file.len += length;
			}
		}
	}

	char ext[] = "_compressed";
	size_t compressed_file_size = strlen(file_name) + strlen(ext) + 1;
	char *compressed_file_name = calloc(compressed_file_size, 1);
	if (!compressed_file_name)
	{
		printf("%m");
		return MALLOC_ERR;
	}

	strcpy(compressed_file_name, file_name);
	strcat(compressed_file_name, ext);

	FILE *outfile = fopen(compressed_file_name, "wb");
	if (!outfile) {
		printf("%m");
		return OPEN_ERR;
	}

	unsigned char byte = 0;
	for (int i = 0; i < encoded_file.len; i += 8) {
		for (int j = i; j < i + 8; j++) {
			char bit = encoded_file.items[j];
			if (bit == '1')
				byte |= 1 << (7 - j + i);
			else if (bit == '0')
				byte |= 0 << (7 - j + i);
		}
		size_t byte_write = fwrite(&byte, 1, 1, outfile);
		byte = 0;
	}

	free(buf);
	free(compressed_file_name);
	fclose(outfile);
	close(file_fd);
}
