#include <stdio.h>
#include <stdlib.h>

inline void swap(char *x, char *y)
{
	char temp = *x;
	*x = *y;
	*y = temp;
}

inline void reverse(char *buf)
{
	if(buf == nullptr) return;

	// TODO

	return;
}

// Using these enum types, so that whatever is returned can
// be examined and print accordingly
typedef enum return_val { GOOD = 0, BAD_NUM, BAD_BASE, BAD_BUF } ret_t;
static ret_t rdx(int num, int base, char *buf)
{
	if(num <= 0) 		    return BAD_NUM;
	if(base <= 0 || base >= 37) return BAD_BASE;
	if(buf == nullptr)	    return BAD_BUF;

	static const char *alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int mod = 0;
	for(int i = 0; num != 0; i++)
	{
		mod = num % base;
		num /= base;

		sprintf(buf + i, "%c", alpha[mod]);
	}

	reverse(buf);
	return GOOD;
}

constexpr unsigned buffer_size = 128;

int main(int argc, char *argv[])
{
	char *buffer = malloc(buffer_size);
	if(buffer == nullptr) { perror(argv[0]); return -1; }

	int number = 56, base = 2;
	switch(rdx(number, base, buffer))
	{
		case BAD_NUM:	fprintf(stderr, "%s: number input is negative or nonexistant, retry\n", argv[0]); return -1;
		case BAD_BASE:	fprintf(stderr, "%s: base input is negative or nonexistant, retry\n", argv[0]); return -1;
		case BAD_BUF:	fprintf(stderr, "%s: internal buffer error\n", argv[0]); return -1;
		case GOOD: break;
	}

	printf("%s\n", buffer);
	return 0;
}
