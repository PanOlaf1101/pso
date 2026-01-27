#ifndef _MAP_H__
#define _MAP_H__

#include <stdio.h>

typedef struct Map {
	unsigned int w, h;
	double *values;
} Map;

void read_map(FILE *file, Map *m);

double mapv(const Map *m, unsigned int x, unsigned int y);

double fitness(const Map *m, const double x, const double y);

#endif //_MAP_H__
