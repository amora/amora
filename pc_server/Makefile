test_presenter: main.o
	gcc -o test_presenter main.o -lX11

main.o: main.c
	gcc -c main.c -Wall

docs: Doxyfile.in
	doxygen Doxyfile.in

clean:
	rm -fr *~ main.o test_presenter html latex