#ifndef READER_INIT
#define READER_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "display.h"

ClassFile* read_file (ClassFile * , char *);
cp_info* read_constant_pool (FILE *fp, cp_info *cp, U2 cp_count);
U2* read_interfaces (FILE *fp, U2* interface, U2 i_count);
field_info* read_fields (FILE *fp, field_info *field, U2 f_count, cp_info *cp);
attribute_info* read_attributes (FILE *fp, attribute_info* a, cp_info *cp);
method_info* read_methods (FILE *fp, method_info* metodo, U2 m_count, cp_info *cp);
code_attribute* read_code (FILE * fp, cp_info *cp);
line_number_table* read_line_number_table(FILE * fp, cp_info * cp);
exception_info* read_exception_table (FILE * fp, U2 size) ;
exception_attribute* read_exceptions_attribute (FILE *fp);
constant_value_attribute* read_constant_value (FILE *fp);
signature_attribute* read_signature (FILE *fp);
inner_classes_attribute* read_inner_classes (FILE *fp, cp_info *cp);
classes* read_classes (FILE *fp);
stack_map_attribute* read_stack_map_table (FILE *fp);
stack_map_frame* read_stack_map_frame (FILE *fp);
verification_type_info* read_verification_type_info (FILE *fp);
source_file_attribute* read_source_file (FILE  *fp);

#endif
