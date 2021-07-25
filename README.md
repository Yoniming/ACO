# ACO
ACO is a lossless quality scores compression algorithm based on adaptive coding order and this software can be used for both encoding and decoding.
# Installing
ACO can be used on Windows,Linux or Mac. We only provide a source distribution and allow users to test or improve. In particular, ACO has no external dependencies so that it is easy to implement its function.

The distribution is configured out of the box for linux. To build on a Mac, change the setting parameters of the command line to work properly

There is currently no makefile for Windows. A visual studio project can be made (and testing has been done on windows to verify compatibility) but the steps are beyond the scope of this guide, because you must take several additional steps to guarantee a sane environment on windows (such as replacing stdint and inttypes with correct versions). For this reason we also currently do not distribute a windows build script.
# Algorithm
The main objective of ACO is to traverse the quality scores along the most relative directions, which can be regarded as a reorganization of the stack of independent 1D quality scores vectors into highly related 2D matrices. Another improvement of the proposed ACO technique over the existing techniques is the compound context modeling strategy. Instead of the adjacent QS values, the ACO context models consists of two additional aspects: 1) the global average of every reads; 2) the variant of DNA bases. The compound context model not only benefits the probability estimation and arithmetic coding, more importantly, in the implementation, it prevents ACO from multiple random access of the input FASTQ file:  the compressing process can be accomplished in only one-path, at the cost of some context dilution and side-information.
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

For example，compress `ERR174324_1.fastq` to `ERR174324_1_c.out`:
```
./code -m 0 -c path/ERR174324_1.fastq path/ERR174324_1_c.out
```
On the contrary, decompress file:
```
./code -m 0 -d path/ERR174324_1_c.out path/ERR174324_1.fastq path/ERR174324_1_r.fastq
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
                Usage: code [OPTION]... INPUT FILE... [FASTA_FILE] OUTPUT FILE... 

DESCRIPTION
                code and decode ...

                -c, --code
                                code input file into output file.

                -d, --decode
                                decode input file into output file.

                -t, --time
                                record time of coding/decoding process.

                -h, --help
                                display this help and exit

                -v, --version
                                output version information and exit.


```
Note:
> + *Options `-c` and `-d`* ： are mutually ***exclusive*** and cannot be used at the same time.
> + *Option `-t`* ： The time measured with the option `-t` means the total time which the program has been executed ***except for command line parsing***.
> + *Option `-m`* : The same mode should be maintained when compressing and decompressing the same files.
> + when decompressing file, the content of the second position argument fastq file is identical to output file. But, we only use the base line (2th line) to restore quality line(4th line) in our algorithm.

# Declaration

In our C++ project,

1. ``ProgressBar.h/ProgressBar.cc``  For the implement of the progress bar, we refer to the following link: [用C++实现一个命令行进度条](https://github.com/HaoKunT/blog-hugo/blob/b6d3a55edc7ab858350445c44ea44cad369468c0/content/post/%E7%94%A8C%2B%2B%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E5%91%BD%E4%BB%A4%E8%A1%8C%E8%BF%9B%E5%BA%A6%E6%9D%A1.md) and modify a little details in order to better adopt to Linux system and run correctly as we expected.

2.  File ```list.h/list.c ``` are copied and modified from [darknet](https://github.com/pjreddie/darknet/blob/master/src/list.c).


# Bugs and Feedback
Please use GitHub issues to open bug reports or provide feedback about ACO.
