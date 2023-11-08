#include <string.h>
#include <math.h>
#include "display.h"
#include "reader.h"

void display_classfile (ClassFile * class_file){
  cp_info * aux;
	double valor;
	uint64_t longValue;
	method_info * auxMethod;
	field_info * aux_field;
	attribute_info ** auxAttributeClasse;
	attribute_info ** fieldAttrAux;
	exception_info * exceptionTableAux;
	uint32_t count = 1;
	char *ponteiroprint;

	printf("\n╔═════════════════════ CLASS INFO ═════════════════════╗\n");
	printf("║  Magic:               0x%08x                     ║\n", class_file->magic);
	printf("║  Minor Version:       0x%04x                         ║\n", class_file->minor_version);
	printf("║  Major Version:       0x%04x                         ║\n", class_file->major_version);
	printf("║  Constant Pool Count: 0x%04x                         ║\n", class_file->constant_pool_count);
	printf("║  Access Flags:        0x%04x                         ║\n", class_file->access_flags);
	printf("║  This Class:          0x%04x                         ║\n", class_file->this_class);
	printf("║  Super Class:         0x%04x                         ║\n", class_file->super_class);
	printf("║  Interfaces Count:    0x%04x                         ║\n", class_file->interfaces_count);
	printf("║  Fields Count:        0x%04x                         ║\n", class_file->fields_count);
	printf("║  Methods Count:       0x%04x                         ║\n", class_file->methods_count);
	printf("║  Attributes Count:    0x%02x                           ║\n", class_file->attributes_count);
	printf("╚══════════════════════════════════════════════════════╝\n\n");


	printf("════════════════════ CONSTANT POOL ═════════════════════\n");

	for (aux = class_file->constant_pool; aux < class_file->constant_pool + class_file->constant_pool_count - 1; aux++) {
			printf("\n═════════════════════ INDEX [%d] ════════════════════════\n\n", count);
			count++;
			printf("TAG: %02d: %s\n", aux->tag, search_tag_name(aux->tag));

			if (aux->tag == CONSTANT_Class) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.Class.name, CLASS_INDEX);
					printf("Class Name: cp_info#%02d <%s>\n", aux->cpUnion.Class.name, ponteiroprint);
			} else if (aux->tag == CONSTANT_Float) {
					printf("Bytes: %04x\n", aux->cpUnion.Float.bytes);
					printf("Float: %d\n", aux->cpUnion.Float.bytes);
			} else if (aux->tag == CONSTANT_Field) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.Field.class_name, NAME_INDEX);
					printf("Class Name: cp_info#%02d <%s>\n", aux->cpUnion.Field.class_name, ponteiroprint);
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.Field.name_and_type, NAME_AND_TYPE);
					printf("Name and Type: cp_info#%02d <%s>\n", aux->cpUnion.Field.name_and_type, ponteiroprint);
			} else if (aux->tag == CONSTANT_InterfaceMethod) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.String.string, NAME_INDEX);
					printf("Class Name: cp_info#%02d <%s>\n", aux->cpUnion.InterfaceMethod.class_name, ponteiroprint);
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.String.string, NAME_AND_TYPE);
					printf("Name and Type Index: cp_info#%02d <%s>\n", aux->cpUnion.InterfaceMethod.name_and_type, ponteiroprint);
			} else if (aux->tag == CONSTANT_String) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.String.string, STRING_INDEX);
					printf("String: cp_info#%02d <%s>\n", aux->cpUnion.String.string, ponteiroprint);
			} else if (aux->tag == CONSTANT_Integer) {
					printf("Bytes: %04x\n", aux->cpUnion.Integer.bytes);
					printf("Integer: %d\n", aux->cpUnion.Integer.bytes);
			} else if (aux->tag == CONSTANT_Long) {
					longValue = decode_long_info(aux);
					printf("Long High Bytes: 0x%08x\n", aux->cpUnion.Long.hi_bytes);
					printf("Long Low Bytes: 0x%08x\n", aux->cpUnion.Long.lo_bytes);
					printf("Long: %lu\n", longValue);
			} else if (aux->tag == CONSTANT_NameAndType) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.NameAndType.name, NAME_AND_TYPE_INFO_NAME_INDEX);
					printf("Name: cp_info#%02d <%s>\n", aux->cpUnion.NameAndType.name, ponteiroprint);
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.NameAndType.descriptor, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
					printf("Descriptor: cp_info#%02d <%s>\n", aux->cpUnion.NameAndType.descriptor, ponteiroprint);
			} else if (aux->tag == CONSTANT_Utf8) {
					printf("Length of byte array: %d\n", (int) aux->cpUnion.Utf8.length);
					printf("Length of string: %d\n", (int) aux->cpUnion.Utf8.length);
					printf("String: ");
					for (U1 *i = aux->cpUnion.Utf8.bytes; i < aux->cpUnion.Utf8.bytes + aux->cpUnion.Utf8.length; i++) {
							printf("%c", (char) (*i));
					}
					printf("\n");
			} else if (aux->tag == CONSTANT_MethodHandle) {
					printf("MethodHandle Reference Kind: %02x\n", aux->cpUnion.MethodHandle.reference_kind);
					printf("MethodHandle Reference Index: %04x\n", aux->cpUnion.MethodHandle.reference);
			} else if (aux->tag == CONSTANT_Method) {
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.Method.class_name, NAME_INDEX);
					printf("Class Name: cp_info#%02d <%s>\n", aux->cpUnion.Method.class_name, ponteiroprint);
					ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool, aux->cpUnion.Method.name_and_type, NAME_AND_TYPE);
					printf("Name and Type: cp_info#%02d <%s>\n", aux->cpUnion.Method.name_and_type, ponteiroprint);
			} else if (aux->tag == CONSTANT_Double) {
					valor = decode_double_info(aux);
					printf("Double High Bytes: 0x%08x\n", aux->cpUnion.Double.hi_bytes);
					printf("Double Low Bytes: 0x%08x\n", aux->cpUnion.Double.lo_bytes);
					printf("Double: %lf\n", valor);
			} else if (aux->tag == CONSTANT_MethodType) {
					printf("MethodType Descriptor Index: %04x\n", aux->cpUnion.MethodType.descriptor);
			} else if (aux->tag == CONSTANT_InvokeDynamic) {
					printf("InvokeDynamic - Bootstrap Method Attr Index: %04x\n", aux->cpUnion.InvokeDynamic.bootstrap_method_attr);
					printf("InvokeDynamic - Name and Type Index: %04x\n", aux->cpUnion.InvokeDynamic.name_and_type);
			} else {
					printf("DefaultImpressao\n");
			}
	}
	printf("\n═════════════════════════════════════════════════════════\n");

	printf("\n\n═════════════════════ INTERFACES ══════════════════════\n\n");
	count = 0;
	for (U2 * auxInterfaces = class_file->interfaces; auxInterfaces < class_file->interfaces+class_file->interfaces_count; auxInterfaces++) {
		ponteiroprint = decode_name_index_and_name_type(class_file->constant_pool,*auxInterfaces,NAME_INDEX);
		printf("Interface: cp_info#%d <%s>\n",*auxInterfaces, ponteiroprint);
	}

	printf("\n═════════════════════════════════════════════════════════\n");

	count = 0;
	for (aux_field = class_file->fields; aux_field < class_file->fields + class_file->fields_count; aux_field++,count++) {
		printf("═════════════════════ FIELD [%d] ════════════════════════\n\n",count);
		ponteiroprint = decode_string_utf8(class_file->constant_pool-1+aux_field->name);
		printf("Name: cp_info#%d <%s>\n",aux_field->name,ponteiroprint);
		ponteiroprint = decode_string_utf8(class_file->constant_pool-1+aux_field->descriptor);
		printf("Descriptor: cp_info#%d <%s>\n",aux_field->descriptor,ponteiroprint);
		ponteiroprint = decode_access_flags(aux_field->access_flags);
		printf("Access Flags: 0x%04x [%s]\n",aux_field->access_flags,ponteiroprint);
		printf("Attributes Count: %d\n",aux_field->attributes_count);
		if (aux_field->attributes_count > 0) {
			fieldAttrAux = aux_field->attributes;
			for (int positionFields = 0; positionFields < aux_field->attributes_count; positionFields++) {
				ponteiroprint = decode_string_utf8(class_file->constant_pool-1+(*(fieldAttrAux+positionFields))->attribute_name);
				printf("Attribute Name Index: cp_info#%d <%s>\n",(*(fieldAttrAux+positionFields))->attribute_name,ponteiroprint);
				printf("Attribute Length: %d\n",(*(fieldAttrAux+positionFields))->attribute_length);
				if (strcmp(ponteiroprint, "ConstantValue") == 0) {
					constant_value_attribute * cvAux = (constant_value_attribute*)(*(fieldAttrAux+positionFields))->info;
					cp_info * cpInfoAux = class_file->constant_pool-1+cvAux->constant_value;
					if (cpInfoAux->tag == 4) {
						float valorCV = decode_float_info(class_file->constant_pool-1+cvAux->constant_value);
						printf("Constant Value Index: cp_info#%d <%f>\n",cvAux->constant_value,valorCV);
					} else if (cpInfoAux->tag == 8) {
						char * stringEntrada = decode_name_index_and_name_type(class_file->constant_pool,cvAux->constant_value,NAME_INDEX);
						printf("Constant Value Index: cp_info#%d <%s>\n",cvAux->constant_value,stringEntrada);
					} else if (cpInfoAux->tag == 3) {
						int valorRetorno = decode_integer_info (class_file->constant_pool-1+cvAux->constant_value);
						printf("Constant Value Index: cp_info#%d <%d>\n",cvAux->constant_value,valorRetorno);
					} else if (cpInfoAux->tag == 6) {
						double valorDB = decode_double_info(class_file->constant_pool-1+cvAux->constant_value);
						printf("Constant Value Index: cp_info#%d <%lf>\n",cvAux->constant_value,valorDB);
					} else if (cpInfoAux->tag == 5) {
						uint64_t valorL = decode_long_info(class_file->constant_pool-1+cvAux->constant_value);
						printf("Constant Value Index: cp_info#%d <%lu>\n",cvAux->constant_value,valorL);
					}
				} else if (strcmp(ponteiroprint,"Signature") == 0) {
					signature_attribute * sig = (signature_attribute*)(*(fieldAttrAux+positionFields))->info;
					char * Signature_Index = decode_string_utf8(class_file->constant_pool-1+sig->signature);
					printf("Signature index: cp_info#%d <%s>\n",sig->signature,Signature_Index);
				}
			}
		}
	}

	printf("\n\n═════════════════════ METHODS ══════════════════════\n\n");

	count = 0;

    for (auxMethod = class_file->methods; auxMethod < class_file->methods + class_file->methods_count; auxMethod++,count++) {
		printf("═════════════════════ METHOD [%d] ════════════════════════\n\n",count);
		ponteiroprint = decode_string_utf8(class_file->constant_pool-1+auxMethod->name);
		printf("Name: cp_info#%d <%s>\n",auxMethod->name,ponteiroprint);
		ponteiroprint = decode_string_utf8(class_file->constant_pool-1+auxMethod->descriptor);
		printf("Descriptor: cp_info#%d <%s>\n",auxMethod->descriptor,ponteiroprint);
		ponteiroprint = decode_access_flags(auxMethod->access_flags);
		printf("Access Flags: 0x%04x [%s]\n",auxMethod->access_flags,ponteiroprint);
		printf("Attributes Count: %d\n",auxMethod->attributes_count);

		printf("Atributos:\n");
		attribute_info ** auxAttrCompleto = auxMethod->attributes;
		for (int position = 0; position < auxMethod->attributes_count; position++) {
			ponteiroprint = decode_string_utf8(class_file->constant_pool-1+(*(auxAttrCompleto+position))->attribute_name);
			printf("Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttrCompleto+position))->attribute_name,ponteiroprint);
			printf("Attribute Length: %d\n",(*(auxAttrCompleto+position))->attribute_length);

			if (strcmp(ponteiroprint,"Code") == 0) {
				code_attribute * auxCodePontual = (code_attribute*)(*(auxAttrCompleto+position))->info;
				printf("Max Stack: %d\n",auxCodePontual->max_stack);
				printf("Max Locals: %d\n",auxCodePontual->max_locals);
				printf("Code length: %d\n",auxCodePontual->code_length);
			} else if (strcmp(ponteiroprint,"Exceptions") == 0) {
				exception_attribute * excpAux = (exception_attribute*)(*(auxAttrCompleto+position))->info;
				int countExcp = 0;
				char * exceptionIndexString;
				printf("Nr.\t\tException\t\t\tVerbose\n");
				for (U2 * indexExcp = excpAux->exception_info; indexExcp < excpAux->exception_info + excpAux->num_exceptions; indexExcp++) {
					exceptionIndexString = decode_name_index_and_name_type(class_file->constant_pool,*indexExcp,NAME_INDEX);
					printf("%d\t\tcp_info#%d\t\t\t%s\n",countExcp,*indexExcp,exceptionIndexString);
					countExcp++;
				}
			} else if (strcmp(ponteiroprint,"Signature") == 0) {
				signature_attribute * sig = (signature_attribute*)(*(auxAttrCompleto+position))->info;
				char * Signature_Index = decode_string_utf8(class_file->constant_pool-1+sig->signature);
				printf("Signature index: cp_info#%d <%s>\n",sig->signature,Signature_Index);
			}
		}
	}
	printf("\n\n═════════════════════ CLASS ATTRIBUTES ══════════════════════\n\n");

	auxAttributeClasse = class_file->attributes;
	for (int position = 0; position < class_file->attributes_count; position++) {
		ponteiroprint = decode_string_utf8(class_file->constant_pool+(*(auxAttributeClasse+position))->attribute_name-1);
		printf("Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttributeClasse+position))->attribute_name,ponteiroprint);
		printf("Attribute Length: %d\n",(int) (*(auxAttributeClasse+position))->attribute_length);
		if (strcmp(ponteiroprint,"SourceFile") == 0) {
		 	source_file_attribute * SourceFile = ((source_file_attribute*)((*(auxAttributeClasse+position))->info));
			printf("Source File Name Index: cp_info#%d <%s>\n",SourceFile->source_file,decode_string_utf8(class_file->constant_pool+SourceFile->source_file-1));
		} else if (strcmp(ponteiroprint, "InnerClasses") == 0) {
			inner_classes_attribute * innerC = ((inner_classes_attribute*)((*(auxAttributeClasse+position))->info));
			printf("Nr.\t\tInner Class\t\t\tOuter Class\t\tInner Name\t\tAccess Flags\n");
			char * innerClassString, * outerClassString, * innerNameIndex, * accessFlagsInner;
			classes ** vetorClasses = innerC->classes_array;
			for (int positionInncerC = 0; positionInncerC < innerC->num_classes; positionInncerC++) {
				innerClassString = decode_name_index_and_name_type(class_file->constant_pool,(*(vetorClasses+positionInncerC))->inner_class_info,NAME_INDEX);
				outerClassString = decode_name_index_and_name_type(class_file->constant_pool,(*(vetorClasses+positionInncerC))->outer_class_info,NAME_INDEX);
				innerNameIndex = decode_string_utf8(class_file->constant_pool-1+(*(vetorClasses+positionInncerC))->inner_name);
				accessFlagsInner = decode_access_flags((*(vetorClasses+positionInncerC))->inner_class_access_flags);
				printf("%d\t\tcp_info#%d\t\t\tcp_info#%d\t\tcp_info#%d\t\t0x%04x\n",positionInncerC,(*(vetorClasses+positionInncerC))->inner_class_info,(*(vetorClasses+positionInncerC))->outer_class_info,(*(vetorClasses+positionInncerC))->inner_name,(*(vetorClasses+positionInncerC))->inner_class_access_flags);
				printf("  \t\t%s\t\t%s\t\t%s\t\t\t%s\n",innerClassString,outerClassString,innerNameIndex,accessFlagsInner);
			}
		} else if (strcmp(ponteiroprint,"Signature") == 0) {
			signature_attribute * sig = (signature_attribute*)((*(auxAttributeClasse+position))->info);
			char * Signature_Index = decode_string_utf8(class_file->constant_pool-1+sig->signature);
			printf("Signature index: cp_info#%d <%s>\n",sig->signature,Signature_Index);
		}
	}
}

char* search_tag_name(U1 tag) {
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

float decode_float_info (cp_info * cp) {
	U4 valor = cp->cpUnion.Float.bytes;
	float result;
	int signal = ((valor>>31) == 0) ? 1 : -1;
	int expon = ((valor>>23) & 0xff);
	int mant = (expon == 0) ? (valor & 0x7fffff)<<1 : (valor & 0x7fffff) | 0x800000;

	result = (signal)*(mant)*(pow(2,expon-150));

	return result;
}

char* arranging_flags(char* flags_reverse_order){
	int flag = 0;
	const char s[2] = ";";
	char* old = (char*)malloc(100*sizeof(char));
	char* flags = strtok(flags_reverse_order,s);
	char* new = (char*)malloc(100*sizeof(char));

	strcpy(new, "");
	strcpy(old, "");

	while(flags != NULL){
		if(flag == 0)
			strcpy(old, flags);

		if(flag > 0){
			strcpy(new, flags);
			strcat(new," ");
			strcat(new, old);
			strcpy(old, new);
			flag++;
		}
		else
			flag++;
		flags = strtok(NULL, s);
	}

	if(flag != 1){
		free(old);
		return new;
	} else{
		free(new);
		return old;
	}
}

int decode_integer_info (cp_info * cp) {
	U4 value = cp->cpUnion.Integer.bytes;
	return value;
}

char* decode_string_utf8(cp_info *cp) {
	char* decodedString = malloc((cp->cpUnion.Utf8.length + 1) * sizeof(char));
	char *auxResult = decodedString;

	for (U1 *aux = cp->cpUnion.Utf8.bytes; aux < cp->cpUnion.Utf8.bytes + cp->cpUnion.Utf8.length; aux++) {
		*auxResult = (char)*aux;
		auxResult++;
	}

	*auxResult = '\0';

	return decodedString;
}

char* decode_name_index_and_name_type(cp_info *cp, U2 index, U1 type) {
	char *result = malloc(100 * sizeof(U1));

	cp_info *aux;
	cp_info *aux2;
	cp_info *aux3;

	aux = cp + index - 1;

	if (type == NAME_INDEX) {
		aux2 = cp + (aux->cpUnion.Class.name - 1);
		result = decode_string_utf8(aux2);
	} else if (type == STRING_INDEX || type == CLASS_INDEX || type == NAME_AND_TYPE_INFO_NAME_INDEX || type == NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX) {
		result = decode_string_utf8(aux);
	} else if (type == NAME_AND_TYPE) {
		aux2 = cp + (aux->cpUnion.NameAndType.name - 1);
		aux3 = cp + (aux->cpUnion.NameAndType.descriptor - 1);

		strcpy(result, "");
		strcat(result, decode_string_utf8(aux2));
		strcat(result, ":");
		strcat(result, decode_string_utf8(aux3));
	}

	return result;
}

char* decode_access_flags(U2 flag) {
	char *result = malloc(100 * sizeof(char));
	strcpy(result, "");

	while (flag) {
		if (flag >= SYNTHETIC) {
			flag -= SYNTHETIC;
			strcat(result, "SYNTHETIC;");
		}
		if (flag >= TRANSIENT) {
			flag -= TRANSIENT;
			strcat(result, "TRANSIENT;");
		}
		if (flag >= VOLATILE) {
			flag -= VOLATILE;
			strcat(result, "VOLATILE;");
		}

		if (flag >= FINAL) {
			flag -= FINAL;
			strcat(result, "FINAL;");
		}

		if (flag >= STATIC) {
			flag -= STATIC;
			strcat(result, "STATIC;");
		}

		if (flag >= PROTECTED) {
			flag -= PROTECTED;
			strcat(result, "PROTECTED;");
		}

		if (flag >= PRIVATE) {
			flag -= PRIVATE;
			strcat(result, "PRIVATE;");
		}

		if (flag >= PUBLIC) {
			flag -= PUBLIC;
			strcat(result, "PUBLIC;");
		}
	}
	result = arranging_flags(result);
	return result;
}

double decode_double_info(cp_info *cp) {
	uint64_t value = ((uint64_t)cp->cpUnion.Double.hi_bytes << 32) | (uint64_t)cp->cpUnion.Double.lo_bytes;
	int sign = ((value >> 63) == 0) ? 1 : -1;
	int exponent = ((value >> 52) & 0x7ffL);
	long mantissa = (exponent == 0) ? ((value & 0xfffffffffffffL) << 1) : ((value & 0xfffffffffffffL) | 0x10000000000000L);
	double result = sign * mantissa * (pow(2, exponent - 1075));

	return result;
}

uint64_t decode_long_info (cp_info * cp) {
	return ((((uint64_t)cp->cpUnion.Long.hi_bytes)<<32) | ((uint64_t)cp->cpUnion.Long.lo_bytes));
}

int set_print_offset (int position, U1 offset) {
	if (position != 0) {
		return (offset+1);
	} else {
		return offset;
	}
}