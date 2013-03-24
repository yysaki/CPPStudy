# Makefile
#.PHONY: all
#all: patchwork

patchwork: MAIN.O st.O
	g++ -o patchwork main.o ST.o

main.o: main.cpp ST.h Item.h
	g++ -c main.cpp ST.h Item.h

ST.o: ST.cpp ST.h Item.h
	g++ -c ST.cpp ST.h Item.h
