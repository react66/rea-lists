DIFCMD= diff --strip-trailing-cr
CC= g++ -I ~/include ~/lib/lists.o

all: gencc tst/qq.vhd install

gencc: gencc.cc ~/lib/lists.o 
	$(CC) -o gencc gencc.cc

tst/qq.vhd: ~/bin/gencc
	cd tst; cp ex/qq.vhd .; gencc qq.vhd genqq.cc;rm qq.vhd; make; chmod +x genqq; genqq; $(DIFCMD) qq.vhd ex/qq.vhd
	
install: ~/bin/gencc

~/bin/gencc: gencc
	cp gencc ~/bin  

clean:
	rm -f gencc ~/bin/gencc tst/genqq tst/*.cc tst/makefile tst/*.vhd
