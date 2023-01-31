exe : huffman.o main.o
	gcc huffman.o main.o -o exe

huffman.o : huffman.c
	gcc -c huffman.c -o huffman.o -Wall -O -ggdb

main.o : main.c
	gcc -c main.c -o main.o -Wall -O -ggdb

clean :
	rm -rf *.o