
// ex1.cc

#include "ex1.h"



//    START OF CLASS: ex1     *******************

vbc_link* ex1::newobj(istream& rst)
{
ex1* il = new ex1();
il->thing_one= (strlnk*)read_obj(rst);
rst>>il->thing_two;
rst>>il->thing_three;
il->thing_4= (rlist*)read_obj(rst);
return il;
}


ostream& ex1::pwrite(ostream& wst)
{
wst<<" "<<cnum();
if (thing_one)thing_one->pwrite(wst);
else wst<<" 0";
wst<<" "<<thing_two;
wst<<" "<<thing_three;
if (thing_4)thing_4->pwrite(wst);
else wst<<" 0";
return wst;
}

ostream& ex1::pdump(ostream& wst)
{
vbc_link::ofs+=1;
vbc_link::indent(wst);
wst<<"ex1 cnum= "<<cnum()<<"\n";
vbc_link::indent(wst);
wst<<"thing_one:";
if (thing_one)thing_one->pdump(wst);
else wst<<" 0";
wst<<"\n";
vbc_link::indent(wst);
wst<<"thing_two:";
wst<<thing_two<<"\n";
vbc_link::indent(wst);
wst<<"thing_three:";
wst<<thing_three<<"\n";
vbc_link::indent(wst);
wst<<"thing_4:";
if (thing_4)thing_4->pdump(wst);
else wst<<" 0";
wst<<"\n";
vbc_link::ofs-=1;
return wst;
}

ex1::~ex1()
{
if (thing_one) delete(thing_one);
if (thing_4) delete(thing_4);
}


//    END OF CLASS: ex1     *******************


//    START OF CLASS: ex2     *******************

vbc_link* ex2::newobj(istream& rst)
{
ex2* il = new ex2();
rst>>il->e33;
il->tree3= (strlnk*)read_obj(rst);
return il;
}


ostream& ex2::pwrite(ostream& wst)
{
wst<<" "<<cnum();
wst<<" "<<e33;
if (tree3)tree3->pwrite(wst);
else wst<<" 0";
return wst;
}

ostream& ex2::pdump(ostream& wst)
{
vbc_link::ofs+=1;
vbc_link::indent(wst);
wst<<"ex2 cnum= "<<cnum()<<"\n";
vbc_link::indent(wst);
wst<<"e33:";
wst<<e33<<"\n";
vbc_link::indent(wst);
wst<<"tree3:";
if (tree3)tree3->pdump(wst);
else wst<<" 0";
wst<<"\n";
vbc_link::ofs-=1;
return wst;
}

ex2::~ex2()
{
if (tree3) delete(tree3);
}


//    END OF CLASS: ex2     *******************

void ex1_register()
{
if (!(obj_reg)) lists_register();
obj_reg->enq(new ex1(),obj_reg->work);
obj_reg->enq(new ex2(),obj_reg->work);
}

