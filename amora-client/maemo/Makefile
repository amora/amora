all: amora.edj controller.edj
	python setup.py build

clean:
	python setup.py clean --all
	rm -f amora.edj controller.edj *.cache *.pyc *~

install:
	python setup.py install --root $(DESTDIR)

amora.edj:
	edje_cc amora.edc

controller.edj:
	edje_cc controller.edc

run: all
	./amora amora.edj -n
