name=test

obj=test.o

cc=gcc
cflags=-Wall -g
ldflags=-lstat

all: $(name)

$(name): $(obj)
	$(cc) -o $@ $^ $(ldflags)

%.o: %.c
	$(cc) -o $@ -c $< $(cflags)

clean:
	rm -f $(obj) $(name)
