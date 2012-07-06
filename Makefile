CFLAGS += $(shell pkg-config --cflags opencv)
LIBS += $(shell pkg-config --libs opencv)

all: main

main:
	g++ $(CFLAGS)  main.cpp -o firstc $(LIBS)
