SRC = src/main.cc
DEP = ${SRC:.cc=.d}

CXXFLAGS += -lSDL2 -O3 -fopenmp

all: src/main

-include: ${DEP}

-include: src/main.d
