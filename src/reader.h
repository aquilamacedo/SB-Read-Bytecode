#ifndef READER_INIT
#define READER_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "display.h"

ClassFile* read_file (ClassFile * , char *);
cp_info* read_constant_pool (FILE *fp, cp_info *cp, u2 cp_count);
u2* read_interfaces (FILE *fp, u2* interface, u2 i_count);

#endif
