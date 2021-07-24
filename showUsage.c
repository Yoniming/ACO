#include <stdio.h>

#include "arguments.h"

void showUsage() {
	// printf("=============================================================\n");
	printf("\nNAME\n");
	printf("\t\tcode - code and decode\n");
	printf("SYNOPSIS\n");
	printf(
		"\t\tUsage: code [OPTION]... INPUT FILE... [FASTA_FILE] OUTPUT FILE... \n\n");
	printf("DESCRIPTION\n");
	printf("\t\tcode and decode ...\n\n");
	printf("\t\t-c, --code\n");
	printf("\t\t\t\tcode input file into output file.\n\n");
	printf("\t\t-d, --decode\n");
	printf("\t\t\t\tcode input file into output file.\n\n");
	printf("\t\t-t, --time\n");
	printf("\t\t\t\trecord time of code or decode process.\n\n");
	printf("\t\t-h, --help\n");
	printf("\t\t\t\tdisplay this help and exit\n\n");
}