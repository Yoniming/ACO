//
// Created by baoyixing on 3/25/21.
//

#ifndef CODE_FILE_H
#define CODE_FILE_H
#include <cstdio>
#include <string>

class File {
public:
	File(): fp(nullptr) {};
	File(const File&) = default;
	File operator=(const File&) = delete;
	File(const char* c_str, const char* mode);
	File(const std::string& str, const char* mode);
	~File() {
		fclose(fp);
	};

	size_t write(const void* ptr, size_t size, size_t nmemb);
	size_t read(void* ptr, size_t size, size_t nmemb);
	char* getline();
	void set_block_size(unsigned int size);
	inline unsigned int get_block_size() const { return block; };
	inline FILE* get_fp() { return fp; };
	int feof();

private:
	FILE* fp = nullptr;
	unsigned int block = 512;	 // getline中使用
};

#endif	  //CODE_FILE_H
