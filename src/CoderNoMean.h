#ifndef CODE_CODERNOMEAN_H
#define CODE_CODERNOMEAN_H
#include <omp.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "ProgressBar.h"
#include "clr.h"
#include "coder.h"
#include "file.h"
#include "list.h"
#include "simple_model.h"
#include "table.h"


#define MAX_SYMBOL 42
#define NUM_SYMBOL 38
#define XX_SYMBOL (MAX_SYMBOL - NUM_SYMBOL)


class CoderNoMean: virtual public Coder {
public:
	CoderNoMean(size_t buffer_sz, size_t used_memory, uint32_t max_len);
	CoderNoMean(size_t buffer_sz, size_t used_memory, uint32_t max_len, uint8_t min, uint8_t max);
	~CoderNoMean();
	void get_max_and_min_value(const char* input_file) override;
	void set_min_value(uint8_t min);
	void set_max_value(uint8_t max);
	uint8_t get_min_value() const;
	uint8_t get_max_value() const;
	int compress(const char* input_file, const char* output_file) override;
	int decompress(const char* compress_file, const char* fasta_file, const char* output_file) override;

protected:
	uint32_t max_len;
	size_t buffer_size;
	size_t used_m;
	uint8_t min_value;
	uint8_t max_value;
};


#endif	  //CODE_CODERNOMEAN_H
