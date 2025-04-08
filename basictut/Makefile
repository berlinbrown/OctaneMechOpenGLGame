# Basic makefile wrapper
.PHONY: build run clean

build:
	cmake -S . -B build && cmake --build build

run: build
	./build/bin/basictut

clean:
	rm -rvf build

