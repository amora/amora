all: amora.edj
	python setup.py build

clean:
	python setup.py clean --all
	rm -f amora.edj

install:
	python setup.py install --root $(DESTDIR)

amora.edj:
	edje_cc amora.edc

run: all
	./amora amora.edj
