#include "main.h"
#include <stdint.h>
#include <stdio.h>

int main(int ac, char **av)
{
	bool is_decompress = false;

	const char *file_name = NULL;
	if (ac == 3 && strcmp(av[1], "-d") == 0)
	{
		is_decompress = true;
		file_name = av[2];
	}
	else if (ac == 2)
	{
		is_decompress = false;
		file_name = av[1];
	}
	else
	{
		fprintf(stderr, "ERR: invalid usage");
		return ARG_ERR;
	}

	size_t file_size = get_file_size(file_name);
	uint8_t *buf = read_entire_file(file_size, file_name);

	hist_arr freq_table = {0};
	struct arena *a = arena_init(ARENA_CAP);
	if (!is_decompress)
	{
		populate_table(buf, &freq_table);
		TreeNode *tree = get_tree(a, &freq_table);
		Maps maps = {0};
		fill_maps(tree, &maps);

		StaticString encoded_file = {0};
		fill_encoded_file(file_size, &maps, &encoded_file, buf);

		FILE *outfile = build_outfile(file_name, "_compressed");
		compress(&freq_table, &encoded_file, outfile);
		fclose(outfile);
	}
	else
	{
		uint32_t curr_ptr = parse_header(buf, &freq_table);
		TreeNode *tree = get_tree(a, &freq_table);
		FILE *outfile = build_outfile(file_name, "_decompressed");
		decompress(tree, buf, curr_ptr, file_size, outfile);
		fclose(outfile);
	}

	free(buf);
	buf = NULL;
	arena_destroy(a);
}

void decompress(TreeNode *tree, const uint8_t *buf, uint curr_ptr, size_t file_size, FILE *outfile)
{
	TreeNode *tmp = tree;
	while (curr_ptr < file_size)
	{
		uint8_t c = buf[curr_ptr];
		for (int i = 0; i < 8; ++i)
		{
			if (c & (1 << (7 - i)))
				tmp = tmp->r;
			else
				tmp = tmp->l;

			if (tmp->c)
			{
				if (tmp->c == '\r')
				{
					return;
				}
				else
				{
					fwrite(&tmp->c, 1, 1, outfile);
					tmp = tree;
				}
			}
		}
		curr_ptr+=1;
	}
}

TreeNode *get_tree(struct arena *a, hist_arr *freq_table)
{
	Queue *initial_q = build_queue_from_table(a, freq_table);
	Queue *combine_q = init_queue(a);
	TreeNode *tree = build_huffman_tree(a, initial_q, combine_q);
	if (!tree)
		ERR(MALLOC_ERR);
	return tree;
}

// return pointer to the begin of content body
uint32_t parse_header(const uint8_t *buf, hist_arr *freq_table)
{
	uint32_t i = 0;

	while (true)
	{
		if (buf[i] == '\r' && buf[i+1] == '\r')
			break;
		c_freq *curr = &freq_table->items[freq_table->count];
		curr->c = buf[i];
		curr->freq = buf[i+4]<<24 | buf[i+3]<<16 | buf[i+2]<<8 | buf[i+1];
		freq_table->count++;
		i += 5;
	}
	return i + 2;
}

void fill_maps(TreeNode *tree, Maps *maps)
{
	char str[32] = {0};
	populate_map(tree, str, maps, 0);
}

// encoded_file is an array of 0 and 1 characters that intepret the original file
void fill_encoded_file(size_t file_size, Maps *maps, StaticString *encoded_file, uint8_t *buf)
{
	for (uint i = 0; i < file_size; ++i)
	{
		for (uint j = 0; j < maps->len; ++j)
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

	// add the pseudo EOF
	for (uint j = 0; j < maps->len; ++j)
	{
		if (maps->maps[j].c == '\r')
		{
			size_t length = strlen((const char*)maps->maps[j].encoded_str);
			memcpy(
				&encoded_file->items[encoded_file->len],
				maps->maps[j].encoded_str,
				length);
			encoded_file->len += length;
			memcpy(
				&encoded_file->items[encoded_file->len],
				maps->maps[j].encoded_str,
				length);
			encoded_file->len += length;
		}
	}
}

FILE *build_outfile(const char *file_name, const char *ext)
{
	size_t file_size = strlen(file_name) + strlen(ext) + 1;
	char *outfile_name = malloc(file_size);
	if (!outfile_name)
		ERR(MALLOC_ERR);

	strcpy(outfile_name, file_name);
	strcat(outfile_name, ext);

	FILE *outfile = fopen(outfile_name, "wb");
	if (!outfile)
		ERR(OPEN_ERR);

	free(outfile_name);
	outfile_name = NULL;
	return outfile;
}

void compress(hist_arr *freq_table, StaticString *encoded_file, FILE *outfile)
{
	for (uint i = 0; i < freq_table->count; ++i)
	{
		c_freq curr_item = freq_table->items[i];
		fwrite(&curr_item.c, 1, 1, outfile);
		fwrite(&curr_item.freq, sizeof(curr_item.freq), 1, outfile);
	}

	char sep = '\r';
	fwrite(&sep, 1, 1, outfile);
	fwrite(&sep, 1, 1, outfile);

	uint8_t byte = 0;
	for (uint i = 0; i < encoded_file->len; i += 8)
	{
		for (uint j = i; j < i + 8; j++)
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
	if (file_stat < 0)
		ERR(STAT_ERR);
	return fs.st_size;
}

uint8_t *read_entire_file(size_t file_size, const char *file_name)
{
	int file_fd = open(file_name, O_RDONLY);
	if (file_fd < 0)
		ERR(OPEN_ERR);

	uint8_t *buf = calloc(file_size + 1, sizeof(*buf));
	if (!buf)
		ERR(MALLOC_ERR);

	ssize_t bytes_read = read(file_fd, buf, file_size);
	if (bytes_read < 0)
		ERR(READ_ERR);
	close(file_fd);
	return buf;
}

void print_map(Maps *maps)
{
	for (uint i = 0; i < maps->len; ++i)
		printf("%c, %s\n", maps->maps[i].c, maps->maps[i].encoded_str);
}

