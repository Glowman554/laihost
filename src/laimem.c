#include <lai/host.h>

void* memcpy(void* dest, const void* src, size_t n){
	unsigned char* d = (unsigned char*) dest;
	const unsigned char* s = (unsigned char*) src;

	while (n--) {
		*d++ = *s++;
	}

	return dest;
}

int memcmp(const void * _s1, const void* _s2, size_t n) {

	const unsigned char* s1 = (unsigned char*) _s1;
	const unsigned char* s2 = (unsigned char*) _s2;

	while(n--) {
		if(*s1 != *s2) {
			return *s1 - *s2;
		}
		++s1;
		++s2;
	}
	return 0;
}

void* memset(void* buf, int c, int n){
	unsigned char* p = (unsigned char*) buf;

	while (n--) {
		*p++ = c;
	}

	return buf;
}