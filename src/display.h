#ifndef DISPLAY_INIT
#define DISPLAY_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "reader.h"

char* search_tag_name(u1 tag);
char* decode_string_utf8(cp_info *cp);
char* decode_name_index_and_name_type(cp_info *cp, u2 index, u1 tipo);
double decode_double_info (cp_info * cp);
uint64_t decode_long_info (cp_info * cp);
void display_classfile (ClassFile *);

#endif
