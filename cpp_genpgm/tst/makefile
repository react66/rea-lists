CC= g++  -fmax-errors=3 -I ~/include ~/lib/lists.o

all: floopy install

floopy: floopy.cc ~/lib/lists.o
	$(CC) -o floopy floopy.cc

install: ~/bin/floopy

~/bin/floopy: floopy
	cp floopy ~/bin

clean:
	rm -f floopy ~/bin/floopy
