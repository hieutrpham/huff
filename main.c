#include "main.h"
#include <stdio.h>

int main(int ac, char **av) {
	if (ac != 2) {
		fprintf(stderr, "Usage: ./main <file_name>\n");
		return ARG_ERR;
	}

	const char *file_name = av[1];
	size_t file_size = get_file_size(file_name);
	char *buf = read_entire_file(file_name);

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
	Maps maps = {0};
	fill_maps(tree, &maps);

	StaticString str_map = {0};
	stringify_mapping(&maps, &str_map);
	// printf("%.*s", str_map.len, str_map.items);

	StaticString encoded_file = {0};
	fill_encoded_file(file_size, &maps, &encoded_file, buf);

	FILE *outfile = get_compress_file(file_name);
	compress(&str_map, &encoded_file, outfile);

	free(buf);
	fclose(outfile);
}



void stringify_mapping(Maps *maps, StaticString *str_map)
{
	for (int i = 0; i < maps->len; ++i)
	{
		str_map->items[str_map->len++] = maps->maps[i].c;
		unsigned char *encoded_str = maps->maps[i].encoded_str;
		for (int j = 0; encoded_str[j] != '\0'; ++j)
		{
			str_map->items[str_map->len] = encoded_str[j];
			str_map->len++;
		}
		str_map->items[str_map->len++] = '\r';
	}
}

void print_map(Maps *maps)
{
	for (int i = 0; i < maps->len; ++i)
	{
		printf("%d%s\r", maps->maps[i].c, maps->maps[i].encoded_str);
	}
}

void fill_maps(TreeNode *tree, Maps *maps)
{
	char str[32] = {0};
	populate_map(tree, str, maps, 0);
}

void fill_encoded_file(size_t file_size, Maps *maps, StaticString *encoded_file, char *buf)
{
	for (int i = 0; i < file_size; ++i)
	{
		for (int j = 0; j < maps->len; ++j)
		{
			if (buf[i] == maps->maps[j].c)
			{
				size_t length = strlen((const char*)maps->maps[j].encoded_str);
				memcpy(
					&encoded_file->items[encoded_file->len],
					maps->maps[j].encoded_str,
					length);
				encoded_file->len += length;
			}
		}
	}

}

FILE *get_compress_file(const char *file_name)
{
	const char ext[] = "_compressed";
	size_t compressed_file_size = strlen(file_name) + strlen(ext) + 1;
	char *compressed_file_name = malloc(compressed_file_size);
	if (!compressed_file_name)
	{
		printf("%m");
		exit(MALLOC_ERR);
	}

	strcpy(compressed_file_name, file_name);
	strcat(compressed_file_name, ext);

	FILE *outfile = fopen(compressed_file_name, "wb");
	if (!outfile) {
		printf("%m");
		exit(OPEN_ERR);
	}
	free(compressed_file_name);
	return outfile;
}

void compress(StaticString *str_map, StaticString *encoded_file, FILE *outfile)
{
	fwrite(str_map->items, 1, str_map->len, outfile);
	char r = '\r';
	fwrite(&r, 1, 1, outfile);

	unsigned char byte = 0;
	for (int i = 0; i < encoded_file->len; i += 8)
	{
		for (int j = i; j < i + 8; j++)
		{
			char bit = encoded_file->items[j];
			if (bit == '1')
				byte |= 1 << (7 - j + i);
			else if (bit == '0')
				byte |= 0 << (7 - j + i);
		}
		fwrite(&byte, 1, 1, outfile);
		byte = 0;
	}
	// printf("Compression done!\n");
}

size_t get_file_size(const char *file_name)
{
	struct stat fs;

	int file_stat = stat(file_name, &fs);
	if (file_stat < 0) {
		fprintf(stderr, "ERR: stat file failed\n");
		return STAT_ERR;
	}
	return fs.st_size;
}

char *read_entire_file(const char *file_name)
{
	size_t file_size = get_file_size(file_name);

	int file_fd = open(file_name, O_RDONLY);
	if (file_fd < 0)
	{
		fprintf(stderr, "ERR: open file failed\n");
		exit(OPEN_ERR);
	}

	char *buf = calloc(file_size + 1, sizeof(*buf));
	if (!buf)
	{
		fprintf(stderr, "ERR: memory allocation failed\n");
		exit(MALLOC_ERR);
	}

	ssize_t bytes_read = read(file_fd, buf, file_size);
	if (bytes_read < 0)
	{
		fprintf(stderr, "ERR: read file failed\n");
		exit(READ_ERR);
	}
	close(file_fd);
	return buf;
}
