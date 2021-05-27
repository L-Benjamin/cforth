.PHONY = build, clean

export CC = gcc
export FLAGS =  -ansi -O3

build:
	cd lang && make build
	cd parser && make build
	cp parser/forth forth

clean:
	cd lang && make clean
	cd parser && make clean
	rm -f forth