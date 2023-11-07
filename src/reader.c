#include "reader.h"
#include <string.h>

// Function to read u1 type from .class
uint8_t u1READ(FILE *arq) {
  return getc(arq);
}

// Function to read u2 type from .class
uint16_t u2READ(FILE *arq) {
  uint16_t value = getc(arq);
  value = (value << 8) | getc(arq);
  return value;
}

// Function to read u4 type from .class
uint32_t u4READ(FILE *arq) {
  uint32_t value = getc(arq);
  value = (value << 8) | getc(arq);
  value = (value << 8) | getc(arq);
  value = (value << 8) | getc(arq);
  return value;
}

ClassFile* read_file (ClassFile* cf, char *filename)
{
  FILE *fp = fopen(filename, "rb");

  if(!fp){
    printf("Error opening file...\n");
    exit(0);
  }

  cf->magic = u4READ(fp);
  cf->minor_version = u2READ(fp);
  cf->major_version = u2READ(fp);

  cf->constant_pool_count = u2READ(fp);
  cf->constant_pool = read_constant_pool(fp, cf->constant_pool, cf->constant_pool_count);

  cf->access_flags = u2READ(fp);
  cf->this_class = u2READ(fp);
  cf->super_class = u2READ(fp);

  cf->interfaces_count = u2READ(fp);
  cf->interfaces = (cf->interfaces_count > 0) ?
                    read_interfaces(fp, cf->interfaces, cf->interfaces_count) : NULL;

  fclose(fp);

  return cf;
}

cp_info* read_constant_pool (FILE *fp, cp_info *cp, u2 cp_count)
{
  cp = (cp_info *) malloc(cp_count * sizeof(cp_info));

  for(cp_info *p = cp; p < cp + cp_count - 1; p++){
    p->tag = u1READ(fp);

    switch (p->tag) {
      case CONSTANT_Class:
				p->cp_union.Class.name = u2READ(fp);
				break;

			case CONSTANT_Field:
				p->cp_union.Field.class_name = u2READ(fp);
				p->cp_union.Field.name_and_type = u2READ(fp);
				break;

			case CONSTANT_Method:
				p->cp_union.Method.class_name = u2READ(fp);
				p->cp_union.Method.name_and_type = u2READ(fp);
				break;

			case CONSTANT_InterfaceMethod:
				p->cp_union.InterfaceMethod.class_name = u2READ(fp);
				p->cp_union.InterfaceMethod.name_and_type = u2READ(fp);
				break;

			case CONSTANT_String:
				p->cp_union.String.string = u2READ(fp);
				break;

			case CONSTANT_Integer:
				p->cp_union.Integer.bytes = u4READ(fp);
				break;

			case CONSTANT_Float:
				p->cp_union.Float.bytes = u4READ(fp);
				break;

			case CONSTANT_Long:
				p->cp_union.Long.hi_bytes = u4READ(fp);
				p->cp_union.Long.lo_bytes = u4READ(fp);
				p++;
				break;

			case CONSTANT_Double:
				p->cp_union.Double.hi_bytes = u4READ(fp);
				p->cp_union.Double.lo_bytes = u4READ(fp);
				p++;
				break;

			case CONSTANT_NameAndType:
				p->cp_union.NameAndType.name = u2READ(fp);
				p->cp_union.NameAndType.descriptor = u2READ(fp);
				break;

			case CONSTANT_Utf8:
				p->cp_union.Utf8.length = u2READ(fp);
				p->cp_union.Utf8.bytes = malloc(p->cp_union.Utf8.length*sizeof(u1));
				for (u1 *i=p->cp_union.Utf8.bytes;i<p->cp_union.Utf8.bytes+p->cp_union.Utf8.length;i++){
					*i = u1READ(fp);
				}
				break;

			case CONSTANT_MethodHandle:
				p->cp_union.MethodHandle.reference_kind = u1READ(fp);
				p->cp_union.MethodHandle.reference = u2READ(fp);
				break;

			case CONSTANT_MethodType:
				p->cp_union.MethodType.descriptor = u2READ(fp);
				break;

			case CONSTANT_InvokeDynamic:
				p->cp_union.InvokeDynamic.bootstrap_method_attr = u2READ(fp);
				p->cp_union.InvokeDynamic.name_and_type = u2READ(fp);
				break;

			default:
				break;
    }
  }

  return cp;
}

u2* read_interfaces (FILE *fp, u2* interface, u2 i_count)
{
	interface = (u2 *) malloc(i_count * sizeof(u2));
	for (u2 *p = interface; p < interface + i_count; p++){
		*interface = u2READ(fp);
	}

	return interface;
}