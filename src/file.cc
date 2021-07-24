//
// Created by baoyixing on 3/25/21.
//

#include "file.h"

#include <climits>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>

File::File(const char* c_str, const char* mode = "rb"): fp(fopen(c_str, mode)) {
	if (fp == nullptr) {
		std::string s = std::string(c_str) + " : doesn't exist or open file.";
		throw std::runtime_error(s);
	}
}


File::File(const std::string& str, const char* mode = "rb"): fp(fopen(str.c_str(), mode)) {
	if (fp == nullptr)
		throw std::runtime_error(str + " : doesn't exist or open file.");
}


size_t File::write(const void* ptr, size_t size, size_t nmemb) {
	return fwrite(ptr, size, nmemb, fp);
}


size_t File::read(void* ptr, size_t size, size_t nmemb) {
	return fread(ptr, size, nmemb, fp);
}


char* File::getline() {
	if (::feof(fp)) return nullptr;
	size_t size = block;
	char* line = (char*)malloc(size * sizeof(char));
	if (line == nullptr) throw std::bad_alloc();
	if (!fgets(line, size, fp)) {
		free(line);
		return nullptr;
	}
	size_t curr = strlen(line);
	while ((line[curr - 1] != '\n') && !::feof(fp)) {
		if (curr == size - 1) {
			size = size * 2;
			line = static_cast<char*>(realloc(line, size * sizeof(char)));
			if (!line) {
				printf("%ld\n", size);
				fprintf(stderr, "Malloc error\n");
				exit(-1);
			}
			size_t readsize = size - curr;
			if (readsize > INT_MAX)
				readsize = INT_MAX - 1;
			fgets(&line[curr], readsize, fp);
			curr = strlen(line);
		}
	}

	if (line[curr - 1] == '\n') line[curr - 1] = '\0';
	return line;
}


void File::set_block_size(unsigned int size) {
	block = size;
}


int File::feof() {
	return ::feof(fp);
}
