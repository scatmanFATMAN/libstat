all:
	cd src/ && $(MAKE)
	cd test/ && $(MAKE)

install:
	cd src/ && $(MAKE) install

clean:
	cd src/ && $(MAKE) clean
	cd test/ && $(MAKE) clean
