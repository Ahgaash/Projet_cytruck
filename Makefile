prog: main.o trait_s.o traitT_4.o
	gcc -o prog main.o trait_s.o traitT_4.o
	
main.o: main.c traits.h traitT.h
	gcc -c main.c -o main.o

trait_s.o: traits_22.c traits.h
	gcc -c traits_22.c -o trait_s.o

traitT_4.o: traitT_42.c traitT.h
	gcc -c traitT_42.c -o traitT_4.o

