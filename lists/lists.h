/** lists.h is the header file for bottom level objects
** REA July 27 1993 
	REA Feb 7 1994
	REA March 15 1994 ...put sort in lists
	REA July 7 to_int in strlnk
	REA May 16 1995 static ofs in link, for indent levels
        REA Oct 30 1996 removing link* parm in newobj,pwrite,pdump
	REA NOV 25 1998 changing link to vbc_link to avoid name 
	aliasing/conflicts
	REA Jun 16 1999 added ifdef _MSC_VER for porting to NT.
	REA jan 2003 fix c++ include headers to get rid of warnings
	    (eg. <strstream> not <strstream.h>
   REA July 14 2006 removed string and added internal ccpy, slen, ccat
      because of deprecations and differences in ms version.
   REA July 27 2019 fixing strlnk:: csv_field, adding copy_range


**/

/** vbc_link is the bottom level object : abstract class.
**  list is a descendant.
**  also define a strlnk as a string type
**  **/
#ifndef lists_h
#define lists_h
#include "clasnum.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
using namespace std;
typedef unsigned char uchar;
typedef unsigned short ushort;

// enable the following line to turn off assert checks
//#define NDEBUG
#include <assert.h>
class vbc_link
{
public:
   vbc_link* p,*n;
   static int ofs;
   virtual long cnum(){return 0;}
   virtual int less_than(vbc_link* cpos){return 0;}
   vbc_link(){p=0;n=0;}
   virtual ~vbc_link(){};
   vbc_link(vbc_link*pnew,vbc_link*nnew){p=pnew;n=nnew;}
   virtual vbc_link* newobj(istream&);
   virtual ostream& pwrite(ostream&);
   virtual ostream& pdump(ostream&);
   virtual ostream& indent(ostream&);
};

class rlist:public vbc_link
{
public:
   vbc_link* head,*tail,*work;
   int count;
   long cnum(){return cnum_rlist;}
   rlist():vbc_link(){head=0;tail=0;work=0;count=0;}
   ~rlist();
   rlist(vbc_link*pnew,vbc_link*nnew);
   rlist(const char*); //read a file into strlnks
   rlist(const char*,int); //read a file into strlnks of max length
   vbc_link* newobj(istream&); // poly read, write, dump
   ostream& pwrite(ostream&);
   ostream& pdump(ostream&);
   ostream& write_str(ostream&);
   virtual vbc_link* enq(vbc_link*,vbc_link*); //second ptr is position to 
      //insert after (0=prepend) lnew gets set to 0, returns work ptr
   virtual vbc_link* enq(vbc_link*); //enq at end of list
   virtual vbc_link* deq(vbc_link*); // uses work if lnew=0
   virtual vbc_link* pop_tail(); // calls deq(tail)
   rlist* operator +(rlist*); // list1, list2 get 
                                      // deleted (=0) afterwards
   virtual vbc_link* insert(rlist*,vbc_link*);//insert the list after vbc_link, delete list
   virtual void sort();
   long tail_cnum();
};

class ralist:public rlist // random access list (indexed)
{
public:
   vbc_link** ptr_table;
   long cnum(){return cnum_ralist;}
   ralist():rlist(){ptr_table=0;}
   ~ralist();
   ralist(vbc_link*pnew,vbc_link*nnew);
   vbc_link* newobj(istream&); // poly read, write, dump
   ostream& pwrite(ostream&);
   ostream& pdump(ostream&);
   vbc_link* enq(vbc_link*,vbc_link*); //second ptr is position to 
      //insert after (0=prepend) lnew gets set to 0, returns work ptr
   vbc_link* enq(vbc_link*); //enq at end of list
   vbc_link* deq(vbc_link*); // uses work if lnew=0
   vbc_link* deq(int); // end one if no such element
   vbc_link* deq(int,int); // assumes 2d ralist  
   vbc_link* ref(int,int); // 2d, but get ptr, do not deq   
   ralist* operator +(ralist*); // list1, list2 get deleted (=0) afterwards
   vbc_link* insert(rlist*,vbc_link*); //insert the list after vbc_link, delete list
   void sort();
   void fix_ptrs();
};

class strlnk:public vbc_link
{
friend ostream&
	operator<<(ostream&, strlnk&);
public:
   int strlnk_len; // actual array is strlnk_len+1 for the \0 eol
   char* str1;
   // virtual (vbc_link) functions:
   long cnum(){return cnum_strlnk;}
   strlnk():vbc_link(){str1=0;strlnk_len=0;}
   ~strlnk();
   vbc_link* newobj(istream&); // poly read, write, dump
   strlnk(istream&);             // read string from input stream
   strlnk(const char*);    // copy construct from string
   strlnk(strlnk&);  // copy construct from strlnk
   ostream& pwrite(ostream&);
   ostream& pdump(ostream&);
// string manipulation
   strlnk& operator+(strlnk&); // b+c returns new string
   strlnk& operator+=(strlnk&);     // += function
   strlnk& operator+=(const char*); // appends string
   strlnk& operator-=(strlnk&); // deletes matching string
   strlnk& operator-=(const char*); // deletes matching string 
   strlnk& operator+=(const int&);  // appends decimal number to string 
   int insert(strlnk&,int);  // insert string, return index just past string
   int insert(const char*,int);  // insert chars, return index just past string
   int cut_range(int,int);  //cut characters, return length
   strlnk* extract(int,int);  //extract range
   strlnk* copy_range(const int&,const int&);  //copy range 
   strlnk* csv_field(int); //copy csv field, first field is 0
// processing into a file:
   int getline(istream&); // delete str1, read in a new line
   int getline(istream&,const int max_size); // as above, max line size
   int inprej(istream&);  // Read until str1=start of line return 0 for eof
   int inprej(istream&,strlnk&);    // inprej, save prev. line. 
// find strings:
   int strpos(const char*);  // return position of substring, -1 if not there
   int strposa(const char*);  // position after substring, -1 if not there
   int chrpos(const char*);  // position of soonest char match, -1 if not there
   int to_int();      // return integer value of str1
   double to_double(); // return str1 value as a double
   int lower_case();  // translate str1 to lower case
   int upper_case();  // translate str1 to upper case
   int pad_bslash();  // insert '\' before \ or " or '
   int operator==(const strlnk&);      // return 1 if equal
   int operator==(const char*);      // return 1 if equal
   int to_hex(const int &, const int &); // convert int to hex, parm2 is length
   int hex_to_uns(const int &); // n hex char->unsigned, -1 if error.
   int hex_to_uns(const int &, const int &); // start, length to convert.
   int bv_to_int(); // bitvector->integer. 0 if bad char.
   int s(const char*,const char*,const int&); //s(ubstitute) 2nd string for first, n times maximum
};
extern rlist* obj_reg;
void lists_register();
vbc_link* read_obj(istream&);

#endif

