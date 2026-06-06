#include "main.h"
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
		return 1;
	}

	size_t file_size = get_file_size(file_name);

	// BUG: when reading binary file?
	char *buf = read_entire_file(file_size, file_name);

	hist_arr freq_table = {0};
	if (!is_decompress)
	{
		populate_table(buf, &freq_table);
		Maps maps = {0};
		TreeNode *tree = get_tree(&freq_table);
		fill_maps(tree, &maps);

		StaticString encoded_file = {0};
		fill_encoded_file(file_size, &maps, &encoded_file, buf);

		FILE *outfile = build_outfile(file_name, "_compressed");
		compress(&freq_table, &encoded_file, outfile);
		fclose(outfile);
	}
	else
	{
		uint curr_ptr = parse_header(buf, &freq_table);
		TreeNode *tree = get_tree(&freq_table);
		FILE *outfile = build_outfile(file_name, "_decompressed");
		decompress(tree, buf, curr_ptr, outfile);
		fclose(outfile);
	}

	free(buf);
	buf = NULL;
}

void decompress(TreeNode *tree, const char *buf, uint curr_ptr, FILE *outfile)
{
	TreeNode *tmp = tree;
	while (buf[curr_ptr])
	{
		unsigned char c = buf[curr_ptr];
		for (int i = 0; i < 8; ++i)
		{
			if (c & (1 << (7 - i)))
				tmp = tmp->r;
			else
				tmp = tmp->l;

			if (tmp->c)
			{
				fwrite(&tmp->c, 1, 1, outfile);
				tmp = tree;
			}
		}
		curr_ptr+=1;
	}
}

TreeNode *get_tree(hist_arr *freq_table)
{
	Queue *initial_q = build_queue_from_table(freq_table);
	Queue *combine_q = init_queue();
	TreeNode *tree = build_huffman_tree(initial_q, combine_q);
	if (!tree)
		ERR(MALLOC_ERR);
	return tree;
}

// return pointer to the begin of content body
uint parse_header(const char *buf, hist_arr *freq_table)
{
	uint i = 0;

	while (buf[i] != '\r')
	{
		c_freq *curr = &freq_table->items[freq_table->count];
		curr->c = buf[i];
		curr->freq = (uint)buf[i+1];
		freq_table->count++;
		i += 5;
	}
	return i + 1;
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
	for (int i = 0; i < freq_table->count; ++i)
	{
		c_freq curr_item = freq_table->items[i];
		fwrite(&curr_item.c, 1, 1, outfile);
		fwrite(&curr_item.freq, sizeof(curr_item.freq), 1, outfile);
	}

	char sep = '\r';
	fwrite(&sep, 1, 1, outfile);

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
	if (file_stat < 0)
		ERR(STAT_ERR);
	return fs.st_size;
}

char *read_entire_file(size_t file_size, const char *file_name)
{
	int file_fd = open(file_name, O_RDONLY);
	if (file_fd < 0)
		ERR(OPEN_ERR);

	char *buf = calloc(file_size + 1, sizeof(*buf));
	if (!buf)
		ERR(MALLOC_ERR);

	ssize_t bytes_read = read(file_fd, buf, file_size);
	if (bytes_read < 0)
		ERR(READ_ERR);
	close(file_fd);
	return buf;
}
