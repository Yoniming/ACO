# code
code/decode with RangeCoder
## Compile project
This program is a command line program based on the Linux operating system. In fact, it only calls functions related to the Linux system when the command line is parsed. If you want to be cross-platform, you only need a few modifications to run on the window 10 platform.
This project is a CMake project. In order to run the program, there are the following steps:

1. Download the source code from the github repository to your local computer
    ```
    git clone https://github.com/Yoniming/code.git
    ```   
2. Enter the `code/` directory, create a new directory `build/`, enter the directory
   ```
   cd code && mkdir build && cd build
   ```
3. Compile this project
   ```
   cmake .. && make
   ```
If you complete the above steps correctly, you will find a executable file named `code` in `build` directory, and then you can run the program.

---
## How to use
Run this program:
```
./code [options] [abs_path_to_src ...] [abs_path_to_dst] 
```
For this program, the following options are necessary:
1. `-c/-d` : must choose at most one of them.
2. `-m` : When `-m` is `0`, it means to use the model without mean value, when it is `1`, it means to use the model with mean value

For example，compress `a.fastq` to `b.out`:
```
./code -m 0 -c path/a.fastq path/b.out
```
On the contrary, decompress file:
```
./code -m 0 -d path/b.out path/a.fastq path/c.fastq
```
You can see more usage by optional argument `-h`:
```
./code -h
```
then, usage is shown:
```

NAME
		 code - code and decode
SYNOPSIS
		Usage: code [OPTION]... INPUT FILE... [FASTQ_FILE] OUTPUT FILE... 

DESCRIPTION
		code and decode ...

		-c, --code
				code input file into output file.

		-d, --decode
				code input file into output file.

		-m, --mode
				the type of model selected when compressing or decompressing

		-t, --time
				record time of code or decode process.

		-h, --help
				display this help and exit


```
Note:
> + *Options `-c` and `-d`* ： are mutually ***exclusive*** and cannot be used at the same time.
> + *Option `-t`* ： The time measured with the option `-t` means the total time which the program has been executed ***except for command line parsing***.
> + *Option `-m`* : The same mode should be maintained when compressing and decompressing the same files.