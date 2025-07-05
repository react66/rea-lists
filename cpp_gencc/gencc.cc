// program gencc.cc to generate a cc program that writes another
//         file. Input is a file.
//
#include "lists.h"

//*******************************************************************
//*******************************************************************

int main(int argc, char* argv[])
{
lists_register();
ofstream ofile;

if (argc!=3) {
  cout<<"*******************************************************************\n";
  cout<<"*******************************************************************\n";
	cout<<" gencc program rev 1.2 2016\n";
  cout<<"USAGE:\n";
  cout<<"  gencc <ifile_name> <ofile_name> \n";
  cout<<"EXAMPLE:\n";
  cout<<"gencc x1.vhd genx1.cc \n";
  cout<<" reads x1.vhd, produces genx1.cc, makefile\n";
  cout<<" where makefile is for gcc on linux\n";
  cout<<" The resulting genx1 program will reproduce x1.vhd.\n\n";
  cout<<"*******************************************************************\n";
  cout<<"*******************************************************************\n";
  return(1);
  }

// make some strlnks to hold parms
strlnk* parm[10]; // argc-1];
for (int i=0;i<argc-1;i++)parm[i] = new strlnk(argv[i+1]);

  
// read the input file
rlist* iput = new rlist(parm[0]->str1);
cout<<" input file: "<<*parm[0]<<" #lines = "<<iput->count<<"\n";

ofile.open(parm[1]->str1,ios::out);
if (!ofile) {
	cerr<<"ERROR writing to file: "<<*parm[1]<<"\n";
	exit(1);
}
// now produce output file ofile
cout<<" writing output file: "<<*parm[1]<<"\n\n";
*parm[1]-=".cc";
//prepend some header and includes:
ofile<<"//\n";
ofile<<"//   file: "<<*parm[1]<<"\n\n\n";
ofile<<"#include \"lists.h\"\n\n";
ofile<<"//*************************************\n";
ofile<<"//*************************************\n\n";
ofile<<"int main(int argc, char* argv[])\n";
ofile<<"{\n";
ofile<<"lists_register();\n";
ofile<<"ifstream infile;\n";
ofile<<"ofstream ofile;\n\n";
ofile<<"// make some strlnks to hold parms\n";
ofile<<"strlnk* parm[10];\n";
ofile<<"for (int i=0;i<argc-1;i++)parm[i] = new strlnk(argv[i+1]);\n";
ofile<<"if (argc!=1) {\n";
ofile<<"  cout<<\"*******************************************************************\\n\";\n";
ofile<<"  cout<<\"*******************************************************************\\n\";\n";
ofile<<"  cout<<\"USAGE:\\n\";\n";
ofile<<"  cout<<\" "<<*parm[1]<<" \\n\";\n";
ofile<<"  cout<<\"  Produces file: "<<*parm[0]<<" \\n\";\n";
ofile<<"  cout<<\"*******************************************************************\\n\";\n";
ofile<<"  cout<<\"*******************************************************************\\n\";\n";
ofile<<"  return(1);\n";
ofile<<"}\n";
ofile<<"ofile.open(\""<<*parm[0]<<"\",ios::out);\n";
// reproduce the contents of the input file:
strlnk* s1;
strlnk* s2 = (strlnk*)(iput->head);
while (s2){
  s1 = new strlnk("ofile");
  s2->pad_bslash();
  if (s2->strlnk_len) {
    *s1 += "<<\"";
    *s1 += *s2;
    *s1 += "\"";
    }
  ofile<<*s1<<"<<\'\\n\';\n";
  s2= (strlnk*)(s2->n);
  delete s1;
  }  
ofile<<"ofile.close();\n";
ofile<<"return 0;\n}\n";
ofile.close();

*parm[1]-=".cc";
ofile.open("makefile",ios::out);
if (!ofile) {
	cerr<<"ERROR writing to file: makefile"<<"\n";
	exit(1);
}
cout<<" writing output file: makefile"<<"\n";
ofile<<"CC= g++  -fmax-errors=3 -I ~/include \
~/lib/lists.o"<<endl;
ofile<<"all: "<<*parm[1]<<" install\n\n";
ofile<<*parm[1]<<": "<<*parm[1]<<".cc\n";
ofile<<"	$(CC) -o "<<*parm[1]<<" "<<*parm[1]<<".cc\n\n";
ofile<<"install: ~/bin/"<<*parm[1]<<"\n\n";
ofile<<"~/bin/"<<*parm[1]<<":"<<*parm[1]<<"\n";
ofile<<"	cp "<<*parm[1]<<" ~/bin\n";
ofile.close();

cout<<"\n***** gencc Program Finished\n";
return(0);
}


