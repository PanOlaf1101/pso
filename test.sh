#!/bin/bash

make
if [[ $(uname) -eq "Linux" ]] then
	for i in $(ls maps); do
		valgrind --leak-check=full --show-leak-kinds=all -- ./pso "maps/$i" $@
	done
else #macOS
	for i in $(ls maps); do
		leaks --atExit -- ./pso "maps/$i" $@
	done
fi
