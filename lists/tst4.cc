/** tst4.cpp is a test program for lists **/
// REA Apr.5 1995
// REA Apr.8 1998
#include "lists.h"
int main()
{
  lists_register();

  cout<<"tst4 program: tst4.cpp for lists\n";
  strlnk* s1 = new strlnk("23 skidoo_");
  strlnk* s2 = new strlnk("street, Oregon");

  strlnk* s3 = new strlnk("REA Computer Technology");


  cout<<"op +=(strtyp&)\n";
  cout<<"s1="<<*s1<<"\n";
  cout<<"s2="<<*s2<<"\n";
  *s1+=*s2;
  cout<<"*s1+=*s2:"<<*s1<<":\n";
  cout<<"op strtyp& strtyp::operator+(strtyp&)\n";
  strlnk* s4 = new strlnk(".INC OREGON");
  cout<<"s3="<<*s3<<"\n";
  cout<<"s4="<<*s4<<"\n";
  strlnk* s5 = &(*s3 + *s4);
  cout<<"s5= &(*s3+*s4)>"<<*s5<<"<\n";
  strlnk* s6 = new strlnk(*s5);
  cout<<"s6= new strlnk(*s5)>"<<*s6<<"<\n";
  cout<<"s3=>"<<*s3<<"<\n";
  cout<<"s4=>"<<*s4<<"<\n";
  cout<<"\ntesting    strlnk& operator+=(const int&); ";
  cout<<"*s5+=556 then +=888:\n>";
  *s5+=556;
  cout<<*s5<<"<\n>";
  int iii=888;
  cout<<(*s5+=iii)<<"<\n";
  cout<<"\n *********** testing list read of file tst2.gold\n";
  rlist* ra=new rlist("tst2.gold");
  ra->pdump(cout);
  cout<<"\n *********** finished testing list read of file tst2.gold\n";
   

  return 0;
}
