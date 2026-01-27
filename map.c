#include "map.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

void read_map(FILE *file, Map *m) {
	if(fscanf(file, "%u %u", &m->w, &m->h) != 2)
		error_msg("Nie można wczytać danych o mapie");
	m->values = malloc(m->w * m->h * sizeof(double));
	if(m->values == NULL)
		error_msg("Nie można zaalokować pamięci");
	for(size_t i = 0; i < m->h * m->w; ++i) {
		if(fscanf(file, "%lf", &m->values[i]) != 1)
			error_msg("W pliku z mapą są niepoprawne dane");
	}
}

double mapv(const Map *m, unsigned x, unsigned int y) {
	return m->values[m->w * y + x];
}

double fitness(const Map *m, const double x, const double y) {
	const double fx = floor(x), fy = floor(y), cx = ceil(x), cy = ceil(y);

	if(fx < 0 || fy < 0 || fx+1 >= (double)m->w || fy+1 >= (double)m->h)
		return -1000.0;
	return mapv(m, fx, fy)*(fx+1-x)*(fy+1-y) + mapv(m, fx, cy)*(fx+1-x)*(y-fy) + mapv(m, cx, fy)*(x-fx)*(fy+1-y) + mapv(m, cx, cy)*(x-fx)*(y-fy);
}
