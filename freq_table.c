#include "main.h"

int compar(const void *one, const void *two) {
	const c_freq *c1 = one;
	const c_freq *c2 = two;
	return c1->freq <= c2->freq;
}

// populate and sort in descending order
void populate_table(char *buf, hist_arr *freq_table) {
	for (int i = 0; buf[i]; ++i) {
		if (exist(buf[i], *freq_table)) {
			for (int j = 0; j < freq_table->count; ++j) {
				if (freq_table->items[j].c == buf[i])
					freq_table->items[j].freq++;
			}
		}
		else
			freq_table->items[freq_table->count++] = (c_freq){.c = buf[i], .freq = 1};
	}
	qsort(freq_table->items, freq_table->count, sizeof(freq_table->items[0]), compar);
}

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
