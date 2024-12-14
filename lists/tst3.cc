/** tst3.cc is on unix **/
// REA July 27 1993
#include "lists.h"
int main()
{
  lists_register();
  char ch1;
  cout<<"*************************************************************\n";
  cout<<"*************************************************************\n";
  cout<<"lists proj: tst3.cpp [cc] for lists\n:";
  strlnk* s1 = new strlnk("123_sTRinG_OF_mixed_CASE");
  cout<<"\n"<<*s1<<"\nnow s1->lower_case()\n";
  s1->lower_case();
  cout<<*s1<<"\n"; //output the object
  cout<<"\n\nrlist test:";
  rlist* rl = new rlist();
  rl->enq(s1,0);
  rl->enq(new strlnk("2_string"),rl->work);
  rl->enq(new strlnk("3_string"),rl->work);
  rl->enq(new strlnk("blue_string"),rl->work);
  cout<<"\npdump of list of 4 strlnk's: rl->pdump(obj_reg,cout)\n";
  rl->pdump(cout);
  cout<<"\n\n******************** new test ***************\n";
  cout<<"\nEnter a string for cin>>string:";
  char inbuf[256],gbuf[256];
  cin>>inbuf; // really read prompted data
  cin.getline(gbuf,256); //clear input buffer
  delete s1;
  s1=new strlnk(&inbuf[0]);
  s1->pdump(cout); //output the object
  cout<<"\nconcatenating !#334!";
  *s1+= "#344";
  s1->pdump(cout); //output the object

  cout<<"\n\n******************** new test ***************\n";
  cout<<"\nEnter a string for getline:";
  cin.getline(inbuf,256); // fill input buffer
  cout<<"\ngcount is: "<<cin.gcount();
  delete s1;
  s1=new strlnk(&inbuf[0]);
  s1->pdump(cout); //output the object
  cout<<"\nconcatenating !#334!";
  *s1+= "#344";
  s1->pdump(cout); //output the object

  cout<<"\n\n***1) testing getline for character - hit char, return:";
  cin.getline(inbuf,256); //clear input buffer
  
  cout<<"\n\n***2) testing getline for character - hit char, return:";
  cin.getline(inbuf,256); //clear input buffer
  
  cout<<"\n\n testing strlnk::inprej for input of --> at start of line";
  delete s1;
  s1=new strlnk("-->");
  s1->pdump(cout);
  cout<<"\n\ntype a string:";
  int goody=s1->inprej(cin);
  cout<<"\ngoodline: "<<goody;
  cout<<"\ngcount is: "<<cin.gcount();
  cin.getline(inbuf,256);
  cout<<"\ngcount is: "<<cin.gcount();
  cout<<"\nrest of line:"<<(char*)&inbuf[0];
  
  cout<<"\n\n***that's all!\nhit char, return to exit:";
  cin>>ch1;
  cout<<"\n*************************************************************\n";
  cout<<"*************************************************************\n";
  return 0;
}
