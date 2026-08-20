.DEFAULT_GOAL := build
.PHONY: build

build:
	cmake -S . -B build
	cmake --build build
