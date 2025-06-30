/** Lists.cpp contains vbc_link, list and strlnk objects
**/
// REA 011100 fixed strlnk getline with max_size parm.
// REA 010703 fixed getline to use dyn char arrays
//            so it works with MS, cygwin, gnu.
#include "lists.h"

// object reader ******************

// assume list obj_reg is set up

vbc_link* read_obj(istream& rst)
{
  long tag1;
  rst>>tag1;
  if (tag1==0) return 0;//base class==null pointer
  // scan obj_reg list for match and create object
  obj_reg->work=obj_reg->head;
  while ((obj_reg->work!=0)&&(obj_reg->work->cnum()!=tag1))
     {obj_reg->work=obj_reg->work->n;}
  if (obj_reg->work!=0)  return obj_reg->work->newobj(rst);
  else {
   cerr<<" error! attempt to read an object with unknown tag\n";
   cerr<<" TAG:"<<tag1<<"\n";
   exit(-1);
   }
}

// ************************ vbc_link *****************************

   //poly write
   ostream& vbc_link::pwrite(ostream& wst)
  {wst<<" "<<cnum(); return wst;}

   //poly dump
   ostream& vbc_link::pdump(ostream& wst)
   {ofs+=1;
    indent(wst);
    wst<<"\nLINK cnum: "<<cnum();
    ofs-=1;
    return wst;}

   //poly read
   vbc_link* vbc_link::newobj(istream& rst)
   {
   vbc_link* i = new vbc_link();
   return i;
   }
   
   //indent
   ostream& vbc_link::indent(ostream& wst){
   wst<<"\n";
   for (int i=0;i<ofs;i++) wst<<"  ";
   return wst;
   }
   

// ************************ rlist *****************************

rlist::rlist(vbc_link*pnew,vbc_link*nnew):vbc_link(pnew,nnew){head=0;tail=0;work=0;count=0;}

rlist::rlist(const char* file_name):vbc_link(){
 head=0;tail=0;work=0;count=0;
 ifstream infile;
 infile.open(file_name,ios::in);
 if (!infile) {
  cout<<" input file open failed for file: "<<file_name<<"\n\n";
  infile.close();
  return;
  }
 while (!infile.eof()) {
  strlnk* s1 = new strlnk();
  if(s1->getline(infile))enq(s1,work); // returns 0 if eof
  else delete(s1);
 }
  infile.close();
}

rlist::rlist(const char* file_name, int max_size):vbc_link(){
 head=0;tail=0;work=0;count=0;
 ifstream infile;
 infile.open(file_name,ios::in);
 if (!infile) {
  cout<<" input file open failed for file: "<<file_name<<"\n\n";
  infile.close();
  return;
  }
 while (!infile.eof()) {
  strlnk* s1 = new strlnk();
  if(s1->getline(infile,max_size)) enq(s1,work);
  else delete(s1);
    }
  infile.close();
}


void rlist::sort(){                 //modified bubble sort
  vbc_link* bub=0,*bubpos=0;
  work=head;
  // work is current position; bub is current leaf
  while(work->n){
   bub=work->n;
    // if smaller pick it up & insert it
   if(bub->less_than(work)){
    bubpos=work;
    deq(bub);
    work=bubpos;
      // scan backwards
    while((work)&&(bub->less_than(work)))
      work=work->p;
      //insert
    enq(bub,work);
    work=bubpos;
      }
   else work=bub;
    }
  }



vbc_link* rlist::newobj(istream& rst)
{
rlist* il;
il=new rlist();
int lcnt;
rst>>(lcnt);
// now read in count objects on list
for (int i=1;i<=lcnt;i++){
  vbc_link* vb = read_obj(rst);
  il->enq(vb);
  }	
return il;
}

ostream& rlist::pwrite(ostream& wst)
{
wst<<" "<<cnum()<<" "<<count;
vbc_link* wsv=head;
while (wsv!=0){wsv->pwrite(wst);wsv=wsv->n;}
return wst;
}

rlist::~rlist()
{
while(tail!=0){
  work=tail->p;
  delete(tail);
  tail=work;
  }
}

ostream& rlist::pdump(ostream& wst)
{
  vbc_link::ofs+=1;
  vbc_link::indent(wst);
  wst<<"RLIST cnum: "<<cnum()<<" count: "<<count;
  vbc_link* wsv=head;
  while (wsv!=0){wsv->pdump(wst);wsv=wsv->n;}
  vbc_link::ofs-=1;
  return wst;
}

vbc_link* rlist::enq(vbc_link* lnew,vbc_link* after) // lnew gets set to 0
// if after==0, prepend the vbc_link
{
  if (lnew==0)return 0;
  if (after==0) {
    if(head)head->p=lnew;
    lnew->n=head;
    lnew->p=0;
    head=lnew;
    if (!(tail)) tail=lnew;
    }
  else {
    if(after->n)after->n->p=lnew;
    else tail=lnew;
    lnew->n=after->n;
    lnew->p=after; 
    after->n=lnew;
    }
  work=lnew;
  lnew=0;
  count+=1;
  return work;
}

vbc_link* rlist::enq(vbc_link* lnew) // append, lnew gets set to 0
{
  return rlist::enq(lnew,tail);
}

vbc_link* rlist::deq(vbc_link* lnew)
// returns work if lnew=0
// but if work is already 0, set it to head first.
{
  if (count==0) return 0;
  if (lnew==0){
    if (work==0) work=head;
    lnew=work;
    }
  if (lnew){
    if(tail==lnew){
      tail=lnew->p;
      work=tail;
      if(!(tail))head=tail;
      else tail->n=0;
      }
    else{
      if(head==lnew){
  head=lnew->n;
  head->p=0;
  work=head;
  }
      else{
  lnew->p->n=lnew->n;
  lnew->n->p=lnew->p;
  work=lnew->n;
  }
      }
    lnew->p=0;
    lnew->n=0;
    }
  count-=1;
  return lnew;
}

vbc_link* rlist::pop_tail()
// return tail, by calling deq
{
return deq(tail);
}

rlist* rlist::operator+(rlist* list1)
// append list1 to current list
{
  count+=list1->count;
  if (list1->count!=0)
    {
    if (tail!=0) tail->n=list1->head;
    tail=list1->tail;
    work=list1->head;
    if (head==0) head=list1->head;
    }
  list1=0;
  return this;
}

vbc_link* rlist::insert(rlist* list1, vbc_link* after)
// insert the list after the vbc_link, delete the list,
// returns list1->tail & sets work to that
// does nothing if list1 = 0 or list1 is empty.
{
  if (!(list1))return 0;
  if (list1->count==0){
    delete list1;
    list1=0;
    return 0;
    }
  count+=list1->count;
  work = list1->tail; 
  if (!(after)){
    work->n = head;
    if (head) head->p = work;
    head = list1->head;
    }
  else{
    list1->head->p = after;
    if (after->n) after->n->p = work;
    else tail=work; // fixup tail if at end
    work->n = after->n;
    after->n = list1->head;
    }
  list1->head = 0;
  list1->tail= 0;
  list1->work= 0;
  list1->count= 0;
  delete(list1); // rid of object safely...
  list1=0;
  return work; 
  }

long rlist::tail_cnum()
// return the cnum for the last object on the list
{
if (!(count)) return 0;
return tail->cnum();
}

// ************************* ralist *****************************

void ralist::fix_ptrs()
// re-do the pointer table
{
  delete[] ptr_table;
  ptr_table=0;
  if (count) {
    ptr_table = new vbc_link* [count];
    vbc_link* wsv=head;
    for (int i=1;i<=count;i++){
      ptr_table[i-1]=wsv;
      wsv=wsv->n;}}
}

ralist::~ralist()
{
  delete[] ptr_table;
}

vbc_link* ralist::newobj(istream& rst)
{
ralist* il = new ralist();
rst>>count;
// now read in count objects on list
for (int i=1;i<=count;i++)il->enq(read_obj(rst));
il->fix_ptrs();
return il;
}

ostream& ralist::pwrite(ostream& wst)
{
wst<<cnum()<<" "<<count;
vbc_link* wsv=head;
while (wsv!=0){wsv->pwrite(wst);wsv=wsv->n;}
return wst;
}

ostream& ralist::pdump(ostream& wst)
{
  vbc_link::ofs+=1;
  vbc_link::indent(wst);
  wst<<"ralist cnum: "<<cnum()<<" count: "<<count;
  vbc_link* wsv=head;
  while (wsv!=0){wsv->pdump(wst);wsv=wsv->n;}
  vbc_link::ofs-=1;
  return wst;
}

vbc_link* ralist::deq(vbc_link* lnew)
// returns work if lnew=0
{
  lnew=rlist::deq(lnew);
  fix_ptrs();
  return lnew;
}

vbc_link* ralist::deq(int ordpos)
// returns end one if ordpos too large
{
  if (count==0) return 0;
  int ordposi = ((ordpos<count)?ordpos:(count-1));
  vbc_link* lnew = rlist::deq(ptr_table[ordposi]);
  fix_ptrs();
  return lnew;
}

vbc_link* ralist::deq(int ordpos1, int ordpos2)
// returns null if ordpos1 out of range else calls deq(int)
{
  if (count==0) return 0;
  if (ordpos1>=count) return 0;
  ralist* ra2 = (ralist*)ptr_table[ordpos1];
  return (ra2->deq(ordpos2));
}

vbc_link* ralist::ref(int ordpos1, int ordpos2)
// returns null if ordpos1 out of range else ptr as in deq(int)
{
  if (count==0) return 0;
  if (ordpos1>=count) return 0;
  ralist* ra2 = (ralist*)ptr_table[ordpos1];
  if (ra2->count==0) return 0;
  int ordposi = ((ordpos2<ra2->count)?ordpos2:(ra2->count-1));
  return (ra2->ptr_table[ordposi]);
}

vbc_link* ralist::enq(vbc_link* lnew,vbc_link* after) // lnew gets set to 0
// if after==0, prepend the vbc_link
{
  vbc_link* ltmp;
  rlist::enq(lnew,after);
  ltmp=work;
  fix_ptrs();
  return work=ltmp;
}

vbc_link* ralist::enq(vbc_link* lnew) // append, lnew gets set to 0
{
  return ralist::enq(lnew,tail);
}

vbc_link* ralist::insert(rlist* list1,vbc_link* after)
{
 rlist::insert(list1,after);
 fix_ptrs();
 return work;
} 

ralist* ralist::operator+(ralist* list1)
// append list1 to current list
{
  rlist::operator+(list1);
  fix_ptrs();
  return this;
}

void ralist::sort(){                 //modified bubble sort
  rlist::sort();
  fix_ptrs();
    }

// ************************* strlnk *****************************

// implementation notes:
//     For a zero length string, str1 is null or array is '\0'
//        lists.h constructor has str1=0.


char* ccpy(const char* src, const int len) //copy len chars and append '\0'
{
   char* dest = new char[len+1];
   for(int i=0;i<len;i++)dest[i] = src[i];
   dest[len]='\0';
   return dest;
}

int slen(const char* src) // length not including '\0'
{
   int i=0;
   if (src) while (src[i] != '\0')i++;
   return i;
}
      
char* ccat(const char* src1,const char* src2) //concat and append '\0'
{
   int len=slen(src1)+slen(src2);
   char* dest = new char[len+1];
   int j=0;
   for(int i=0;i<slen(src1);i++){dest[j] = src1[i];j++;}
   for(int i=0;i<slen(src2);i++){dest[j] = src2[i];j++;}
   dest[len]='\0';
   return dest;
}
 
strlnk::strlnk(const char* newstr):vbc_link()         // copy string, incl \0
{
  strlnk_len=slen(newstr);
  str1=ccpy(newstr,strlnk_len);
}

strlnk::strlnk(strlnk& astr):vbc_link()         // copy strlnk, incl \0
{
  strlnk_len=astr.strlnk_len;
  str1=ccpy(astr.str1,strlnk_len);
}

strlnk::strlnk(istream& ist):vbc_link()        // read string from input stream
{
  char inb[4096];
  ist>>inb;
  strlnk_len=slen(inb);
  str1=ccpy(inb,strlnk_len);
}

strlnk::~strlnk()                    // 
{
  delete[] str1;
}


vbc_link* strlnk::newobj(istream& rst)   // poly input
{
  strlnk* i=new strlnk();
  rst>>i->strlnk_len;
  i->str1=new char[(i->strlnk_len)+1];
  //read one blank
  rst.get(i->str1[0]);
  for (int j=0;j<i->strlnk_len;j++){
   rst.get(i->str1[j]);
   }
  i->str1[i->strlnk_len]='\0';
  return i;
}

ostream& strlnk::pwrite(ostream& wst)  // poly output
{
  wst<<" "<<cnum()<<" "<<strlnk_len;
  if (strlnk_len>0) wst<<" ";
  for(int i=0; i<strlnk_len; wst<<str1[i++]);
  return wst;
}

ostream& strlnk::pdump(ostream& wst)
{
  vbc_link::ofs+=1;
  vbc_link::indent(wst);
  wst<<"strlnk cnum: "<<cnum()<<" length: "<<strlnk_len;
  vbc_link::indent(wst);
  wst<<" >"<<*this<<"<\n";
  vbc_link::ofs-=1;
  return wst;
}

   strlnk& strlnk::operator+=(const char* chs) // lengthens current string
{
  char* jj=ccat(str1,chs); // new string
  delete[] str1;
  str1=jj;
  strlnk_len=slen(str1);
  return *this;
}

   strlnk& strlnk::operator-=(const char* chs) // shortens current string
{
  int k=strpos(chs);
  int chs_len=slen(chs);
  int i,new_length;
  if (k>=0){
    new_length=strlnk_len-chs_len;
    char* jj=new char[new_length+1];
    jj[new_length]='\0';
    if (new_length==0) {
      strlnk_len=0;
      delete[] str1;
      str1=jj;
      return *this;
      }
    //first part of the line
    for (i=0; i<k; i++)jj[i]=str1[i];
    //last part of the line
    for (i=k; i<new_length; i++)jj[i]=str1[i+slen(chs)];
    delete[] str1;
    str1=jj;
    strlnk_len=new_length;
    }
  return *this;
}

// appends decimal number to string
strlnk& strlnk::operator+=(const int& ia)  
{
string s=to_string(ia);
*this+=s.c_str();
//char s[32];
//ostrstream xx(s,sizeof(s));
//xx<<ia<<ends;  // null terminated string in s
//*this+=(char*)&s;
return *this;
}

bool strlnk::operator==(const char* chs) // return 0 if not equal
{
  if (slen(chs) != strlnk_len) return 0;
  int isequal=1;
  for (int i=0;i<strlnk_len;i++)
    isequal &= (str1[i]==chs[i]);
  return isequal;
  }


ostream& operator<<(ostream& wst, strlnk& me)
{
  for(int i=0; i<me.strlnk_len; wst<<me.str1[i++]);
  return wst;
}

int strlnk::getline(istream& ist)
{
  if (ist.eof()) return 0; // no line.
  char dummy[lists_str_size]; // hope it's big enough
  ist.getline(dummy,lists_str_size,'\n');
    if (ist.gcount() < lists_str_size) {
      strlnk_len=ist.gcount()-1; // gcount includes \n
      if (strlnk_len < 0) return 0; // eof condition.
      }
    else {
      cerr<<"strlnk::getline(istream&) : ERROR: line longer than ";
      cerr<<lists_str_size<<" chars  \n";
      exit(1);
      }
  if (dummy[strlnk_len-1]=='\r'){
    dummy[strlnk_len-1]='\0';
    strlnk_len--;
  } 
  if (strlnk_len>0) delete[] str1;
  str1=ccpy((char*)&dummy,strlnk_len);
  return 1;  // return non 0 code
}

int strlnk::getline(istream& ist, const int max_size)
{
  if (ist.eof()) return 0; // no line.
  char* dummy = new char[max_size]; // hope it's big enough
  ist.getline(dummy,max_size,'\n');
  if (ist.gcount() < max_size) {
    strlnk_len=ist.gcount()-1; // gcount includes \n
    if (strlnk_len < 0) {
      delete [] dummy;
      return 0; // eof condition.
    }
  }
  else {
      cerr<<"strlnk::getline(istream&) : ERROR: line longer than ";
      cerr<<max_size<<" chars  \n";
      exit(1);
  }
  if (dummy[strlnk_len-1]=='\r'){
    dummy[strlnk_len-1]='\0';
    strlnk_len--;
  } 
  delete[] str1;
  str1=ccpy(dummy,strlnk_len);
  delete [] dummy;
  return strlnk_len;  // return non 0 code
}

int strlnk::inprej(istream& ist)
{  
  int kk=strlnk_len+1;
  int goodline=0,eof0=0,eof1=0;           // string buffer size
  char cmpc[256];
  char dummy[256];
  eof0=!(ist.get(cmpc,kk,'\n'));
  while ((!(goodline))&&(!(eof1))){
    if (ist.gcount()>=kk-1){
      goodline=1;
      for (int i=0; i<kk; i++) if (cmpc[i]!=str1[i]) goodline=0;   
      }
    if (!goodline){
      eof0=!(ist.getline(dummy,256,'\n'));    // purge line
      eof1=eof0;
      if (!eof0) {eof0=!(ist.get(cmpc,kk,'\n'));}
      }
    }
  return goodline;
}

int strlnk::inprej(istream& ist, strlnk& pname)
{  
  int kk=strlnk_len+1;
  int goodline=0,eof0=0,eof1=0;           // string buffer size
  char cmpc[256];
  char dummy[256];
  char pn1[256];
  eof0=!(ist.get(cmpc,kk,'\n'));
  while ((!(goodline))&&(!(eof1))){
    if (ist.gcount()>=kk-1){
      goodline=1;
      for (int i=0; i<kk; i++) if (cmpc[i]!=str1[i]) goodline=0;   
      }
    if (!goodline){
      ist>>pn1;
      if(pname.str1)delete[] pname.str1;
      pname.str1=ccat((char*)&cmpc,(char*)&pn1);
      pname.strlnk_len=slen(pname.str1); //fixup length
      eof0=!(ist.getline(dummy,256,'\n'));    // purge line
      eof1=eof0;
      if (!eof0) {eof0=!(ist.get(cmpc,kk,'\n'));}
      }
    }
  return goodline;
}

int strlnk::strpos(const char* chs)    
     // position index for substring; -ve if absent
{
  if (slen(chs) > strlnk_len) return -1;
  int mpos=0,chs_pos=0,fpos=0;
  while ((mpos<strlnk_len)&&(chs_pos<slen(chs))){
    if (str1[mpos]==chs[chs_pos])chs_pos++;
    else {
      mpos-=chs_pos; //start matching over
      chs_pos=0;
      }
    mpos++;
    }
  if (chs_pos==slen(chs)) return mpos-chs_pos; else return -1;  
}

int strlnk::strposa(const char* chs)    
     // position index after substring; -ve if absent
{
  if (slen(chs) > strlnk_len) return -1;
  int mpos=0,chs_pos=0;
  while ((mpos<strlnk_len)&&(chs_pos<slen(chs))){
    if (str1[mpos]==chs[chs_pos])chs_pos++;
    else {
      mpos-=chs_pos; //start matching over
      chs_pos=0;
      }
    mpos++;
    }
  if (chs_pos==slen(chs)) return mpos; else return -1;  
}

int strlnk::chrpos(const char* chs)    
     // position index at first char match; -ve if absent
{
  if ((slen(chs) == 0) || (strlnk_len==0)) return -1;
  int mpos=0,chs_pos=0,ch_match=0;
  while ((mpos<strlnk_len)&&!(ch_match)){
   ch_match=(str1[mpos]==chs[chs_pos]);
   if (!ch_match){
    if (chs_pos<slen(chs))chs_pos++;
    else {chs_pos=0;mpos++;}
    }
    }
  if(mpos==strlnk_len) mpos = -1;
  return mpos;
  }

int strlnk::to_int()       // integer value of string
{
  return atoi(str1);
}

double strlnk::to_double()
{
  if ((strlnk_len == 0) || (str1 == 0)) return 0.0;
  else return atof(str1);
}
  

int strlnk::lower_case()  // convert string to all lower case
{
for (int i=0;i<strlnk_len;i++)
  if ((str1[i]>='A') && (str1[i]<='Z'))
    str1[i]=(str1[i]-'A'+'a');
return 1;
}

int strlnk::upper_case()  // convert string to all upper case
{
for (int i=0;i<strlnk_len;i++)
  if ((str1[i]>='a') && (str1[i]<='z'))
    str1[i]=(str1[i]-'a'+'A');
return 1;
}

int strlnk::pad_bslash()  //insert '\' before \ or " or ': see gencc prog.
{
  int jj_ct=0;
  char* jj = new char[strlnk_len*2+1];
  for (int i=0;i<strlnk_len;i++){
    if ((str1[i]=='\\') ||
  (str1[i]=='\"') ||
  (str1[i]=='\'')) {
      jj[jj_ct++]='\\';
      }
    jj[jj_ct++]=str1[i];
    }
  jj[jj_ct]='\0';
  delete[] str1;
  str1=ccpy(jj,jj_ct);
  strlnk_len=jj_ct;
  delete[] jj;
  return strlnk_len;
  }
  
int strlnk::cut_range(int pos1,int pos2)
  // delete pos1 to pos2 inclusive return strlnk_len
  {
  if ((pos1 > pos2) || (pos2 >= strlnk_len) || (pos1 < 0))
   return strlnk_len;
  int new_len=strlnk_len-(pos2-pos1+1);
  char* jj = new char[new_len+1]; //'\0' at end.
  jj[new_len]='\0';
  if (pos1>0) for (int i=0; i<pos1; i++) jj[i]=str1[i];
  int jjpos=pos1; // now copy pos2+1 to strlnk_len-1
  if ((pos2+1) <= (strlnk_len-1))
   for (int i=(pos2+1);i<strlnk_len;jj[jjpos++]=str1[i]);
  delete[] str1;
  strlnk_len=new_len;
  str1=jj;
  return new_len;
  }

int strlnk::insert(strlnk& astr,int idx)  
   // insert string, return index just past string
{
  if (astr.strlnk_len==0) return strlnk_len;
  if (idx<0) return -1;
  int new_len = strlnk_len + astr.strlnk_len;
  char* jj = new char[new_len+1];
  int i=0;
  int j=0;
  for (j=0;j<idx;j++){jj[i]=str1[i]; i++;}
  for (j=0;j<astr.strlnk_len;j++){jj[i]=astr.str1[j];i++;}
  if (idx<strlnk_len) for(j=idx;j<strlnk_len;j++){jj[i]=str1[j]; i++;}
  jj[i]='\0';
  delete[] str1;
  str1=jj;
  strlnk_len=new_len;
  return (idx+astr.strlnk_len);
}

int strlnk::insert(const char* astr,int idx)  
   // insert string, return index just past string
{
  if (slen(astr)==0) return strlnk_len;
  if (idx<0) return -1;
  int new_len = strlnk_len + slen(astr);
  char* jj = new char[new_len+1];
  int i=0;
  int j=0;
  for (j=0;j<idx;j++){jj[i]=str1[i]; i++;}
  for (j=0;j<slen(astr);j++){jj[i]=astr[j];i++;}
  if (idx<strlnk_len) for(j=idx;j<strlnk_len;j++){jj[i]=str1[j]; i++;}
  jj[i]='\0';
  delete[] str1;
  str1=jj;
  strlnk_len=new_len;
  return (idx+slen(astr));
}

strlnk& strlnk::operator+=(strlnk& astr)  // append str1 from astr
{
int new_len = strlnk_len + astr.strlnk_len;
char* jj = ccat(str1,astr.str1);
delete[] str1;
str1=jj;
strlnk_len=new_len;
return *this;
}

strlnk& strlnk::operator-=(strlnk& astr)  // shortens current string
{
  int k=strpos(astr.str1);
  int chs_len=astr.strlnk_len;
  int i,new_length;
  if (k>=0){
    new_length=strlnk_len-chs_len;
    char* jj=new char[new_length+1];
    jj[new_length]='\0';
    if (new_length==0) {
      strlnk_len=0;
      delete[] str1;
      str1=jj;
      return *this;
      }
    //first part of the line
    for (i=0; i<k; i++)jj[i]=str1[i];
    //last part of the line
    for (i=k; i<new_length; i++)jj[i]=str1[i+chs_len];
    delete[] str1;
    str1=jj;
    strlnk_len=new_length;
    }
  return *this;
  }

strlnk& strlnk::operator+(strlnk& b) // add 2 strlnk, create new one
{
strlnk* c = new strlnk();
c->strlnk_len=strlnk_len + b.strlnk_len;
c->str1 = ccat(str1,b.str1);
return *c;
}

bool strlnk::operator==(const strlnk& xstr) // return 0 if not equal
{
  if (xstr.strlnk_len != strlnk_len) return 0;
  int isequal=1;
  for (int i=0;i<strlnk_len;i++)
    isequal &= (str1[i]==xstr.str1[i]);
  return isequal;
  }

strlnk* strlnk::extract(int i, int j)
// copy the substring and shorten the string by that much
{
  if (i<0) i=0;
  if ((strlnk_len==0) || (j<i)) return new strlnk();
  if (i>=strlnk_len) i=strlnk_len-1;
  if (j>=strlnk_len) j=strlnk_len-1;
  // create new strlnk with extracted data
  strlnk* ext = new strlnk();
  ext->strlnk_len = j-i+1; // string length from i through j
  ext->str1 = new char[ext->strlnk_len+1]; // \0 char
  ext->str1[ext->strlnk_len]='\0';   // nul termination
  for (int ii=i;ii<=j;ii++) ext->str1[ii-i] = str1[ii]; // copy data
  // done with ext
  // fixups
  int newstr1_len = strlnk_len - ext->strlnk_len;
  if (newstr1_len==0){
   delete str1;
   str1=0;
   strlnk_len=0;
   return ext;
   }
  // create a new string
  char* newstr1= new char[newstr1_len+1];
  newstr1[newstr1_len]='\0';
  // copy remaining data
  int k=0;
  // it will be 0 to i-1 and j+1 to strlnk_len-1
  // step through whole string and conditionally copy
  for (int ii=0; ii<strlnk_len; ii++) {
   if ((ii<i) || (ii>j)) newstr1[k++]=str1[ii];
   }
  delete(str1);
  str1=newstr1;
  strlnk_len=newstr1_len;
  return ext;
  }

 strlnk* strlnk::copy_range(const int& i,const int& j) // copy i-j inclusive
 {
  if ((i<0) || (j<i)) return 0;
  strlnk* ext=new strlnk();
  ext->strlnk_len=j-i+1;
  ext->str1=new char[ext->strlnk_len+1];
  ext->str1[ext->strlnk_len]='\0'; // nul terminated
  for (int jj=0;jj<ext->strlnk_len;jj++) 
     ext->str1[jj]=str1[i+jj]; //copy string
  return ext;
 }
 
 strlnk* strlnk::csv_field(int i)
// copy the substring to a new strlnk, field starts at 0
{
  if (i<0)return 0;
  int fpos=0;
  int icount=0;
  while(icount<i) {
    if (str1[fpos++]==',')icount++;
    if (fpos>strlnk_len) return 0;
    }
  // fpos positioned at start of string of interest
  int lpos=fpos;
  while((str1[lpos]!=',') && (str1[lpos]!='\0'))lpos++;
  if (lpos==fpos) return 0;
  lpos-=1; // do not include final inc.
  return copy_range(fpos,lpos);
  }
        
int strlnk::to_hex(const int& ival, const int& len)
  // convert first int to length in second int.
{
// create the string...
  if (str1) delete[] str1;
  str1 = new char[len + 1];
  strlnk_len=len;
  str1[len]='\0';
  int iival=ival;
  for (int i=len-1;i>=0;i--){
    int four_bits=iival & 0x0F;
    iival>>=4; // shift
    if (four_bits>9) four_bits= four_bits-10+(int)'A';
    else four_bits= four_bits + (int)'0';
    str1[i]=(char)four_bits;
  }
return ival;
}

int strlnk::hex_to_uns(const int& num_chars)
   // return integer; if something is wrong, return -1
{
  if (strlnk_len < num_chars) return -1;
  if (num_chars < 1 ) return -1;
  if (num_chars > 7 ) return -1;
  int rslt = 0;
  for (int i=0;i < num_chars; i++){
     rslt = rslt * 16;
     char xc = str1[i];
     if ((xc >= '0') & (xc <= '9')) rslt = rslt + (xc - '0');
     else if ((xc >= 'a') & (xc <='f')) rslt = rslt + (xc + 10 - 'a');
          else if ((xc >= 'A') & (xc <='F')) rslt = rslt + (xc + 10 - 'A');
	       else // erk
                 return -1;
  }
  return rslt;
} 


int strlnk::hex_to_uns(const int& init_char, const int& num_chars)
   // return integer; if something is wrong, return -1
{
  if (strlnk_len < (init_char + num_chars)) return -1;
  if (num_chars < 1 ) return -1;
  if (num_chars > 7 ) return -1;
  int rslt = 0;
  for (int i=init_char;i < (init_char+num_chars); i++){
     rslt = rslt * 16;
     char xc = str1[i];
     if ((xc >= '0') & (xc <= '9')) rslt = rslt + (xc - '0');
     else if ((xc >= 'a') & (xc <='f')) rslt = rslt + (xc + 10 - 'a');
          else if ((xc >= 'A') & (xc <='F')) rslt = rslt + (xc + 10 - 'A');
	       else // erk
                 return -1;
  }
  return rslt;
} 
     

int strlnk::bv_to_int()
  // return integer value of string taken as a bit vector
{
  int total=0;
  int adder=1;
  for (int i=strlnk_len-1;i>=0;i--)
  {
    if (!((str1[i]=='1') || (str1[i]=='0')))return 0;
    if (str1[i]=='1') total=total+adder;
    adder+=adder;
  }
  return total;
}

int strlnk::s(const char* target, const char* subst, const int& max_times)
{
  if (strlnk_len<slen(target)) return 0;
  int s_count=0;
  int tpos;
  strlnk* s2 = new strlnk();
  while(((tpos=(strpos(target)))>=0) && (s_count<max_times)){
    //copy beginning of str1 to str2, append subst to str2
    if (tpos>0) {
      strlnk* s3 = extract(0,tpos-1);
      *s2+=*s3; // accumulate
      delete(s3);
    }
    *this-=target;
    *s2+=subst;
    s_count++;
  }
  // put the string together
  *s2+=str1; // the rest of it.
  delete str1;
  str1=s2->str1;
  strlnk_len=s2->strlnk_len;
  s2->str1=0;
  delete(s2);
  return s_count;
}

// ************************* ofs static member in vbc_link *******************

  int vbc_link::ofs = 0;

  rlist* obj_reg = 0;
// ********************************* register function ************************

void lists_register() {
  if(!(obj_reg)) obj_reg=new rlist(); // in case we ever want different order
  obj_reg->enq(new vbc_link(),obj_reg->work);
  obj_reg->enq(new rlist(),obj_reg->work);
  obj_reg->enq(new ralist(),obj_reg->work);
  obj_reg->enq(new strlnk(),obj_reg->work);
  }
