#include "pso.h"
#include "utils.h"
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

void init_swarm(Swarm *swarm, const Map *m) {
	swarm->part = malloc(sizeof(Particle) * swarm->part_n);
	check_null(swarm->part);
	swarm->best_val = -1000.0;

	for(Particle *i = swarm->part; i < swarm->part + swarm->part_n; ++i) {
		i->v.x = 0;
		i->v.y = 0;
		i->pos.x = rand_double(0, m->w);
		i->pos.y = rand_double(0, m->h);
		i->best_pos = i->pos;
		i->val = fitness(m, i->pos.x, i->pos.y);
		i->best_val = i->val;
		i->id = i - swarm->part + 1;
		if(i->val > swarm->best_val) {
			swarm->best_val = i->val;
			swarm->best_pos = i->pos;
		}
	}
}

void update_swarm(Swarm *swarm, const Map *m) {
	for(Particle *i = swarm->part; i < swarm->part + swarm->part_n; ++i) {
		i->v.x = swarm->w * i->v.x + swarm->c1 * swarm->r1 * (i->best_pos.x - i->pos.x) + swarm->c2 * swarm->r2 * (swarm->best_pos.x - i->pos.x);
		i->v.y = swarm->w * i->v.y + swarm->c1 * swarm->r1 * (i->best_pos.y - i->pos.y) + swarm->c2 * swarm->r2 * (swarm->best_pos.y - i->pos.y);
		i->pos.x += i->v.x;
		i->pos.y += i->v.y;
		i->val = fitness(m, i->pos.x, i->pos.y);
		if(i->val > i->best_val) {
			i->best_val = i->val;
			i->best_pos = i->pos;
			if(i->val > swarm->best_val) {
				swarm->best_val = i->val;
				swarm->best_pos = i->pos;
			}
		}
	}
}
