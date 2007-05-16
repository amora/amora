CC=gcc


test_presenter: main.o linux.o
	$(CC) -o test_presenter main.o linux.o -lX11 -lbluetooth

main.o: main.c linux.o
	$(CC) -c main.c -Wall

linux.o: linux.c
	$(CC) -c linux.c -Wall

docs: Doxyfile.in
	doxygen Doxyfile.in

clean:
	rm -fr *~ main.o test_presenter html latex semantic.cache TAGS linux.o protocol.o


tags:
	find . -name '*.cpp' -o -name '*.h' -o -name '*.c' | xargs etags --append

