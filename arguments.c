#include "arguments.h"

#include <stdlib.h>
#include <stdio.h>


void init_args(Args* args, char* config_file) {
	if (config_file == NULL) {
		args->fun = UNDEFINED;
		args->input_file = NULL;
		args->output_file = NULL;
		args->fasta_file = NULL;
		args->others = NULL;
	}
	else {
		/*
    从config_file文件中读入相应的配置选项完成默认的初始化
    */
	}
}

void reset_args(Args* args) {
	args->fun = UNDEFINED;
	free(args->input_file);
	args->input_file = NULL;
	free(args->fasta_file);
	args->fasta_file = NULL;
	free(args->output_file);
	args->output_file = NULL;
	free(args->others);
	args->others = NULL;
}

void set_args_char(char** key, char* value) {
	int l = strlen(value);
	if (key == NULL) {
		*key = (char*)malloc((l + 1) * sizeof(char));
		strncpy(*key, value, (l + 1) * sizeof(char));
	}
	else {
		*key = (char*)realloc(*key, (l + 1) * sizeof(char));
		strncpy(*key, value, (l + 1) * sizeof(char));
	}
}

void showVersion(int major, int minor, int patch) {
	char *repo_url = "https://github.com/Yoniming/code";
	printf("ACO code version %d.%d.%d\n\n", major, minor, patch);
	printf("ACO code software maintained and supported by %s.\n", repo_url);
}
