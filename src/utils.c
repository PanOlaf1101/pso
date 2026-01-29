#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

__attribute__((noreturn))
void error_msg(const char *restrict msg) {
	cleanup();
	fprintf(stderr, "\033[31;1mBłąd: %s!\033[0m\n", msg);
	exit(EXIT_FAILURE);
}

__attribute__((noreturn))
void verror_msg(const char *restrict msg, ...) {
	cleanup();
	char buf[0x4000];
	va_list args;
	va_start(args, msg);
	vsnprintf(buf, sizeof buf, msg, args);
	fprintf(stderr, "\033[31;1mBłąd: %s!\033[0m\n", buf);
	va_end(args);
	exit(EXIT_FAILURE);
}

void check_null(const void *const ptr) {
	if(ptr == NULL)
		error_msg("Nie można zaalokować pamięci");
}

double rand_double(double a, double b) {
	return ((double)rand() / RAND_MAX) * (b-a) + a;
}
