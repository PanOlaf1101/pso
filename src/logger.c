#include "logger.h"

#include <time.h>
#include <stdio.h>
#include <stdbool.h>

void log_to_file(FILE *file, int iterator, const Swarm *swarm) {
	static bool info = true;
	if(info) {
		fprintf(file, "Rój: [CZAS]\tITERACJA\tNAJLEPSZA_WARTOŚĆ\t(NAJLEPSZE_POŁOŻENIE)\nCząstka: ID\tWARTOŚĆ\t(POŁOŻENIE)\t(PRĘDKOŚĆ)\tNAJLEPSZA_WARTOŚĆ\t(NAJLEPSZE_POŁOŻENIE)\n");
		info = false;
	}

	char date[128];
	time_t timer = time(NULL);
	struct tm *tm = localtime(&timer);
	strftime(date, sizeof date, "%Y-%m-%d %H:%M:%S %z", tm);
	fprintf(file, "\n[%s]\t%3d\t%lg\t(%lg; %lg)\n", date, iterator, swarm->best_val, swarm->best_pos.x, swarm->best_pos.y);

	for(const Particle *i = swarm->part; i < swarm->part + swarm->part_n; ++i)
		fprintf(file, "%03d\t\t%.4lf\t\t(%.4lf; %.4lf)\t\t(%.4lf; %.4lf)\t\t%.4lf\t\t(%.4lf; %.4lf)\n", i->id, i->val, i->pos.x, i->pos.y, i->v.x, i->v.y, i->best_val, i->best_pos.x, i->best_pos.y);
}
