#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *VERSION = "1.0.0";
constexpr unsigned buffer_size = 128;

static inline void swap(char *x, char *y)
{
	char temp = *x;
	*x = *y;
	*y = temp;
}

// This function is transformative, it creates no copies
// but it alters the input, be warned.
static inline void reverse(char *buf)
{
	if(buf == nullptr) return;

	char *end = buf + strlen(buf) - 1;
	while(buf < end)
	{
		swap(buf, end);
		buf++;
		end--;
	}

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

void usage(void)
{
	printf("RaDiX (%s): a converter from decimal to arbitrary radix\n", VERSION);
	printf("created by anson <thesearethethingswesaw@gmail.com>\n\n");
	printf("usage:\n\trdx (-h / --help)\n\trdx --version\n\trdx <number> <base>\n\n");
	printf("options:\n\t%10s\t%s\n", "<number>", "a decimal to convert");
	printf("\t%10s\t%s\n", "<base>", "a decimal representing the base to convert to\n\n");
	printf("copyright (c) 2024, see LICENSE for related details\n");
}

int main(int argc, char *argv[])
{
	if(argc < 2) { fprintf(stderr, "%s: too few arguments, try \"--help\"\n", argv[0]); return -1; }

	char *buffer = malloc(buffer_size), *program = argv[0];
	if(buffer == nullptr) { perror(program); return -1; }

	int c, number = 0, base = 0;
	// Iterate through all arguments sent, character by character
	while(--argc > 0 && (*++argv)[0] != '\0')
        {
		if((*argv)[0] != '-')
		{
			if(base != 0)
			{
				fprintf(stderr, "%s: discarded program input -- %s\n", program, *argv);
				continue;
			}

			if(number == 0) number = atoi(*argv);
			else		base = atoi(*argv);
		}

		if((*argv)[0] == '-')
		{
			// If there's another dash, then it's a long option.
			// Move the pointer up 2 places and compare the word itself.
			if((*argv)[1] == '-')
			{
				// Using continue statements here so that the user
				// can use both single character and long options
				// simultaniously, and the loop can test both.
				if(strcmp((*argv) + 2, "help")    == 0)  { usage(); return 0;   }
				if(strcmp((*argv) + 2, "version")  == 0)
				{
					fprintf(stderr, "RaDiX (%s): a converter from decimal to arbitrary radix\n", VERSION);
					return 0;
				}
			}
			while((c = *++argv[0]))
			{
				// Single character option testing here.
				switch(c)
				{
					case 'h': usage(); return 0;
					// This error flag can either be set by a
					// completely unrelated character inputted,
					// or you managed to put -option instead of
					// --option.
					default : fprintf(stderr, "%s: unknown option -- \"%s\". try \"--help\"\n", *argv, program); return -1;
				}
			}

			continue;
		}
        }

	if(number == 0)	   { fprintf(stderr, "%s: number not defined, retry\n", program); return -1; }
	else if(base == 0) { fprintf(stderr, "%s: base not defined, retry\n", program); return -1; }

	switch(rdx(number, base, buffer))
	{
		case BAD_NUM:	fprintf(stderr, "%s: number input is negative or nonexistant, retry\n", program); return -1;
		case BAD_BASE:	fprintf(stderr, "%s: base input is negative or nonexistant, retry\n", program); return -1;
		case BAD_BUF:	fprintf(stderr, "%s: internal buffer error\n", program); return -1;
		case GOOD: 	printf("%s\n", buffer); break;
	}

	free(buffer);
	return 0;
}
