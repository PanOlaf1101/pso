#ifndef _PSO_H__
#define _PSO_H__

#include "map.h"

typedef struct Coords {
	double x, y;
} Coords;

typedef struct Particle {
	Coords v, pos, best_pos;
	double val, best_val;
	unsigned int id;
} Particle;

typedef struct Swarm {
	Particle *part;
	Coords best_pos;
	double best_val, w, c1, c2, r1, r2;
	unsigned int part_n;
} Swarm;

void init_swarm(Swarm *swarm, const Map *m);
void update_swarm(Swarm *swarm, const Map *m);

#endif //_PSO_H__
