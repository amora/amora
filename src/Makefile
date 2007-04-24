test_presenter: main.o
	gcc -o test_presenter main.o -lX11

main.o: main.c
	gcc -c main.c -Wall

clean:
	rm -f *~ main.o test_presenter