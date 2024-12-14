/** tst2.cpp is easywin version - should also compile on unix **/
// REA July 27 1993
#include "lists.h"
int main()
{
  lists_register();
  int i;

  cout<<"tst1 proj: tst2.cpp for lists\n";
  cout<<"demonstrating:\ncin>>ch1\nenter char, hit return:";
  char ch1;
  cin>>ch1;
  cout<<"\nch1:"<<ch1<<"\n::::new strlnk showing string:\n";
  strlnk* s1 = new strlnk("123456_string");
  for (i=0;i<s1->strlnk_len;i++)cout<<s1->str1[i];
  cout<<"\n operator <<*s1<<\"\\n\";";
  cout<<"\n"<<*s1<<"\n Now s1->pdump(obj_reg,cout):\n";
  s1->pdump(cout); //output the object
  cout<<"\nnow *s1-=\"3456\"";
  *s1-="3456";
  cout<<"\n>"<<*s1<<"<\n";
  cout<<"\nrlist test:";
  rlist* rl = new rlist();
  rl->enq(s1,0);
  rl->enq(new strlnk("2_string"),rl->work);
  cout<<"\npdump of list of 2 strlnk's:";
  rl->pdump(cout);
  cout<<"\nEnter a string:";
  char inbuf[256];
  cin.getline(inbuf,256); //clear input buffer
  cin>>inbuf; // really read prompted data
  delete s1;
  s1=new strlnk(&inbuf[0]);
  s1->pdump(cout); //output the object
  cout<<"\nconcatenating !#334!";
  *s1+= "#344";
  s1->pdump(cout); //output the object
  cout<<"\n\n";
  return 0;
}
