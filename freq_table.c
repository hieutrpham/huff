#include "main.h"
#include <stdint.h>
#include <sys/types.h>

c_freq make_freq_test(uint8_t c, uint32_t freq) {
	return (c_freq){.freq = freq, .c = c};
}

int compar(const void *one, const void *two) {
	const c_freq *c1 = (const c_freq*)one;
	const c_freq *c2 = (const c_freq*)two;
	return c1->freq <= c2->freq;
}

// populate and sort in descending order
void populate_table(uint8_t *buf, hist_arr *freq_table) {
	for (uint i = 0; buf[i]; ++i)
	{
		if (exist(buf[i], freq_table)) {
			for (uint j = 0; j < freq_table->count; ++j)
			{
				if (freq_table->items[j].c == buf[i])
					freq_table->items[j].freq++;
			}
		}
		else
			freq_table->items[freq_table->count++] = make_freq_test(buf[i], 1);
	}
	freq_table->items[freq_table->count++] = make_freq_test('\r', 1);
	qsort(freq_table->items, freq_table->count, sizeof(freq_table->items[0]), compar);
}

void print_table(hist_arr *table) {
	for (uint i = 0; i < table->count; ++i) {
		if (table->items[i].c != '\n') // skip new line since it will mess up the stdout
			printf("char: %c, freq: %u\n", table->items[i].c, table->items[i].freq);
		else
			printf("new line freq: %u\n", table->items[i].freq);
	}
}

boolean exist(uint8_t c, hist_arr *hist_a) {
	for (uint i = 0; i < hist_a->count; ++i)
	{
		if (hist_a->items[i].c == c)
			return true;
	}
	return false;
}
