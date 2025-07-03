DIFCMD= diff --strip-trailing-cr
CC= g++ -I ~/include ~/lib/lists.o

all: genpgm tst/floopy.cc install

genpgm: genpgm.cc ~/lib/lists.o 
	$(CC) -o genpgm genpgm.cc

tst/floopy.cc: ~/bin/genpgm
	cd tst; genpgm floopy	1 2; $(DIFCMD) floopy.cc floopy.gld
	
install: ~/bin/genpgm

~/bin/genpgm: genpgm
	cp genpgm ~/bin  
