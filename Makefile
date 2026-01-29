CFLAGS = -Wall -Wextra -pedantic -O2 -std=gnu99

pso: src/main.o src/utils.o src/pso.o src/map.o src/logger.o
	$(CC) $(CLAGS) -o $@ $^

clean:
	rm src/*.o pso

src/main.o: src/main.c
src/utils.o: src/utils.c src/utils.h
src/pso.o: src/pso.c src/pso.h
src/map.o: src/map.c src/map.h
src/logger.o: src/logger.c src/logger.h
