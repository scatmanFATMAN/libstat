name=libstat.so

obj=stat.o strlcpy.o

cc=gcc
cflags=-fPIC -Wall -D_GNU_SOURCE -g
ldflags=-shared

all: $(name)

$(name): $(obj)
	$(cc) -o $@ $^ $(ldflags)

%.o: %.c
	$(cc) -o $@ -c $< $(cflags)

install: all
	cp ../api/stat.h /usr/local/include
	cp libstat.so /usr/local/lib

clean:
	rm -f $(obj) $(name)
