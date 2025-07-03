// gencla.cc program
// REA 100996
// generate the .h and .cc files for a class
//
 
#include "lists.h"

class cldata: public vbc_link
{
public:
strlnk* cl_name;
strlnk* d_name;
strlnk* d_name_unqual;
int is_a_ptr,is_array,array_size;
cldata():vbc_link(){cl_name=0;d_name=0;is_a_ptr=0;is_array=0;array_size=0;}
~cldata(){delete cl_name;delete d_name;}
};
 
class cldef: public vbc_link
{
public:
strlnk* name;
rlist* cldata_list;
cldef():vbc_link(){name=0;cldata_list=0;}
~cldef(){delete name;delete cldata_list;}
};

class idef: public vbc_link
{
public:
strlnk* clas_name;
strlnk* file_name;
idef():vbc_link(){clas_name = new strlnk("vbc_link"); file_name = new strlnk("lists");}
~idef(){delete clas_name; delete file_name;}
};

class merge: public vbc_link
{
public:
strlnk* clas_name;
rlist* info;
merge():vbc_link(){clas_name=0; info=0;}
~merge(){delete clas_name; delete info;}
};
 
int main(int argc, char* argv[])
{
lists_register();
ifstream infile;
ofstream ofile_h, ofile_cc;
 
// make some strlnks to hold parms
strlnk* parm[10];
for (int i=0;i<argc-1;i++)parm[i] = new strlnk(argv[i+1]);
if (argc!=2) {
  cout<<"*******************************************************************\n";
  cout<<"*******************************************************************\n";
  cout<<"Operation: gencla program generates class skeletons\n\n";
  cout<<"USAGE:\n";
  cout<<"  gencla <parm_file>\n";
  cout<<"       where:\n";
  cout<<"       <parm_file>.cld is a file containing:\n";
  cout<<"       m merge_file_name\n";
  cout<<"       i abstract_base_name file_name\n";
  cout<<"       n clasnum_start_value\n";
  cout<<"       c class_name\n";
  cout<<"       d thing_type_one[*] thing_one \n";
  cout<<"       d thing_type_two[*] thing_two \n";
  cout<<"       ... and so on with c and d lines for class, data\n";
  cout<<"     Operation:\n";
  cout<<"        the i fields are optional but if there, the abstract_base_name\n";
  cout<<"        overrides \"vbc_link\" and lists.h is replaced by file_name.h\n";
  cout<<"        produce a file <parm_file>.h and <parm_file>.cc\n";
  cout<<"        the starting clasnum just increments for each new class.\n";
  cout<<"        The merge file is a file containing lines to add to h and \n";
  cout<<"        cc files, at specific classes.\n";
  cout<<"        FORMAT consists of multiple i,h and cc sections in any order:\n";
  cout<<"        i:\n";
  cout<<"        <lines to add at include section at top of file\n";
  cout<<"        /i:\n";
  cout<<"        h:<class1>\n";
  cout<<"        <lines to add to h file at class class1>\n";
  cout<<"        /h:\n";
  cout<<"        cc:<class1>\n";
  cout<<"        <lines to add to cc file at class class1>\n";
  cout<<"        /cc:\n";
  cout<<"*******************************************************************\n";
  cout<<"*******************************************************************\n";
  return(1);
}
strlnk* input_file_name= new strlnk(parm[0]->str1);
*input_file_name+=".cld";

// ************************* READING .cld FILE ****************************
// ************************************************************************
rlist* cl_list = new rlist(); // read the data in properly
strlnk* stmp;
cldef* cl1 = 0;
cldata* cld1;
strlnk* clnum = 0;
rlist* merge_i = new rlist();
rlist* merge_h = new rlist();
rlist* merge_cc = new rlist();
idef idef1;   // i field for different include and base class
infile.open(input_file_name->str1,ios::in);
if (!(infile)) {
  cerr<<"gencla: ERROR no input file: "<<*input_file_name<<"\n";
  exit(1);
  }
stmp = new strlnk(infile);
while (!(infile.eof())){
  if (stmp->strpos("m")>=0){
    // read the merge file
    delete stmp;
    stmp = new strlnk(infile);
    ifstream infile1;
    infile1.open(stmp->str1,ios::in);
    cout<<"gencla: reading merge file: "<<*stmp<<"\n";
    if (!(infile1)) {
       cerr<<"gencla: ERROR no input file: "<<*stmp<<"\n";
       exit(1);
       }
    delete stmp;
    stmp = new strlnk(infile1);
    while (!(infile1.eof())){ 
      if ((stmp->strpos("i:")) == 0) {
        delete(stmp);
        stmp = new strlnk();
        stmp->getline(infile1);
        while ((stmp->strpos("/i:")) != 0){
          merge_i->enq(stmp);
        stmp = new strlnk();
        stmp->getline(infile1);
          }
        delete stmp;
        }
      if ((stmp->strpos("h:")) == 0) {
        *stmp-="h:";
        merge* mr = new merge();
        mr->clas_name = stmp;
        mr->info = new rlist();
        stmp = new strlnk();
        stmp->getline(infile1);
        while ((stmp->strpos("/h:")) != 0){
          mr->info->enq(stmp);
        stmp = new strlnk();
        stmp->getline(infile1);
          }
        delete stmp;
        merge_h->enq(mr);
        }
      if ((stmp->strpos("cc:")) == 0) {
        *stmp-="cc:";
        merge* mr = new merge();
        mr->clas_name = stmp;
        mr->info = new rlist();
        stmp = new strlnk();
        stmp->getline(infile1);
        while ((stmp->strpos("/cc:")) != 0){
          mr->info->enq(stmp);
        stmp = new strlnk();
        stmp->getline(infile1);
          }
        delete stmp;
        merge_cc->enq(mr);
        }
      if (!(infile1.eof())) stmp = new strlnk(infile1);
      } // while (!(infile1.eof())) 
    cout<<"gencla: END of merge file\n";
    stmp = new strlnk("m");
    } // stmp = m
  if (stmp->strpos("i")>=0){
    if (!(idef1.clas_name->strpos("vbc_link")>=0))
    { cout<<"gencla: ERROR, multiple i lines in cld file \n";
      exit(1);
    }
    delete idef1.clas_name ;
    delete idef1.file_name ;
    idef1.clas_name = new strlnk(infile);
    idef1.file_name = new strlnk(infile);
    cout<<"\ngencla: defined base class: "<<*(idef1.clas_name)<<"\n";
    cout<<"gencla:           from file: "<<*(idef1.file_name)<<"\n";
    }
  if (stmp->strpos("c")>=0){
    if (cl1) cl_list->enq(cl1);
    delete stmp;
    stmp = new strlnk(infile);
    cl1 = new cldef();
    cl1->name = stmp;
    stmp = new strlnk("c");
    }
  if (stmp->strpos("d")>=0){
    if (!cl1){
      cl1=new cldef();
      cl1->name = new strlnk("NO_NAME_GIVEN");
      }
    delete stmp;
    cld1 = new cldata();
    stmp = new strlnk(infile); // cl_name
    if ((stmp->strpos("*"))!=-1) {
      cld1->is_a_ptr = 1;
      *stmp-="*";
      }
    cld1->cl_name=stmp;
    cld1->d_name = new strlnk(infile);  //d_name
    int kk=(cld1->d_name)->strpos("[");
    if (kk >=0){ // is array
      cld1->is_array=1;
      stmp=new strlnk(cld1->d_name->str1);
      cld1->d_name_unqual=stmp->extract(0,kk-1);
      // get the array size
      *stmp-="[";
      *stmp-="]";
      cld1->array_size=stmp->to_int();
      delete(stmp);}
     else cld1->d_name_unqual= new strlnk(cld1->d_name->str1);
    if (!(cl1->cldata_list))cl1->cldata_list=new rlist();
    cl1->cldata_list->enq(cld1);
    stmp = new strlnk("d");
    }
  if (stmp->strpos("n")>=0){
    if(clnum) {
       cerr<<"gencla: ERROR: cannot redefine clasnum\n";
       return(1);
       }
    clnum= new strlnk(infile);
    }
    delete stmp;
    stmp = new strlnk(infile);
  }
if (cl1) cl_list->enq(cl1);
cout<< "\ngencla: number of classes defined: "<<cl_list->count<<"\n";

strlnk* ofile_h_name;
strlnk* ofile_cc_name;

ofile_h_name= new strlnk(parm[0]->str1);
ofile_cc_name= new strlnk(parm[0]->str1);
*ofile_h_name+=".h";
*ofile_cc_name+=".cc";

// *************************** H FILE ***************************
// *************************************************************
ofile_h.open((ofile_h_name->str1),ios::out); // keep open until end.

ofile_h<<"\n// "<<*ofile_h_name<<"\n";
ofile_h<<"\n";

ofile_h<<"#ifndef "<<*parm[0]<<"_h "<<'\n';
ofile_h<<"#define "<<*parm[0]<<"_h"<<'\n';
ofile_h<<"\n#include \""<<*(idef1.file_name)<<".h\"\n";

// include section from merge file:
merge_i->work = merge_i->head;
while(merge_i->work){
  ofile_h<<*((strlnk*)(merge_i->work))<<"\n";
  merge_i->work = merge_i->work->n;
  }
ofile_h<<"\n";
  
// create defines iff clnum is defined
if (clnum) {
  cl1 = (cldef*)(cl_list->head);
  int int1 = clnum->to_int();
  while (cl1){
    ofile_h<<"const int cnum_"<<*(cl1->name)<<" = "<<int1++<<";\n"; 
    cl1=(cldef*)(cl1->n); 
    }
  }
// create h part, include all classes
cl1 = (cldef*)(cl_list->head);
while (cl1){
  ofile_h<<"\n\ntypedef class "<<*(cl1->name)<<" *P"<<*(cl1->name)<<";\n";
  ofile_h<<"class "<<*(cl1->name)<<":public "<<*(idef1.clas_name)<<"\n";
  ofile_h<<"{"<<'\n';
  ofile_h<<"public:"<<'\n';
  if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
  else cld1=0;
  while (cld1){
    ofile_h<<*(cld1->cl_name)<<" ";
    if (cld1->is_a_ptr) ofile_h<<"*";
    ofile_h<<*(cld1->d_name)<<";\n";
    cld1=(cldata*)(cld1->n);
    }
  ofile_h<<"long cnum(){return cnum_"<<*(cl1->name)<<" ;}\n";
  ofile_h<<*(cl1->name)<<"():"<<*(idef1.clas_name)<<"()";
  ofile_h<<"{";
  if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
  else cld1=0;
  while (cld1){
    if (cld1->is_a_ptr){
      ofile_h<<"\n"<<*(cld1->d_name)<<"=0;";
      }
    cld1=(cldata*)(cld1->n);
    }
  ofile_h<<"}"<<'\n';
  ofile_h<<"~"<<*(cl1->name)<<"();\n";
  ofile_h<<"vbc_link* newobj(istream&);"<<'\n';
  ofile_h<<"ostream& pwrite(ostream&);"<<'\n';
  ofile_h<<"ostream& pdump(ostream&);"<<'\n';
  // now check for merge  records
  vbc_link* mrpos = merge_h->head;
  while (mrpos) {
    merge* mr = (merge*)mrpos;
    if (*(mr->clas_name)==*(cl1->name)){
      strlnk* s1 = (strlnk*)(mr->info->head);
      while (s1){
        ofile_h<<*s1<<'\n';
        s1 = (strlnk*)(s1->n);
        }
      }
    mrpos = mrpos->n;
    }
  ofile_h<<"};\n\n";
  cl1=(cldef*)(cl1->n);
  }

//create register for classes
ofile_h<<"void "<<*parm[0]<<"_register();\n\n";

ofile_h<<"#endif\n";
ofile_h.close();

// ********************* CC FILE ***********************************
// ****************************************************************

ofile_cc.open((ofile_cc_name->str1),ios::out); // keep open until end.

ofile_cc<<"\n// "<<*ofile_cc_name<<"\n";
ofile_cc<<"\n";
ofile_cc<<"#include \""<<*ofile_h_name<<"\"\n\n\n";

cl1 = (cldef*)(cl_list->head);
while (cl1){

ofile_cc<<"\n//    START OF CLASS: "<<*(cl1->name)<<"     *******************\n\n";

 // newobj
   ofile_cc<<"vbc_link* "<<*(cl1->name)<<"::newobj(istream& rst)\n{\n";
   ofile_cc<<*(cl1->name)<<"* il = new "<<*(cl1->name)<<"();\n";
   if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
   else cld1=0;
   while (cld1){
      if (cld1->is_a_ptr){
        ofile_cc<<"il->"<<*(cld1->d_name);
        ofile_cc<<"= ("<<*(cld1->cl_name)<<"*)read_obj(rst);\n";
        }
      else 
        {
        if (cld1->is_array){
         ofile_cc<<"int j;\n";
         ofile_cc<<"for (int i=0; i<"<<cld1->array_size<<"; i++){"<<"\n";
         ofile_cc<<"  rst>>hex>>j; // assume int compatible\n";
         ofile_cc<<"  il->"<<*(cld1->d_name_unqual)<<"[i]=("<<*(cld1->cl_name);
         ofile_cc<<")j;\n";
         ofile_cc<<"}\n";
         }
        else
          {
          if (cld1->d_name_unqual)
          ofile_cc<<"rst>>"<<"il->"<<*(cld1->d_name_unqual)<<";\n";
          else
          ofile_cc<<"rst>>"<<"il->"<<*(cld1->d_name)<<";\n";
          }
        }
      cld1=(cldata*)(cld1->n);
      }
   ofile_cc<<"return il;\n}\n\n\n";


 // pwrite
   ofile_cc<<"ostream& "<<*(cl1->name)<<"::pwrite(ostream& wst)"<<"\n"<<"{\n";
   ofile_cc<<"wst<<\" \"<<cnum();\n";
   if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
   else cld1=0;
   while (cld1){
      if (cld1->is_a_ptr){
        ofile_cc<<"if ("<<*(cld1->d_name)<<")";
        ofile_cc<<*(cld1->d_name)<<"->pwrite(wst);\n";
        ofile_cc<<"else wst<<\" 0\";\n";
        }
      else
        if (cld1->is_array){
          ofile_cc<<"wst<<hex<<\" \";"<<"\n"; // need to iterate
          ofile_cc<<"int lcount=0;"<<"\n";
          ofile_cc<<"for (int i=0; i<"<<cld1->array_size<<"; i++){"<<"\n";
          ofile_cc<<"  wst<<"<<*(cld1->d_name_unqual)<<"[i]<<\" \";"<<"\n";
          ofile_cc<<"  lcount++;"<<"\n";
          ofile_cc<<"  if (lcount==16){lcount=0;wst<<\" \\\\\\n\";};"<<"\n";
          ofile_cc<<"}"<<"\n";
        }
        else ofile_cc<<"wst<<\" \"<<"<<*(cld1->d_name_unqual)<<";\n";
      cld1=(cldata*)(cld1->n);
      }
   ofile_cc<<"return wst;"<<"\n}\n\n";

 // pdump
   ofile_cc<<"ostream& "<<*(cl1->name)<<"::pdump(ostream& wst)"<<"\n"<<"{\n";
   ofile_cc<<"vbc_link::ofs+=1;\n";
   ofile_cc<<"vbc_link::indent(wst);\n";
   ofile_cc<<"wst<<\""<<*(cl1->name)<<" cnum= \"<<cnum()<<\"\\n\";\n";
   if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
   else cld1=0;
   while (cld1){
      ofile_cc<<"vbc_link::indent(wst);\n";
      if (cld1->is_a_ptr){
        ofile_cc<<"wst<<\""<<*(cld1->d_name)<<":\";\n";
        ofile_cc<<"if ("<<*(cld1->d_name)<<")";
        ofile_cc<<*(cld1->d_name)<<"->pdump(wst);\n";
        ofile_cc<<"else wst<<\" 0\";\n";
        ofile_cc<<"wst<<\"\\n\";\n";
        }
      else 
        ofile_cc<<"wst<<\""<<*(cld1->d_name_unqual)<<":\";\nwst<<"<<*(cld1->d_name_unqual)<<"<<\"\\n\";\n";
      cld1=(cldata*)(cld1->n);
      }
   ofile_cc<<"vbc_link::ofs-=1;"<<"\n";
   ofile_cc<<"return wst;"<<"\n}\n\n";
// ~
   ofile_cc<<*(cl1->name)<<"::~"<<*(cl1->name)<<"()\n"<<"{\n";
   if (cl1->cldata_list) cld1= (cldata*)(cl1->cldata_list->head);
   else cld1=0;
   while (cld1){
      if (cld1->is_a_ptr)
        ofile_cc<<"if ("<<*(cld1->d_name)<<") delete("<<*(cld1->d_name)<<");\n";
      cld1=(cldata*)(cld1->n);
      }
   ofile_cc<<"}\n\n";

  // now check for merge  records
  vbc_link* mrpos = merge_cc->head;
  while (mrpos) {
    merge* mr = (merge*)mrpos;
    if (*(mr->clas_name)==*(cl1->name)){
      strlnk* s1 = (strlnk*)(mr->info->head);
      while (s1){
        ofile_cc<<*s1<<"\n";
        s1 = (strlnk*)(s1->n);
        }
      }
    ofile_cc<<"\n\n";
    mrpos = mrpos->n;
    }

ofile_cc<<"\n//    END OF CLASS: "<<*(cl1->name)<<"     *******************\n\n";

   cl1= (cldef*)(cl1->n);
   }

// register

ofile_cc<<"void "<<*parm[0]<<"_register()\n{\n";
ofile_cc<<"if (!(obj_reg)) lists_register();\n";
cl1 = (cldef*)(cl_list->head);
while (cl1) {
  ofile_cc<<"obj_reg->enq(new "<<*(cl1->name)<<"(),obj_reg->work);\n";
  cl1= (cldef*)(cl1->n);
  }
ofile_cc<<"}\n\n";

ofile_cc.close();
return 0;
}

