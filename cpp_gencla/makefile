DIFCMD= diff --strip-trailing-cr
CC= g++ -I ~/include ~/lib/lists.o

all: gencla tst/ex1.cc install

gencla: gencla.cc ~/lib/lists.o 
	$(CC) -o gencla gencla.cc

tst/ex1.cc: ~/bin/gencla
	cd tst; gencla ex1; $(DIFCMD) ex1.cc ex1.gld; $(DIFCMD) ex1.h ex1_h.gld
	
install: ~/bin/gencla

~/bin/gencla: gencla
	cp gencla ~/bin  
