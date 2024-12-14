// tst5.cc is for extract, s command, some concat and subtraction.
//
#include "lists.h"
int main()
{
  lists_register();
  cout<<"tst5: extract, etc.\n";

  strlnk* t1 = new strlnk("abcdefghij");
  cout<<"str:"<<*t1<<": now -= cde :";
  *t1-="cde";
  cout<<*t1<<"\n";
  cout<<"length should be 7 now : "<<t1->strlnk_len<<"\n";
  
  *t1+="ggt";
  cout<<"now += ggt :"<<*t1<<": length should be 10 now : "<<t1->strlnk_len<<"\n";

  cout<<"#changes from s() : "<<t1->s("ab","cde",1);
  cout<<"\nnow change the ab to cde :"<<*t1<<": length (11 now) : "<<t1->strlnk_len<<"\n";

  return 0;
}
