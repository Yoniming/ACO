#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FUNCTION_ { UNDEFINED, HELP = 1, CODE, DECODE } FUNCTION;

typedef struct args {
	FUNCTION fun;
	char* input_file;
	char* output_file;
	char* fasta_file;
	void* others;	 // 预留扩充字段
} Args;

void init_args(Args* args, char* config_file);

void reset_args(Args* args);

void set_args_char(char** key, char* value);

// void set_args_int(int key, char* value);
#define set_args_int(key, value) (key = atoi(value))

// void set_args_float(float key, char* value);
#define set_args_float(key, value) (key = atof(value))

void showUsage();

void showVersion(int major, int minor, int patch);

#ifdef __cplusplus
}
#endif

#endif // !ARGUMENTS_H
