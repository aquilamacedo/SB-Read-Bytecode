#include "reader.h"
#include <string.h>

// Function to read U1 type from .class
uint8_t U1READ(FILE *arq) {
  return getc(arq);
}
// Function to read U2 type from .class
uint16_t U2READ(FILE *arq) {
  uint16_t value = getc(arq);
  value = (value << 8) | getc(arq);
  return value;
}
// Function to read U4 type from .class
uint32_t U4READ(FILE *arq) {
  uint32_t value = getc(arq);
  value = (value << 8) | getc(arq);
  value = (value << 8) | getc(arq);
  value = (value << 8) | getc(arq);
  return value;
}

cp_info* read_constant_pool(FILE* fp, cp_info* cp, U2 cp_count)
{
	cp = (cp_info*)malloc(cp_count * sizeof(cp_info));

	for (cp_info* p = cp; p < cp + cp_count - 1; p++) {
		p->tag = U1READ(fp);

		if (p->tag == CONSTANT_Class) {
			p->cpUnion.Class.name = U2READ(fp);
		} else if (p->tag == CONSTANT_Field) {
			p->cpUnion.Field.class_name = U2READ(fp);
			p->cpUnion.Field.name_and_type = U2READ(fp);
		} else if (p->tag == CONSTANT_Method) {
			p->cpUnion.Method.class_name = U2READ(fp);
			p->cpUnion.Method.name_and_type = U2READ(fp);
		} else if (p->tag == CONSTANT_InterfaceMethod) {
			p->cpUnion.InterfaceMethod.class_name = U2READ(fp);
			p->cpUnion.InterfaceMethod.name_and_type = U2READ(fp);
		} else if (p->tag == CONSTANT_String) {
			p->cpUnion.String.string = U2READ(fp);
		} else if (p->tag == CONSTANT_Integer) {
			p->cpUnion.Integer.bytes = U4READ(fp);
		} else if (p->tag == CONSTANT_Float) {
			p->cpUnion.Float.bytes = U4READ(fp);
		} else if (p->tag == CONSTANT_Long) {
			p->cpUnion.Long.hi_bytes = U4READ(fp);
			p->cpUnion.Long.lo_bytes = U4READ(fp);
			p++;
		} else if (p->tag == CONSTANT_Double) {
			p->cpUnion.Double.hi_bytes = U4READ(fp);
			p->cpUnion.Double.lo_bytes = U4READ(fp);
			p++;
		} else if (p->tag == CONSTANT_NameAndType) {
			p->cpUnion.NameAndType.name = U2READ(fp);
			p->cpUnion.NameAndType.descriptor = U2READ(fp);
		} else if (p->tag == CONSTANT_Utf8) {
			p->cpUnion.Utf8.length = U2READ(fp);
			p->cpUnion.Utf8.bytes = malloc(p->cpUnion.Utf8.length * sizeof(U1));
			for (U1* i = p->cpUnion.Utf8.bytes; i < p->cpUnion.Utf8.bytes + p->cpUnion.Utf8.length; i++) {
				*i = U1READ(fp);
			}
		} else if (p->tag == CONSTANT_MethodHandle) {
			p->cpUnion.MethodHandle.reference_kind = U1READ(fp);
			p->cpUnion.MethodHandle.reference = U2READ(fp);
		} else if (p->tag == CONSTANT_MethodType) {
			p->cpUnion.MethodType.descriptor = U2READ(fp);
		} else if (p->tag == CONSTANT_InvokeDynamic) {
			p->cpUnion.InvokeDynamic.bootstrap_method_attr = U2READ(fp);
			p->cpUnion.InvokeDynamic.name_and_type = U2READ(fp);
		} else {
			// Handle the default case if necessary.
		}
	}

	return cp;
}

U2* read_interfaces (FILE *fp, U2* interface, U2 i_count)
{
	interface = (U2 *) malloc(i_count * sizeof(U2));
	for (U2 *p = interface; p < interface + i_count; p++){
		*interface = U2READ(fp);
	}
	return interface;
}

field_info* read_fields (FILE *fp, field_info *field, U2 f_count, cp_info *cp)
{
    field = (field_info *) malloc(f_count * sizeof(field_info));

    for(field_info *p = field; p < field + f_count; p++){
			p->access_flags = U2READ(fp);
			p->name = U2READ(fp);
			p->descriptor = U2READ(fp);
			p->attributes_count = U2READ(fp);

			if (p->attributes_count > 0) {
				p->attributes = (attribute_info **) malloc(p->attributes_count * sizeof(attribute_info*));
				for (int i = 0; i < p->attributes_count; p++) {
					*(p->attributes + i) = read_attributes(fp, *(p->attributes + i), cp);
				}
			}
		}
    return field;
}

stack_map_frame* read_stack_map_frame (FILE *fp)
{
	stack_map_frame * StackMapFrame = (stack_map_frame*)malloc(sizeof(stack_map_frame));
	StackMapFrame->frame_type = U1READ(fp);
	if (StackMapFrame->frame_type >= 0 && StackMapFrame->frame_type <= 63) {
	} else if (StackMapFrame->frame_type >= 64 && StackMapFrame->frame_type <= 127) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack) = read_verification_type_info(fp);
	} else if (StackMapFrame->frame_type >= 248 && StackMapFrame->frame_type <= 250) {
		StackMapFrame->map_frame_type.chop_frame.offset_delta = U2READ(fp);
	} else if (StackMapFrame->frame_type == 251) {
		StackMapFrame->map_frame_type.same_frame_extended.offset_delta = U2READ(fp);
	} else if (StackMapFrame->frame_type == 247) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = U2READ(fp);
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack) = read_verification_type_info(fp);
	} else if (StackMapFrame->frame_type >= 252 && StackMapFrame->frame_type <= 254) {
		StackMapFrame->map_frame_type.append_frame.offset_delta = U2READ(fp);
		U2 sizeMalloc = (StackMapFrame->frame_type-251);
		StackMapFrame->map_frame_type.append_frame.locals = (verification_type_info**)malloc(sizeMalloc*sizeof(verification_type_info*));
		for (int posicao = 0; posicao < sizeMalloc; posicao++) {
			*(StackMapFrame->map_frame_type.append_frame.locals+posicao) = read_verification_type_info(fp);
		}
	} else if (StackMapFrame->frame_type == 255) {
		StackMapFrame->map_frame_type.full_frame.offset_delta = U2READ(fp);
		StackMapFrame->map_frame_type.full_frame.num_locals = U2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.num_locals > 0) {
			StackMapFrame->map_frame_type.full_frame.locals = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.num_locals*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.num_locals; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.locals+posicao) = read_verification_type_info(fp);
				if ((*(StackMapFrame->map_frame_type.full_frame.locals+posicao))->tag == 7) {
				}
			}
		}
		StackMapFrame->map_frame_type.full_frame.num_stack_items = U2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.num_stack_items > 0) {
			StackMapFrame->map_frame_type.full_frame.stack = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.num_stack_items*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.num_stack_items; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.stack+posicao) = read_verification_type_info(fp);
			}
		}
	}
	return StackMapFrame;
}


attribute_info* read_attributes(FILE *fp, attribute_info* attribute_information, cp_info *cp)
{
	attribute_information = (attribute_info *)malloc(sizeof(attribute_info));

	attribute_information->attribute_name = U2READ(fp);
	attribute_information->attribute_length = U4READ(fp);

	if (attribute_information->attribute_length > 0) {
		char *string_name;
		string_name = decode_string_utf8(cp + attribute_information->attribute_name - 1);

		if (strcmp(string_name, "SourceFile") == 0) {
			source_file_attribute *SourceFile = NULL;
			SourceFile = read_source_file(fp);
			attribute_information->info = NULL;
			attribute_information->info = (source_file_attribute*)SourceFile;
		}
		else if (strcmp(string_name, "LineNumberTable") == 0) {
			line_number_table *line_n_t = NULL;
			printf("Entered LineNumberTable\n");
			line_n_t = read_line_number_table(fp, cp);
			attribute_information->info = (line_number_table*)line_n_t;
		}
		else if (strcmp(string_name, "StackMapTable") == 0) {
			printf("Entered stack map table\n");
			stack_map_attribute *stack_map_table = NULL;
			stack_map_table = read_stack_map_table(fp);
			printf("Exited smt\n");
			attribute_information->info = (stack_map_attribute*)stack_map_table;
		}
		else if (strcmp(string_name, "Exceptions") == 0) {
			exception_attribute *exceptions = NULL;
			exceptions = read_exceptions_attribute(fp);
			attribute_information->info = (exception_attribute*)exceptions;
		}
		else if (strcmp(string_name, "InnerClasses") == 0) {
			printf("Entered inner class\n");
			inner_classes_attribute *inner_classes = NULL;
			inner_classes = read_inner_classes(fp, cp);
			attribute_information->info = (inner_classes_attribute*)inner_classes;
		}
		else if (strcmp(string_name, "Signature") == 0) {
			printf("Entered signature\n");
			signature_attribute *signatureR = NULL;
			signatureR = read_signature(fp);
			attribute_information->info = (signature_attribute*)signatureR;
		}
		else if (strcmp(string_name, "Code") == 0) {
			printf("Entered code\n");
			code_attribute *code_attr = NULL;
			code_attr = read_code(fp, cp);
			attribute_information->info = (code_attribute*)code_attr;
		}
		else if (strcmp(string_name, "ConstantValue") == 0) {
			constant_value_attribute *constantV = NULL;
			constantV = read_constant_value(fp);
			attribute_information->info = (constant_value_attribute*)constantV;
		}
	}
	return attribute_information;
}

source_file_attribute* read_source_file (FILE *fp)
{
	source_file_attribute * SourceFile = NULL;
	SourceFile = (source_file_attribute*)malloc(sizeof(source_file_attribute));
	SourceFile->source_file = U2READ(fp);
	return SourceFile;
}

code_attribute* read_code (FILE * fp, cp_info *cp)
{
	code_attribute * code_attr = NULL;
	code_attr = (code_attribute*) malloc(sizeof(code_attribute));
	code_attr->max_stack = U2READ(fp);
	code_attr->max_locals = U2READ(fp);
	code_attr->code_length = U4READ(fp);
	if(code_attr->code_length > 0) {
		code_attr->code = malloc(code_attr->code_length*sizeof(U1));
		for(U1 *p = code_attr->code; p < code_attr->code + code_attr->code_length; p++){
			*p = U1READ(fp);
		}
	}

	code_attr->exception_info_length = U2READ(fp);

	if(code_attr->exception_info_length > 0){
		code_attr->ex_info = NULL;
		code_attr->ex_info = read_exception_table(fp, code_attr->exception_info_length);
	}

	code_attr->attributes_count = U2READ(fp);

	if (code_attr->attributes_count > 0){
		code_attr->attributes = (attribute_info**)malloc(code_attr->attributes_count*sizeof(attribute_info*));
		for (int i = 0; i < code_attr->attributes_count; i++) {
			*(code_attr->attributes + i) = read_attributes(fp, *(code_attr->attributes + i), cp);
		}
	}

	return code_attr;
}

line_number_table* read_line_number_table(FILE * fp, cp_info * cp)
{
	line_number_table * line_n_t = (line_number_table*)malloc(sizeof(line_number_table));
	line_n_t->line_number_length = U2READ(fp);
	if (line_n_t->line_number_length > 0) {
		line_n_t->info = (line_number_info*)malloc(line_n_t->line_number_length*sizeof(line_number_info));
		for (line_number_info * linfo = line_n_t->info; linfo < line_n_t->info+line_n_t->line_number_length; linfo++) {
			linfo->start_pc = U2READ(fp);
			linfo->line_number = U2READ(fp);
		}
	}
	return line_n_t;
}

stack_map_attribute* read_stack_map_table (FILE *fp)
{
	stack_map_attribute * stack_map_table = (stack_map_attribute*)malloc(sizeof(stack_map_attribute));
	stack_map_table->num_entries = U2READ(fp);
	if (stack_map_table->num_entries > 0) {
		stack_map_table->entries = (stack_map_frame**)malloc(stack_map_table->num_entries*sizeof(stack_map_frame*));
		for (int posicao = 0; posicao < stack_map_table->num_entries; posicao++) {
			*(stack_map_table->entries+posicao) = read_stack_map_frame(fp);
		}
	}
	return stack_map_table;
}

inner_classes_attribute* read_inner_classes (FILE *fp, cp_info *cp)
{
	inner_classes_attribute * inner_classes = (inner_classes_attribute*)malloc(sizeof(inner_classes_attribute));
	inner_classes->num_classes = U2READ(fp);
	if (inner_classes->num_classes > 0) {
		inner_classes->classes_array = (classes**)malloc(inner_classes->num_classes*sizeof(classes*));
		for (int posicao = 0; posicao < inner_classes->num_classes; posicao++) {
			*(inner_classes->classes_array+posicao) = read_classes(fp);
		}
	}
	return inner_classes;
}

signature_attribute* read_signature (FILE *fp)
{
	signature_attribute * signature = (signature_attribute*)malloc(sizeof(signature_attribute));
	signature->signature = U2READ(fp);
	return signature;
}

constant_value_attribute* read_constant_value (FILE *fp)
{
	constant_value_attribute * cv = (constant_value_attribute*)malloc(sizeof(constant_value_attribute));
	cv->constant_value = U2READ(fp);
	return cv;
}

exception_attribute* read_exceptions_attribute (FILE *fp)
{
	exception_attribute * exceptions = (exception_attribute*)malloc(sizeof(exception_attribute));
	exceptions->num_exceptions = U2READ(fp);
	exceptions->exception_info = NULL;
	if (exceptions->num_exceptions > 0) {
		exceptions->exception_info = (U2*)malloc(exceptions->num_exceptions*sizeof(U2));
		for (U2 * excpAux = exceptions->exception_info; excpAux < exceptions->exception_info + exceptions->num_exceptions; excpAux++) {
			*excpAux = U2READ(fp);
		}
	}
	return exceptions;
}

verification_type_info* read_verification_type_info (FILE *fp)
{
	verification_type_info * VTI = (verification_type_info*)malloc(sizeof(verification_type_info));
	VTI->tag = U1READ(fp);
	switch (VTI->tag) {
		case 7:
			VTI->type_info.object_variable_info.cp = U2READ(fp);
			break;
		case 8:
			VTI->type_info.uninitialized_variable_info.offset = U2READ(fp);
			break;
		default:
			break;
	}

	return VTI;
}

exception_info* read_exception_table (FILE * fp, U2 size)
{
	exception_info * exception_tableAux = (exception_info*) malloc(size*sizeof(exception_info));
	for(exception_info * e = exception_tableAux; e < exception_tableAux + size; e++){
		e->start_pc = U2READ(fp);
		e->end_pc = U2READ(fp);
		e->handler_pc = U2READ(fp);
		e->catch_type = U2READ(fp);
	}
	return exception_tableAux;
}

method_info* read_methods (FILE *fp, method_info* metodo, U2 m_count, cp_info *cp)
{
  metodo = (method_info *) malloc(m_count * sizeof(method_info));

  for(method_info *p = metodo; p < metodo + m_count; p++){
    p->access_flags = U2READ(fp);
    p->name = U2READ(fp);
    p->descriptor = U2READ(fp);
    p->attributes_count = U2READ(fp);

		/*Conditional structure that evaluates whether the number of attributes
		of the method is greater than zero. If so, continue reading the
		method attributes*/
		attribute_info *atributos = (attribute_info *) malloc(p->attributes_count * sizeof(attribute_info));

		if (p->attributes_count > 0) {
			p->attributes = (attribute_info **) malloc(p->attributes_count * sizeof(attribute_info*));
			for (int i = 0; i < p->attributes_count; i++) {
				*(p->attributes + i) = read_attributes(fp,	*(p->attributes + i), cp);
			}
		}
  }

  return metodo;
}

classes* read_classes (FILE *fp)
{
	classes *classeRetorno = (classes*)malloc(sizeof(classes));
	classeRetorno->inner_class_info = U2READ(fp);
	classeRetorno->outer_class_info = U2READ(fp);
	classeRetorno->inner_name = U2READ(fp);
	classeRetorno->inner_class_access_flags = U2READ(fp);

	return classeRetorno;
}

ClassFile* read_file (ClassFile* cf, char *filename)
{
  FILE *fp = fopen(filename, "rb");

  if(!fp){
    printf("Error opening file...\n");
    exit(0);
  }

  cf->magic = U4READ(fp);
  cf->minor_version = U2READ(fp);
  cf->major_version = U2READ(fp);
  cf->constant_pool_count = U2READ(fp);
  cf->constant_pool = read_constant_pool(fp, cf->constant_pool, cf->constant_pool_count);
  cf->access_flags = U2READ(fp);
  cf->this_class = U2READ(fp);
  cf->super_class = U2READ(fp);

  cf->interfaces_count = U2READ(fp);
  cf->interfaces = (cf->interfaces_count > 0) ?
                    read_interfaces(fp, cf->interfaces, cf->interfaces_count) : NULL;

	cf->fields_count = U2READ(fp);
	cf->fields = (cf->fields_count > 0) ?
								read_fields(fp, cf->fields, cf->fields_count, cf->constant_pool) : NULL;

	/*Leitura do valor 'methods_count' = quantidade de entradas na tabela Method*/
	cf->methods_count = U2READ(fp);
	cf->methods = (cf->methods_count > 0) ?
									read_methods(fp, cf->methods, cf->methods_count, cf->constant_pool) : NULL;

	cf->attributes_count = U2READ(fp);
	if(cf->attributes_count > 0)
	{
		cf->attributes = (attribute_info **) malloc(cf->attributes_count * sizeof(attribute_info*));
		for (int i = 0; i < cf->attributes_count; i++) {
			*(cf->attributes + i) = read_attributes(fp, *(cf->attributes + i), cf->constant_pool);
		}
	}

  fclose(fp);

  return cf;
}