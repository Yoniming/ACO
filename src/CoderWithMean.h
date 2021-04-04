//
// Created by baoyixing on 3/26/21.
//

#ifndef CODE_CODERWITHMEAN_H
#define CODE_CODERWITHMEAN_H
#include <cstdio>
#include <cstring>
#include <iostream>

#include "clr.h"
#include "coder.h"
#include "file.h"
#include "list.h"
#include "simple_model.h"
#include "table.h"

class CoderWithMean: virtual public Coder {
public:
	CoderWithMean(size_t buffer_sz, size_t used_memory, uint32_t max_len, uint8_t min, uint8_t max);
	CoderWithMean(size_t buffer_sz, size_t used_memory, uint32_t max_len);
	~CoderWithMean();

	void get_max_and_min_value(const char* input_file) override;
	void set_min_value(uint8_t min);
	void set_max_value(uint8_t max);
	uint8_t get_min_value() const;
	uint8_t get_max_value() const;
	int compress(const char* input_file, const char* output_file) override;
	int decompress(const char* compress_file, const char* fasta_file, const char* output_file) override;

protected:
	uint32_t max_len {1000000};
	size_t buffer_size {200000000};
	size_t used_m {500000000};
	uint8_t min_value {0};
	uint8_t max_value {0};
};


#endif	  //CODE_CODERWITHMEAN_H
