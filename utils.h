#ifndef _UTILS_H__
#define _UTILS_H__

typedef unsigned int u32;

extern void cleanup(void);

__attribute__((noreturn))
void error_msg(const char *restrict msg);

__attribute__((noreturn))
void verror_msg(const char *restrict msg, ...);

void check_null(const void *const ptr);

double rand_double(double a, double b);

#endif //_UTILS_H__
