#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>

#include "utils.h"
#include "pso.h"
#include "logger.h"
#include "map.h"

static struct Config {
	FILE *config_file, *map_file, *log_file;
	char *config_file_name, *map_file_name, *log_file_name;
	unsigned int iter, part, log_iter;
} conf;

static Swarm swarm;
static Map map;
static char buf[PATH_MAX];

#define LIMIT UINT32_MAX
#define ALREADY_GIVEN(arg) error_msg("Parametr " arg " został podany wielokrotnie")
#define WRONG_VALUE(arg) error_msg("Niepoprawna wartość dla " arg)
#define NOT_GIVEN(arg) error_msg("Nie podano wartości dla " arg)

static void get_args(const int argc, char *argv[]) {
	conf.iter = LIMIT;
	conf.log_iter = LIMIT;
	conf.part = LIMIT;
	conf.log_file = stdout;
	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			if(strlen(argv[i]) != 2)
				verror_msg("Niepoprawny parametr \"%s\"", argv[i]);
			switch (argv[i][1]) {
				case 'p':
					if(conf.part != LIMIT)
						ALREADY_GIVEN("-p");
					if(++i >= argc)
						NOT_GIVEN("-p");
					if(sscanf(argv[i], "%u", &conf.part) != 1)
						WRONG_VALUE("-p");
					break;
				case 'i':
					if(conf.iter != LIMIT)
						ALREADY_GIVEN("-i");
					if(++i >= argc)
						NOT_GIVEN("-i");
					if(sscanf(argv[i], "%u", &conf.iter) != 1)
						WRONG_VALUE("-i");
					break;
				case 'n':
					if(conf.log_iter != LIMIT)
						ALREADY_GIVEN("-n");
					if(++i >= argc)
						NOT_GIVEN("-n");
					if(sscanf(argv[i], "%u", &conf.log_iter) != 1)
						WRONG_VALUE("-n");
					break;
				case 'c':
					if(conf.config_file != NULL)
						ALREADY_GIVEN("-c");
					if(++i >= argc)
						NOT_GIVEN("-c");
					conf.config_file = fopen(argv[i], "r");
					if(conf.config_file == NULL)
						verror_msg("Nie można otworzyć pliku \"%s\"", argv[i]);
					conf.config_file_name = argv[i];
					break;
				case 'o':
					if(conf.log_file != stdout)
						ALREADY_GIVEN("-o");
					if(++i >= argc)
						NOT_GIVEN("-o");
					conf.log_file = fopen(argv[i], "w+");
					if(conf.log_file == NULL) {
						conf.log_file = stdout;
						verror_msg("Nie można utworzyć pliku \"%s\"", argv[i]);
					}
					conf.log_file_name = argv[i];
					break;
				default:
					verror_msg("Niepoprawny parametr \"%s\"", argv[i]);
			}
		} else {
			if(conf.map_file != NULL)
				error_msg("Plik z mapą został już podany");
			conf.map_file = fopen(argv[i], "r");
			if(conf.map_file == NULL)
				verror_msg("Nie można otworzyć pliku z mapą \"%s\"", argv[i]);
			conf.map_file_name = argv[i];
		}
	}
	if(conf.map_file == NULL)
		error_msg("Nie podano pliku z mapą");
	if(conf.iter == LIMIT)
		conf.iter = 100;
	if(conf.part == LIMIT)
		conf.part = 30;
	if(conf.log_iter == LIMIT)
		conf.log_iter = 0;
}

void cleanup(void) {
	free(swarm.part);
	if(conf.config_file != NULL)
		fclose(conf.config_file);
	if(conf.map_file != NULL)
		fclose(conf.map_file);
	if(conf.log_file != stdout)
		fclose(conf.log_file);
}

int main(int argc, char *argv[]) {
	setlocale(LC_CTYPE, "C.UTF-8");
	get_args(argc, argv);
	srand(time(NULL) ^ clock());

	printf("Parametry wywołania:\nPlik konfiguracyjny: %s\nPlik z mapą: %s\nLiczba cząstek: %u\nLiczba iteracji: %u\nLogi raz na %u iteracje\nPlik z logami: %s\n\n", realpath(conf.config_file_name, buf), realpath(conf.map_file_name, buf), conf.part, conf.iter, conf.log_iter, realpath(conf.log_file_name, buf));

	if(conf.config_file == NULL) {
		swarm.c1 = 0.5;
		swarm.c2 = 0.5;
		swarm.r1 = 0.5;
		swarm.r2 = 0.5;
	} else {
		if(fscanf(conf.config_file, "%lf%lf%lf%lf", &swarm.c1, &swarm.c2, &swarm.r1, &swarm.r2) != 4)
			error_msg("Nie można wczytać danych z pliku konfiguracyjnego");
		fclose(conf.config_file);
		conf.config_file = NULL;
	}
	swarm.part_n = conf.part;

	read_map(conf.map_file, &map);
	fclose(conf.map_file);
	conf.map_file = NULL;

	init_swarm(&swarm, &map);

	log_to_file(stdout, 0, &swarm);
	for(uint32_t i = 1; i <= conf.iter; ++i) {
		update_swarm(&swarm, &map);
		if(conf.log_iter != 0 && i % conf.log_iter == 0)
			log_to_file(conf.log_file, i, &swarm);
	}

	cleanup();
	return 0;
}
