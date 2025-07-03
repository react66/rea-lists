
// ex1.h

#ifndef ex1_h 
#define ex1_h

#include "lists.h"

const int cnum_ex1 = 300;
const int cnum_ex2 = 301;


typedef class ex1 *Pex1;
class ex1:public vbc_link
{
public:
strlnk *thing_one;
strlnk thing_two;
int thing_three;
rlist *thing_4;
long cnum(){return cnum_ex1 ;}
ex1():vbc_link(){
thing_one=0;
thing_4=0;}
~ex1();
vbc_link* newobj(istream&);
ostream& pwrite(ostream&);
ostream& pdump(ostream&);
};



typedef class ex2 *Pex2;
class ex2:public vbc_link
{
public:
int e33;
strlnk *tree3;
long cnum(){return cnum_ex2 ;}
ex2():vbc_link(){
tree3=0;}
~ex2();
vbc_link* newobj(istream&);
ostream& pwrite(ostream&);
ostream& pdump(ostream&);
};

void ex1_register();

#endif
