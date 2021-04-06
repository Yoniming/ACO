#include "arguments.h"
#include <stdlib.h>


void init_args(Args *args, char *config_file) {
  if (config_file == NULL) {
    args->fun = UNDEFINED;
    args->input_file = NULL;
    args->output_file = NULL;
    args->fasta_file = NULL;
    args->others = NULL;
  } else {
    /*
    从config_file文件中读入相应的配置选项完成默认的初始化
    */
  }
}

void set_args_char(char **key, char* value)
{
    int l = strlen(value);
    if(key == NULL)
    {
        *key = (char*)malloc(( l + 1) * sizeof(char));
        strncpy(*key, value, (l+1) * sizeof(char));
    }else
    {
        *key = (char*)realloc(*key, (l+1) * sizeof(char));
        strncpy(*key, value, (l+1) * sizeof(char));
    }
}
