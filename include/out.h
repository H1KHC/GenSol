#ifndef __OUT_H__
#define __OUT_H__
#include <stdio.h>
#include "error.h"

struct OUT {
	FILE *fp;
	char buf[1048576];
	int pt;
	void putch(char ch) { buf[pt++] = ch; }
	void flush() { if(pt) fwrite(buf,sizeof(char),pt,fp), pt = 0; }
	void check() { if(pt > 1000000) flush(); }
	void init(const char *fileName) {
		fp = fopen(fileName, "w");
		if(!fp) throw ERR::FILE_OPEN_FAILED("File name: %s", fileName);
		pt = 0;
	}
	OUT() { pt = 0; }
	~OUT() { flush(); }
	OUT &operator << (char ch) {
		putch(ch);
		return *this;
	}
	OUT &operator << (const char* str) {
		while(*str) putch(*(str++));
		check();
		return *this;
	}
};

extern OUT out;
#endif