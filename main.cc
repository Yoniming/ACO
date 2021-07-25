#include <getopt.h>

#include <iostream>
#include <iomanip>

#include "arguments.h"
#include "src/mytime.h"
#include "src/coder.h"
#include "src/CoderNoMean.h"
#include "src/CoderWithMean.h"

#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define REVISION_VERSION 2

#define RET_INIT -2

int main(int argc, char* argv[]) {
	int opt;
	int option_index = 0;
	const char* short_string = "cdhtvm:";

	static struct option long_options[] = {
		{"code", no_argument, NULL, 'c'},
		{"decode", no_argument, NULL, 'd'},
		{"help", no_argument, NULL, 'h'},
		{"time", no_argument, NULL, 't'},
		{"version", no_argument, NULL, 'v'},
		{"mode", required_argument, NULL, 'm'},	// change mode
		{NULL, 0, NULL, 0},
	};

	Args args;
	init_args(&args, NULL);

	bool test_time = false;
	bool version = false;
    int model = 0;

	while ((opt = getopt_long_only(argc, argv, short_string, long_options,
								   &option_index))
		   != -1) {
		switch (opt) {
			case 'c':
				args.fun = FUNCTION::CODE;
				break;
			case 'd':
				args.fun = FUNCTION::DECODE;
				break;
			case 't':
				test_time = true;
				break;
			case 'm':
                set_args_int(model, optarg);
				break;
			case 'v':
				version = true;
			case 'h':
			default:
				args.fun = FUNCTION::HELP;
				break;
		}
	}

	if (version)
	{
		showVersion(MAJOR_VERSION, MINOR_VERSION, REVISION_VERSION);
		return 0;
	}

	if (args.fun == FUNCTION::CODE) {
		if (optind + 1 < argc) {
			set_args_char(&args.input_file, argv[optind]);
			set_args_char(&args.output_file, argv[optind + 1]);
		}
		else
			std::cout << "Error: nessary argumets is missing!!!" << std::endl;
	}
	else if (args.fun == FUNCTION::DECODE) {
		if (optind + 2 < argc) {
			set_args_char(&args.input_file, argv[optind]);
			set_args_char(&args.fasta_file, argv[optind + 1]);
			set_args_char(&args.output_file, argv[optind + 2]);
		}
		else
			std::cout << "Error: nessary argumets is missing!!!" << std::endl;
	}

	Coder *coder;
	if (model == 0)
		coder = new CoderNoMean(1000000000, 1700000000, 10000000);
	else if (model == 1)
		coder = new CoderWithMean(1000000000, 1700000000, 10000000);
	else
		std::cout << "m == " << model << ": out of set = { 0, 1 } " << std::endl;

	std::cout << std::setw(120) << std::setfill('=') << "\n";

	int ret = RET_INIT;
	Time time = Time();
	switch (args.fun) {
		case FUNCTION::CODE:
			std::cout << "Compute max and min value of " << args.input_file << std::endl;
			if (test_time) time.start();
			coder->get_max_and_min_value(args.input_file);
			std::cout << "Now start to compress:" << std::endl;
			ret = coder->compress(args.input_file, args.output_file);
			if (test_time) time.end();
			if (test_time) std::cout << "Time of compress process:\t" << time.get_time_used() << " s." << std::endl;
			break;

		case FUNCTION::DECODE:
			if (test_time) time.start();
			std::cout << "Now start to decompress:" << std::endl;
			ret = coder->decompress(args.input_file, args.fasta_file, args.output_file);
			if (test_time) time.end();
			if (test_time) std::cout << "Time of decompress process:\t" << time.get_time_used() << " s." << std::endl;
			break;

		default:
			showUsage();
			break;
	}

	if (ret == 0)
		std::cout << "run successfully.........." << std::endl;

	reset_args(&args);

	return 0;
}
