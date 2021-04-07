#!/bin/bash
pragram=./bulid/code
# compress
${pragram} -t -m 0 -c fastq_file compress_file

# decompress
#${pragram} -t -m 0 -d compress_file fastq_file output_file