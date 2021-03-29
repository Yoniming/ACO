//
// Created by baoyixing on 3/26/21.
//

#ifndef CODE_CODER_H
#define CODE_CODER_H

class Coder {
public:
	virtual void get_max_and_min_value(const char* input_file) = 0;
	virtual int compress(const char* input_file, const char* output_file) = 0;
	virtual int decompress(const char* compress_file, const char* fasta_file, const char* output_file) = 0;
};

#endif	  //CODE_CODER_H
