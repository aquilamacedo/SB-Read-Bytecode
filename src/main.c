#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ClassFile.h"
#include "reader.h"
#include "display.h"

int main(int argc, char *argv[]) {
	ClassFile *class_file;
	char *filename;

	if( argc == 2 ) {
		filename = (char *) malloc(strlen(argv[1]) * sizeof(char));
		strcpy(filename, argv[1]);
  } else printf("One argument expected!\n");

	class_file = (ClassFile *) malloc(sizeof(ClassFile));

	class_file = read_file(class_file, filename);

  display_classfile(class_file);

  return 0;
}