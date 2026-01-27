CFLAGS = -Wall -Wextra -pedantic -O2 -std=gnu99

pso: main.o utils.o pso.o map.o logger.o
	$(CC) $(CLAGS) -o $@ $^

clean:
	rm *.o pso

main.o: main.c
utils.o: utils.c utils.h
pso.o: pso.c pso.h
map.o: map.c map.h
logger.o: logger.c logger.h
