#set page(paper: "a4", margin: (x: 1in, y: 1in), numbering: "I")
#set text(lang: "pl", size: 12pt)
#set par(justify: true, leading: 0.55em)
#set heading(numbering: "1.")
#show link: set text(fill: blue)
#show link: underline

#align(center, text(size: 24pt, [*Sprawozdanie z projektu końcowego: algorytm PSO*]))
#align(center, text(size: 16pt, [Autor: Olaf Mianowski, nr albumu: 343355]))

= Wstęp
Niniejsze Sprawozdanie jest omówieniem algorytmu PSO (ang. _Particle Swarm Optimization_) oraz mojej implementacji.

*Czym jest algorytm PSO?*

Algorytm optymalizacji cząstek roju (PSO) to algorytm rozpatrujący i poprawiający położenie czątek na mapie w celu znalezienia najlepszego miejsca. Przykładowym zastosowaniem takiego rozwiązania jest użycie dronów nad określonym terenem gdzie trzeba znaleźć nadajnik na podstawie siły emisji jego sygnału w różnych regionach. Cząstki-drony poprawiają swoje położenie zrówno na podstawie własnej pamięci, jak i ogólnej wiedzy całego roju. Dane z mapy są przetwarzane przez specjalną funkcję `fitness()`

= Opis matematyczny aktualizacji cząstek
Na zmianę w położeniu cząstek moją głównie wpływ 3 czynniki:
1. *Pamięć własna (_pBest_)* - najlepsza pozycja cząstki do tej pory
2. *Wiedza roju (_gBest_)* - najlepsza pozycja znaleziona przez cały rój
3. *Bezwładność* (masa) - cząstka ma tendencję do kontynuowania dotychczasowego kierunku ruchu

$ v_i (t+1) = w dot v_i (t) + c_1 dot r_1 dot (p B e s t_i - x_i (t)) + c_2 dot r_2 dot (g B e s t - x_i (t)) $
$ x_i (t+1) = x_i (t) + v_i (t+1) $

gdzie:

- $w$ $dash$ waga bezwładności (jak bardzo cząstka jest "ciężka")
- $c_1, c_2$ $dash$ współczynniki uczenia (wpływ własnego doświadczenia i wiedzy roju)
- $r_1, r_2$ $dash$ losowe liczby z zakresu $[0, 1]$ (element stochastyczny)
- $p B e s t_i$ $dash$ najlepsza znana pozycja danej cząstki
- $g B e s t$ $dash$ najlepsza znana pozycja całego roju

#pagebreak()

= implementacja
Kod źródłowy znajduje się w folderze `src` i jest podzielony na poniższe moduły:

== Pliki z kodem źródłowym
=== `main.c`
- Wczytywanie argumentów
- Struktura konfiguracji
- Wczytywanie danych z pliku konfiguracyjnego
- Alokacja i dealokacja pamięci (wszystkie zmienne wykorzystujące alokację pamięci na stercie są w zasięgu globalnym w tym pliku)

=== `map.c` / `map.h`
- Struktura mapy
- Wczytywanie danych o mapie z pliku
- Funkcja `fitness()`

=== `pso.c` / `pso.h`
- Struktury roju i cząstki
- Inicjalizacja roju i cząstek
- Aktualizacja zmiennych roju i cząstek zgodnie z równaniami we wcześniejszej sekcji sprawozdania

=== `logger.c` / `logger.h`
- Logowanie danych o cząstce w podanym pliku

=== `utils.c` / `utils.h`
- Funkcje do zgłaszania błędów (`error_msg()`, `verror_msg()`)
- Funkcja `rand_double()` zwracająca losową liczbę rzeczywistą w podanym przedziale

== Funkcja `fitness()`
```c
double mapv(const Map *m, unsigned x, unsigned int y) {
	return m->values[m->w * y + x];
}

double fitness(const Map *m, const double x, const double y) {
	const double fx = floor(x), fy = floor(y), cx = ceil(x), cy = ceil(y);

	if(fx < 0 || fy < 0 || fx+1 >= (double)m->w || fy+1 >= (double)m->h)
		return -INFINITY;
	return mapv(m, fx, fy)*(fx+1-x)*(fy+1-y) + mapv(m, fx, cy)*(fx+1-x)*(y-fy) + mapv(m, cx, fy)*(x-fx)*(fy+1-y) + mapv(m, cx, cy)*(x-fx)*(y-fy);
}
```

Funkcja wykorzystuje aktualne położenie cząstki i na podstawie średniej ważonej z najbliższych pól mapy ustala aktualną wartość dla cząstki.

#pagebreak()

= Kompilacja
Kod źródłowy można skompilować za pomocą pliku `Makefile`, z taką zawartością:
```makefile
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
```

= Wywołanie programu
```bash
./pso <plik mapy z mapą> -p <liczba cząstek> -i <liczba iteracji> \
    -c <plik konfiguracyjny> -n <co którą iterację logujemy dane> \
    -o <plik z logami>
```

Wartości domyślne:
- Liczba cząstek: 30
- Liczba iteracji: 100
- Parametry PSO: $wide w=0.5 wide c_1=1.0 wide c_2=1.0$
- Zapis postępów: 0 (brak zapisu)
- Plik z logami: wyjście standardowe (`stdout`)
