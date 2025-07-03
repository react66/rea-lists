*******************************************************************
*******************************************************************
Operation: gencla program generates class skeletons

USAGE:
  gencla <parm_file>
       where:
       <parm_file>.cld is a file containing:
       m merge_file_name
       i abstract_base_name file_name
       n clasnum_start_value
       c class_name
       d thing_type_one[*] thing_one 
       d thing_type_two[*] thing_two 
       ... and so on with c and d lines for class, data
     Operation:
        the i fields are optional but if there, the abstract_base_name
        overrides "vbc_link" and lists.h is replaced by file_name.h
        produce a file <parm_file>.h and <parm_file>.cc
        the starting clasnum just increments for each new class.
        The merge file is a file containing lines to add to h and 
        cc files, at specific classes.
        FORMAT consists of multiple i,h and cc sections in any order:
        i:
        <lines to add at include section at top of file
        /i:
        h:<class1>
        <lines to add to h file at class class1>
        /h:
        cc:<class1>
        <lines to add to cc file at class class1>
        /cc:
*******************************************************************
*******************************************************************
