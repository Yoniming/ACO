//
// Created by baoyixing on 3/26/21.
//

#include "CoderNoMean.h"

#include <exception>

#include "mytime.h"
#include "show.h"

#define BLOCK 160

typedef struct strings_ {
	char* val;
	size_t sz;
} strings;

CoderNoMean::CoderNoMean(size_t buffer_sz, size_t used_memory, uint32_t max_len): max_len(max_len), buffer_size(buffer_sz), used_m(used_memory) {
}

CoderNoMean::CoderNoMean(size_t buffer_sz, size_t used_memory, uint32_t max_len, uint8_t min, uint8_t max): max_len(max_len), buffer_size(buffer_sz), used_m(used_memory), min_value(min), max_value(max) {
}

CoderNoMean::~CoderNoMean() = default;

int CoderNoMean::compress(const char* input_file, const char* output_file) {
	int ret_status = 0;
	std::chrono::milliseconds interval = std::chrono::milliseconds(25);
	size_t origin_size = 0, compress_size = 0;
	Time running_time = Time();

	try {
		running_time.start();
		int model_num = NUM_SYMBOL * NUM_SYMBOL * NUM_SYMBOL * 16;
		list* quality_block_list = make_list();
		list* base_block_list = make_list();
		create_table(table);
		init_table();
		char* out0 = new char[buffer_size];

		auto in_file = File(input_file, "rb");
		auto out_file = File(output_file, "wb");

		// save min and max value
		out_file.write(&min_value, sizeof(char), 1);
		out_file.write(&max_value, sizeof(char), 1);
		compress_size += 2;

		in_file.set_block_size(BLOCK);
		int T = XX_SYMBOL - 1;

		SIMPLE_MODEL<NUM_SYMBOL + 1>* model_qual;
		model_qual = new SIMPLE_MODEL<NUM_SYMBOL + 1>[model_num + 1];

		while (!in_file.feof()) {
			char* line;
			uint32_t row_count = 0;
			while (!in_file.feof() && row_count < max_len && row_count * in_file.get_block_size() * 1.000001 < used_m) {
				for (int i = 0; i < 4; ++i) {
					line = in_file.getline();

					if (i == 0 || i == 2)
						free(line);
					else if (i == 1 && line)
						list_insert(base_block_list, (void*)line);
					else if (i == 3 && line)
						list_insert(quality_block_list, (void*)line);
				}
				++row_count;
			}

			row_count = quality_block_list->size;
			char** quality_block = (char**)list_to_array(quality_block_list);
			char** base_block = (char**)list_to_array(base_block_list);

			auto col_len = (uint32_t*)malloc(row_count * sizeof(uint32_t));
			uint32_t col_max = 0, tmp;
			for (uint32_t i = 0; i < row_count; ++i) {
				tmp = strlen(quality_block[i]);
				origin_size += tmp;
				col_len[i] = tmp;
				if (col_max < tmp) col_max = tmp;
			}

			/* destory list*/
			free_list(quality_block_list);
			free_list(base_block_list);

			// progress bar
			auto progressBar = ProgressBar(col_max * row_count, interval);
			progressBar.start();

			RangeCoder rc;
			rc.output(out0 + 8);
			rc.StartEncode();
			uint32_t i = 0, model_idx;
			for (uint32_t j = 0; j < col_max; ++j) {
				for (uint32_t k = 0; k < row_count; ++k) {
					i = j % 2 ? (row_count - k - 1) : k;

					/* check boundary of array */
					if (j >= col_len[i]) continue;

					quality_block[i][j] = quality_block[i][j] - min_value;
					char cur_base = base_block[i][j];
					int J0 = table(cur_base);
					if (J0 == 4) {
						model_idx = 0;
					}
					else {
						if (j == 0) {
							model_idx = J0 + 1;
						}
						else if (j == 1) {
							char pre_base_1 = base_block[i][j - 1];
							int J1 = table(pre_base_1);
							int G1 = J0 * 5 + J1;
							int Q1 = quality_block[i][j - 1];
							model_idx = Q1 * 20 + G1 + 5;
						}
						else if (j == 2) {
							char pre_base_1 = base_block[i][j - 1];
							int J1 = table(pre_base_1);
							int G1 = J0 * 5 + J1;
							int Q1 = quality_block[i][j - 1];
							int Q2 = quality_block[i][j - 2];
							model_idx = (Q1 * NUM_SYMBOL + Q2) * 20 + G1 + 5 + NUM_SYMBOL * 20;
						}
						else {
							char pre_base_1 = base_block[i][j - 1];
							// char pre_base_2 = base_block[i][j - 2];
							int J1 = table(pre_base_1);
							// int J2 = table[pre_base_2];
							int G1 = J0 * 5 + J1;
							// int G2 = J0 * 4 * 4 + J1 * 4 + J2;
							int Q1 = quality_block[i][j - 1];
							int Q2 = quality_block[i][j - 2];
							int Q3 = quality_block[i][j - 3];
							int Q4 = 0;
							if (j == 3) {
								Q4 = Q3;
							}
							else {
								Q4 = quality_block[i][j - 4];
							}
							int A = Q1;
							int B = std::max(Q2, Q3);
							int C = 0;
							if (Q2 == Q3) {
								C = 1;
							}
							if (Q3 == Q4) {
								C = 2;
							}
							//							 int M = j / 13;
							//							 int a = col_len[i] / 13 + 1;
							//							 model_idx = A * NUM_SYMBOL * 3 * a * 20 + B * 3 * a * 20 +
							//							            C * a * 20 + M * 20 + G1 + 5 + NUM_SYMBOL * 20 +
							//							            NUM_SYMBOL * NUM_SYMBOL * 20;
							model_idx = A * NUM_SYMBOL * 3 * 20 + B * 3 * 20 + C * 20 + G1 + 5
										+ NUM_SYMBOL * 20 + NUM_SYMBOL * NUM_SYMBOL * 20;
						}
					}

					// model_qual[model_idx].encodeSymbol(&rc, quality_block[offset]);
					if (quality_block[i][j] > T) {
						// ac_encode_symbol(&ace_ny, &acm_ny[model_idx], quality_block[offset]
						// - T);
						model_qual[model_idx].encodeSymbol(&rc, quality_block[i][j] - T);
					}
					else {
						// ac_encode_symbol(&ace_ny, &acm_ny[model_idx], 0);
						// ac_encode_symbol(&ace_ny, &acm_ny[model_num],
						// quality_block[offset]);
						model_qual[model_idx].encodeSymbol(&rc, 0);
						model_qual[model_num].encodeSymbol(&rc, quality_block[i][j]);
					}
					progressBar.update();
				}
			}
			progressBar.finish();

			rc.FinishEncode();
			uint32_t sz1;
			sz1 = rc.size_out();
			/* block size */
			out0[0] = (sz1 >> 0) & 0xff;
			out0[1] = (sz1 >> 8) & 0xff;
			out0[2] = (sz1 >> 16) & 0xff;
			out0[3] = (sz1 >> 24) & 0xff;
			/* save lines number*/
			out0[4] = (row_count >> 0) & 0xff;
			out0[5] = (row_count >> 8) & 0xff;
			out0[6] = (row_count >> 16) & 0xff;
			out0[7] = (row_count >> 24) & 0xff;

			compress_size += out_file.write(out0, sizeof(char), sz1 + 8);
			free_array((void**)quality_block, row_count);
			free_array((void**)base_block, row_count);
			free(col_len);

			quality_block_list = make_list();
			base_block_list = make_list();
		}


		delete[] out0;
		free_list_contents(quality_block_list);
		free_list(quality_block_list);
		free_list_contents(base_block_list);
		free_list(base_block_list);
		running_time.end();
		show(origin_size, compress_size, running_time.get_time_used());
	}
	catch (std::bad_alloc& e) {
		e.what();
		ret_status = -1;
	}
	catch (std::runtime_error& e) {
		e.what();
		ret_status = 1;
	}
	catch (std::exception& e) {
		e.what();
		ret_status = 2;
	}

	return ret_status;
}

int CoderNoMean::decompress(const char* compress_file, const char* fasta_file, const char* output_file) {
	char** decoded_block = nullptr;
	std::chrono::milliseconds interval = std::chrono::milliseconds(25);
	size_t origin_size = 0, decompress_size = 0;
	Time running_time = Time();

	running_time.start();
	int model_num = NUM_SYMBOL * NUM_SYMBOL * NUM_SYMBOL * 16;
	create_table(table);
	init_table();

	int ret_status = 0;

	try {
		char* in_buf1 = new char[buffer_size];
		list* base_block_list = nullptr;
		list* first_line_list = nullptr;
		list* third_line_list = nullptr;

		auto in_file = File(compress_file, "rb");
		auto fa_file = File(fasta_file, "rb");
		auto out_file = File(output_file, "wb");
		// restore min and max value
		in_file.read(&min_value, sizeof(char), 1);
		in_file.read(&max_value, sizeof(char), 1);
		origin_size += 2;

		int T = XX_SYMBOL - 1;
		SIMPLE_MODEL<NUM_SYMBOL + 1>* model_qual;
		model_qual = new SIMPLE_MODEL<NUM_SYMBOL + 1>[model_num + 1];

		uint32_t len_buf[2];
		while (!in_file.feof()) {
			/* init list */
			base_block_list = make_list();
			first_line_list = make_list();
			third_line_list = make_list();

			/* read bytes from compress file */
			in_file.read(len_buf, sizeof(char), 4);							   // block size (bytes)
			in_file.read(&len_buf[1], sizeof(char), 4);						   // block rows
			origin_size += in_file.read(in_buf1, sizeof(char), len_buf[0]);	   // block content

			char* line;
			uint32_t row_count = 0, tmp;
			while (!fa_file.feof() && row_count < len_buf[1]) {
				for (int i = 0; i < 4; ++i) {
					line = fa_file.getline();
					if (!line) continue;

					if (i == 0) {
						/* 节省内存空间 */
						tmp = strlen(line);
						if (tmp * 3 < in_file.get_block_size()) {
							line = (char*)realloc(line, tmp * sizeof(char));
							if (!line) throw std::bad_alloc();
						}
						auto* line_sz = (strings*)malloc(sizeof(strings));
						if (!line_sz) throw std::bad_alloc();
						line_sz->val = line;
						line_sz->sz = tmp;
						list_insert(first_line_list, (void*)line_sz);
					}
					else if (i == 1)
						list_insert(base_block_list, (void*)line);
					else if (i == 2) {
						/* 节省内存空间 */
						tmp = strlen(line);
						if (tmp * 3 < in_file.get_block_size()) {
							line = (char*)realloc(line, tmp * sizeof(char));
							if (!line) throw std::bad_alloc();
						}
						auto* line_sz = (strings*)malloc(sizeof(strings));
						if (!line_sz) throw std::bad_alloc();
						line_sz->val = line;
						line_sz->sz = tmp;
						list_insert(third_line_list, (void*)line_sz);
					}
					else if (i == 3)
						free(line); /* 不保存第四行信息 */
				}
				++row_count;
			}

			row_count = base_block_list->size;	  // save the number of quality_block;
			char** base_block = (char**)list_to_array(base_block_list);
			free_list(base_block_list);

			auto* col_len = (uint32_t*)malloc(row_count * sizeof(uint32_t));
			uint32_t col_max = 0;
			for (uint32_t i = 0; i < row_count; ++i) {
				tmp = strlen(base_block[i]);
				col_len[i] = tmp;
				if (col_max < tmp) col_max = tmp;
			}

			decoded_block = (char**)calloc(row_count, sizeof(char*));
			for (uint32_t i = 0; i < row_count; ++i)
				decoded_block[i] = (char*)malloc(col_len[i] * sizeof(char));

			ProgressBar progressBar = ProgressBar(col_max * row_count, interval);
			progressBar.start();

			RangeCoder rc;
			rc.input(in_buf1);
			rc.StartDecode();
			uint32_t i, model_idx;
			for (uint32_t j = 0; j < col_max; ++j) {
				for (uint32_t k = 0; k < row_count; ++k) {
					i = j % 2 ? (row_count - k - 1) : k;

					/* check boundary of array */
					if (j >= col_len[i]) continue;

					char cur_base = base_block[i][j];
					int J0 = table(cur_base);
					if (J0 == 4) {
						model_idx = 0;
					}
					else {
						if (j == 0) {
							model_idx = J0 + 1;
						}
						else if (j == 1) {
							char pre_base_1 = base_block[i][j - 1];
							int J1 = table(pre_base_1);
							int G1 = J0 * 5 + J1;
							int Q1 = decoded_block[i][j - 1];
							model_idx = Q1 * 20 + G1 + 5;
						}
						else if (j == 2) {
							char pre_base_1 = base_block[i][j - 1];
							int J1 = table(pre_base_1);
							int G1 = J0 * 5 + J1;
							int Q1 = decoded_block[i][j - 1];
							int Q2 = decoded_block[i][j - 2];
							model_idx = (Q1 * NUM_SYMBOL + Q2) * 20 + G1 + 5 + NUM_SYMBOL * 20;
						}
						else {
							char pre_base_1 = base_block[i][j - 1];
							int J1 = table(pre_base_1);
							int G1 = J0 * 5 + J1;
							int Q1 = decoded_block[i][j - 1];
							int Q2 = decoded_block[i][j - 2];
							int Q3 = decoded_block[i][j - 3];
							int Q4 = 0;
							if (j == 3) {
								Q4 = Q3;
							}
							else {
								Q4 = decoded_block[i][j - 4];
							}
							int A = Q1;
							int B = std::max(Q2, Q3);
							int C = 0;
							if (Q2 == Q3) {
								C = 1;
							}
							if (Q3 == Q4) {
								C = 2;
							}
							//							int M = j / 13;
							//							int a = col_len[i] / 13 + 1;
							//
							//							model_idx = A * NUM_SYMBOL * 3 * a * 15 + B * 3 * a * 20 + C * a * 20 + M * 20 + G1 + 5 + NUM_SYMBOL * 20 + NUM_SYMBOL * NUM_SYMBOL * 20;
							model_idx = A * NUM_SYMBOL * 3 * 20 + B * 3 * 20 + C * 20 + G1 + 5
										+ NUM_SYMBOL * 20 + NUM_SYMBOL * NUM_SYMBOL * 20;
						}
					}
					decoded_block[i][j] = model_qual[model_idx].decodeSymbol(&rc);
					// decoded_matrix[offset] = decoded_matrix[offset] + '!';
					if (int(decoded_block[i][j]) == 0) {
						decoded_block[i][j] = model_qual[model_num].decodeSymbol(&rc);
					}
					else {
						decoded_block[i][j] = decoded_block[i][j] + T;
					}
					progressBar.update();
				}
			}

			progressBar.finish();

			node *f_n = first_line_list->front, *t_n = third_line_list->front;
			strings* ss;
			for (i = 0; i < row_count; ++i) {
				for (uint32_t j = 0; j < col_len[i]; ++j)
					decoded_block[i][j] = decoded_block[i][j] + min_value;
				/* 1st line */
				ss = (strings*)f_n->val;
				out_file.write(ss->val, sizeof(char), ss->sz);
				fprintf(out_file.get_fp(), "\n");
				free(ss->val);
				f_n = forward(f_n);

				/* 2nd line */
				out_file.write(base_block[i], sizeof(char), col_len[i]);
				fprintf(out_file.get_fp(), "\n");

				/* 3rd line */
				ss = (strings*)t_n->val;
				out_file.write(ss->val, sizeof(char), ss->sz);
				fprintf(out_file.get_fp(), "\n");
				free(ss->val);
				t_n = forward(t_n);

				/* 4th line */
				decompress_size += out_file.write(decoded_block[i], sizeof(char), col_len[i]);
				fprintf(out_file.get_fp(), "\n");
			}

			free(col_len);
			free_list_contents(first_line_list);
			free_list(first_line_list);
			free_list_contents(third_line_list);
			free_list(third_line_list);
			free_array((void**)decoded_block, row_count);
			free_array((void**)base_block, row_count);

			rc.FinishDecode();
		}

		std::cout << "finished" << std::endl;

		delete[] model_qual;
		running_time.end();
		show(origin_size, decompress_size, running_time.get_time_used());
	}
	catch (std::bad_alloc& e) {
		e.what();
		ret_status = -1;
	}
	catch (std::runtime_error& e) {
		e.what();
		ret_status = 1;
	}
	catch (std::exception& e) {
		e.what();
		ret_status = 2;
	}

	return ret_status;
}


void CoderNoMean::get_max_and_min_value(const char* input_file) {
	try {
		File in_file = File(input_file, "rb");
		int max = 0;
		int min = 255;
		uint col_len = 0;
		while (!in_file.feof()) {
			char* line;
			for (int i = 0; i < 4; ++i) {
				line = in_file.getline();

				if (i == 0 || i == 1 || i == 2)
					free(line);
				else if (i == 3 && line) {
					col_len = strlen(line);
					for (uint32_t j = 0; j < col_len; ++j) {
						if (line[j] < min) min = line[j];
						if (line[j] > max) max = line[j];
					}
					free(line);
				}
			}
		}

		max_value = max;
		min_value = min;
	}
	catch (std::bad_alloc& e) {
		e.what();
	}
	catch (std::runtime_error& e) {
		e.what();
	}
	catch (std::exception& e) {
		e.what();
	}
}

void CoderNoMean::set_min_value(uint8_t min) {
	min_value = min;
}
void CoderNoMean::set_max_value(uint8_t max) {
	max_value = max;
}
uint8_t CoderNoMean::get_min_value() const {
	return min_value;
}
uint8_t CoderNoMean::get_max_value() const {
	return max_value;
}
