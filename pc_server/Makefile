CC=gcc


test_presenter: main.o
	$(CC) -o test_presenter main.o -lX11 -lbluetooth

main.o: main.c
	$(CC) -c main.c -Wall

docs: Doxyfile.in
	doxygen Doxyfile.in

clean:
	rm -fr *~ main.o test_presenter html latex


tags:
	find . -name '*.cpp' -o -name '*.h' -o -name '*.c' | xargs etags --append

