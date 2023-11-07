#include <string.h>
#include <math.h>
#include "display.h"
#include "reader.h"

void display_classfile (ClassFile * arquivoClass)
{
  cp_info * aux;
	double valor;
	uint64_t longValue;
	method_info * auxMethod;
	field_info * auxField;
	attribute_info ** auxAttributeClasse;
	attribute_info ** fieldAttrAux;
	exception_info * exceptionTableAux;
	uint32_t contador = 1;
	char *ponteiroprint;

	printf("\n╔═════════════════════ CLASS INFO ═════════════════════╗\n");
	printf("║  Magic:               0x%08x                     ║\n", arquivoClass->magic);
	printf("║  Minor Version:       0x%04x                         ║\n", arquivoClass->minor_version);
	printf("║  Major Version:       0x%04x                         ║\n", arquivoClass->major_version);
	printf("║  Constant Pool Count: 0x%04x                         ║\n", arquivoClass->constant_pool_count);
	printf("║  Access Flags:        0x%04x                         ║\n", arquivoClass->access_flags);
	printf("║  This Class:          0x%04x                         ║\n", arquivoClass->this_class);
	printf("║  Super Class:         0x%04x                         ║\n", arquivoClass->super_class);
	printf("║  Interfaces Count:    0x%04x                         ║\n", arquivoClass->interfaces_count);
	printf("║  Fields Count:        0x%04x                         ║\n", arquivoClass->fields_count);
	printf("║  Methods Count:       0x%04x                         ║\n", arquivoClass->methods_count);
	printf("║  Attributes Count:    0x%02x                           ║\n", arquivoClass->attributes_count);
	printf("╚══════════════════════════════════════════════════════╝\n\n");


	printf("════════════════════ CONSTANT POOL ═════════════════════\n");

	for (aux = arquivoClass->constant_pool; aux < arquivoClass->constant_pool + arquivoClass->constant_pool_count - 1; aux++) {
		printf("\n═════════════════════ INDEX [%d] ════════════════════════\n\n", contador);
		contador++;
		printf("TAG: %02d: %s\n", aux->tag, search_tag_name(aux->tag));

		if (aux->tag == CONSTANT_Class) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.Class.name, CLASS_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", aux->cp_union.Class.name, ponteiroprint);
		} else if (aux->tag == CONSTANT_Float) {
			printf("Bytes: %04x\n", aux->cp_union.Float.bytes);
			printf("Float: %d\n", aux->cp_union.Float.bytes);
		} else if (aux->tag == CONSTANT_Field) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.Field.class_name, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", aux->cp_union.Field.class_name, ponteiroprint);
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.Field.name_and_type, NAME_AND_TYPE);
			printf("Name and Type: cp_info#%02d <%s>\n", aux->cp_union.Field.name_and_type, ponteiroprint);
		} else if (aux->tag == CONSTANT_InterfaceMethod) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.String.string, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", aux->cp_union.InterfaceMethod.class_name, ponteiroprint);
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.String.string, NAME_AND_TYPE);
			printf("Name and Type Index: cp_info#%02d <%s>\n", aux->cp_union.InterfaceMethod.name_and_type, ponteiroprint);
		} else if (aux->tag == CONSTANT_String) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.String.string, STRING_INDEX);
			printf("String: cp_info#%02d <%s>\n", aux->cp_union.String.string, ponteiroprint);
		} else if (aux->tag == CONSTANT_Integer) {
			printf("Bytes: %04x\n", aux->cp_union.Integer.bytes);
			printf("Integer: %d\n", aux->cp_union.Integer.bytes);
		} else if (aux->tag == CONSTANT_Long) {
			longValue = decode_long_info(aux);
			printf("Long High Bytes: 0x%08x\n", aux->cp_union.Long.hi_bytes);
			printf("Long Low Bytes: 0x%08x\n", aux->cp_union.Long.lo_bytes);
			printf("Long: %lu\n", longValue);
		} else if (aux->tag == CONSTANT_NameAndType) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.NameAndType.name, NAME_AND_TYPE_INFO_NAME_INDEX);
			printf("Name: cp_info#%02d <%s>\n", aux->cp_union.NameAndType.name, ponteiroprint);
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.NameAndType.descriptor, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
			printf("Descriptor: cp_info#%02d <%s>\n", aux->cp_union.NameAndType.descriptor, ponteiroprint);
		} else if (aux->tag == CONSTANT_Utf8) {
			printf("Length of byte array: %d\n", (int) aux->cp_union.Utf8.length);
			printf("Length of string: %d\n", (int) aux->cp_union.Utf8.length);
			printf("String: ");
			for (u1 *i = aux->cp_union.Utf8.bytes; i < aux->cp_union.Utf8.bytes + aux->cp_union.Utf8.length; i++) {
					printf("%c", (char) (*i));
			}
			printf("\n");
		} else if (aux->tag == CONSTANT_MethodHandle) {
			printf("MethodHandle Reference Kind: %02x\n", aux->cp_union.MethodHandle.reference_kind);
			printf("MethodHandle Reference Index: %04x\n", aux->cp_union.MethodHandle.reference);
		} else if (aux->tag == CONSTANT_Method) {
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.Method.class_name, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", aux->cp_union.Method.class_name, ponteiroprint);
			ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool, aux->cp_union.Method.name_and_type, NAME_AND_TYPE);
			printf("Name and Type: cp_info#%02d <%s>\n", aux->cp_union.Method.name_and_type, ponteiroprint);
		} else if (aux->tag == CONSTANT_Double) {
			valor = decode_double_info(aux);
			printf("Double High Bytes: 0x%08x\n", aux->cp_union.Double.hi_bytes);
			printf("Double Low Bytes: 0x%08x\n", aux->cp_union.Double.lo_bytes);
			printf("Double: %lf\n", valor);
		} else if (aux->tag == CONSTANT_MethodType) {
			printf("MethodType Descriptor Index: %04x\n", aux->cp_union.MethodType.descriptor);
		} else if (aux->tag == CONSTANT_InvokeDynamic) {
			printf("InvokeDynamic - Bootstrap Method Attr Index: %04x\n", aux->cp_union.InvokeDynamic.bootstrap_method_attr);
			printf("InvokeDynamic - Name and Type Index: %04x\n", aux->cp_union.InvokeDynamic.name_and_type);
		} else {
			printf("DefaultImpressao\n");
		}
	}
	printf("\n═════════════════════════════════════════════════════════\n");

	printf("\n\n═════════════════════ INTERFACES ══════════════════════\n\n");

	contador = 0;
	for (u2 * auxInterfaces = arquivoClass->interfaces; auxInterfaces < arquivoClass->interfaces+arquivoClass->interfaces_count; auxInterfaces++) {
		ponteiroprint = decode_name_index_and_name_type(arquivoClass->constant_pool,*auxInterfaces,NAME_INDEX);
		printf("Interface: cp_info#%d <%s>\n",*auxInterfaces, ponteiroprint);
	}
}

char* search_tag_name(u1 tag)
{
	char* tagname = malloc(40 * sizeof(char));

	if (tag == CONSTANT_Class) {
		strcpy(tagname, "CONSTANT_Class_Info");
	} else if (tag == CONSTANT_Field) {
		strcpy(tagname, "CONSTANT_Field_Info");
	} else if (tag == CONSTANT_Method) {
		strcpy(tagname, "CONSTANT_Method_Info");
	} else if (tag == CONSTANT_InterfaceMethod) {
		strcpy(tagname, "CONSTANT_InterfaceMethod_Info");
	} else if (tag == CONSTANT_String) {
		strcpy(tagname, "CONSTANT_String_Info");
	} else if (tag == CONSTANT_Integer) {
		strcpy(tagname, "CONSTANT_Integer_Info");
	} else if (tag == CONSTANT_Float) {
		strcpy(tagname, "CONSTANT_Float_Info");
	} else if (tag == CONSTANT_Long) {
		strcpy(tagname, "CONSTANT_Long_Info");
	} else if (tag == CONSTANT_Double) {
		strcpy(tagname, "CONSTANT_Double_Info");
	} else if (tag == CONSTANT_NameAndType) {
		strcpy(tagname, "CONSTANT_NameAndType_Info");
	} else if (tag == CONSTANT_Utf8) {
		strcpy(tagname, "CONSTANT_Utf8_Info");
	} else if (tag == CONSTANT_MethodHandle) {
		strcpy(tagname, "CONSTANT_MethodHandle_Info");
	} else if (tag == CONSTANT_MethodType) {
		strcpy(tagname, "CONSTANT_MethodType_Info");
	} else if (tag == CONSTANT_InvokeDynamic) {
		strcpy(tagname, "CONSTANT_InvokeDynamic_Info");
	} else {
		free(tagname);
		return NULL;
	}
	return tagname;
}

char* decode_string_utf8(cp_info *cp)
{
	char* decodedString = malloc((cp->cp_union.Utf8.length + 1) * sizeof(char));
	char *auxResult = decodedString;

	for (u1 *aux = cp->cp_union.Utf8.bytes; aux < cp->cp_union.Utf8.bytes + cp->cp_union.Utf8.length; aux++) {
		*auxResult = (char)*aux;
		auxResult++;
	}

	*auxResult = '\0';

	return decodedString;
}

// CP INFO STARTS FROM 1, WHICH IS WHY WE SUBTRACT 1 IN THE SUM
char* decode_name_index_and_name_type(cp_info *cp, u2 index, u1 type)
{
	char *result = malloc(100 * sizeof(u1));

	cp_info *aux;
	cp_info *aux2;
	cp_info *aux3;

	aux = cp + index - 1;

	if (type == NAME_INDEX) {
		aux2 = cp + (aux->cp_union.Class.name - 1);
		result = decode_string_utf8(aux2);
	} else if (type == NAME_AND_TYPE) {
		aux2 = cp + (aux->cp_union.NameAndType.name - 1);
		aux3 = cp + (aux->cp_union.NameAndType.descriptor - 1);

		strcpy(result, "");
		strcat(result, decode_string_utf8(aux2));
		strcat(result, ":");
		strcat(result, decode_string_utf8(aux3));
	} else if (type == STRING_INDEX || type == CLASS_INDEX || type == NAME_AND_TYPE_INFO_NAME_INDEX || type == NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX) {
		result = decode_string_utf8(aux);
	}

	return result;
}

double decode_double_info(cp_info *cp)
{
	uint64_t value = ((uint64_t)cp->cp_union.Double.hi_bytes << 32) | (uint64_t)cp->cp_union.Double.lo_bytes;
	int sign = ((value >> 63) == 0) ? 1 : -1;
	int exponent = ((value >> 52) & 0x7ffL);
	long mantissa = (exponent == 0) ? ((value & 0xfffffffffffffL) << 1) : ((value & 0xfffffffffffffL) | 0x10000000000000L);

	double result = sign * mantissa * (pow(2, exponent - 1075));
	return result;
}

uint64_t decode_long_info (cp_info * cp)
{
	return ((((uint64_t)cp->cp_union.Long.hi_bytes)<<32) | ((uint64_t)cp->cp_union.Long.lo_bytes));
}