// plyiotst.cpp is a test for the read and write virtual functions
// for lists, strlnk objects
// R. Anderson April 8 1994
//

#include "lists.h"

int main()
{
lists_register(); 

cout<<"POLY IO TEST for lists, strlnk:\n";

vbc_link* l1=new strlnk("23 skidoo");
cout<<"\n *****Dump of string:\n";
l1->pdump(cout);
cout<<"\n *****End of string dump\n";

cout<<"\n writing a string to disk file piot1.ply\n";
ofstream polyout("piot1.ply",ios::out);
if (!polyout)
	{ //open failed
	cerr<< "cannot open piot1.ply for output\n";
	exit(-1);
	}
l1->pwrite(polyout);
// terminal character
polyout.close();
cout<<"wrote file and closed it\n";

cout<<"opening file piot1.ply to read\n";
ifstream polyin("piot1.ply",ios::in);
if (!polyin)
	{ //open failed
	cerr<< "cannot open piot1.ply for input\n";
	exit(-1);
	}

vbc_link* l2= read_obj(polyin);
polyin.close();

cout<<"\n *****Dump of object:\n";
l2->pdump(cout);
cout<<"\n *****End of object dump\n";

cout<<"\n\n testing ralist:\n";
ralist* l3 = new ralist();
l3->enq(new strlnk("now I will enq the object just read in"));
l3->enq(l2);
l3->enq(new strlnk("this is the third item on the list"));

cout<<"\n#################### pdump of ralist:\n";
l3->pdump(cout);

cout<<"\n##################### now writing to file\n";
polyout.open("piot1.ply",ios::out);
if (!polyout)
	{ //open failed
	cerr<< "cannot open piot1.ply for output\n";
	exit(-1);
	}
l3->pwrite(polyout);
// terminal character
polyout.close();
cout<<"wrote file and closed it\n";

cout<<"\n################ now reading file\n";
polyin.open("piot1.ply",ios::in);
if (!polyin)
	{ //open failed
	cerr<< "cannot open piot1.ply for input\n";
	exit(-1);
	}

vbc_link* l5= read_obj(polyin);
polyin.close();

cout<<"#################### pdump of object read:\n";
l5->pdump(cout);
cout<<"\n\n";

return 0;
}
