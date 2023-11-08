#ifndef DISPLAY_INIT
#define DISPLAY_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "reader.h"

char* search_tag_name(U1 tag);
char* decode_string_utf8(cp_info *cp);
char* decode_name_index_and_name_type(cp_info *cp, U2 index, U1 tipo);
char* decode_access_flags(U2 flag);
char* arranging_flags(char* flags_reverse_order);
int decode_integer_info (cp_info * cp);
double decode_double_info (cp_info * cp);
uint64_t decode_long_info (cp_info * cp);
float decode_float_info (cp_info * cp);
int set_print_offset (int position, U1 offset);
void display_classfile (ClassFile *);

#endif
