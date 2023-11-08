#ifndef CLASSFILE_INIT
#define CLASSFILE_INIT

#include <stdint.h>

typedef uint8_t     U1;
typedef uint16_t    U2;
typedef uint32_t    U4;

#define NAME_INDEX 1
#define NAME_AND_TYPE 2
#define STRING_INDEX 3
#define CLASS_INDEX 4
#define NAME_AND_TYPE_INFO_NAME_INDEX 5
#define NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX 6
#define FIELD_INDEX 7
#define MAXU1 255
#define MAXU2 65535
#define MAXU4 2147483647

// Information about Attributes
struct attribute_info
{
    U2 attribute_name;
    U4 attribute_length;
    void *info;
};

typedef struct attribute_info attribute_info;

// Enum of possible access flags values
enum access_flags{
    PUBLIC = 1,
    PRIVATE = 2,
    PROTECTED = 4,
    STATIC = 8,
    FINAL = 16,
    VOLATILE = 64,
    TRANSIENT = 128,
    SYNTHETIC = 4096,
    ENUM = 16384
};

typedef enum access_flags access_flags;

// Enum of possible CP entry tag values
enum tag_values {

    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_Class = 7,
    CONSTANT_String = 8,
    CONSTANT_Field = 9,
    CONSTANT_Method = 10,
    CONSTANT_InterfaceMethod = 11,
    CONSTANT_NameAndType = 12,
    CONSTANT_MethodHandle = 15,
    CONSTANT_MethodType = 16,
    CONSTANT_InvokeDynamic = 18
};

// Information about Constant Pool
struct cp_info
{
    // Tag byte for CP entry type
    U1 tag;
    // Union with CP entry types
    union 
    {
        struct 
        {
            U2 name;
        } Class;

        struct 
        {
            U2 class_name;
            U2 name_and_type;
        } Field;
        
        struct 
        {
            U2 class_name;
            U2 name_and_type;
        } Method;

        struct
        {
            U2 class_name;
            U2 name_and_type;
        } InterfaceMethod;
        
        struct 
        {
            U2 string;
        } String;
        
        struct 
        {
            U4 bytes;
        } Integer;
        
        struct 
        {
            U4 bytes;
        } Float;
        
        struct 
        {
            U4 hi_bytes;
            U4 lo_bytes;
        } Long;

        struct 
        {
            U4 hi_bytes;
            U4 lo_bytes;
        } Double;
        
        struct 
        {
            U2 name;
            U2 descriptor;
        } NameAndType;
        
        struct 
        {
            U1 *bytes;
            U2 length;
        } Utf8;
        
        struct 
        {
            U1 reference_kind;
            U2 reference;
        } MethodHandle;

        struct 
        {
            U2 descriptor;
        } MethodType;
        
        struct 
        {
            U2 name_and_type;
            U2 bootstrap_method_attr;
        } InvokeDynamic;        

    } cpUnion;
};

typedef struct cp_info cp_info;

// Information about Fields
struct field_info
{
    U2 name;
    U2 descriptor;
    U2 attributes_count;
    U2 access_flags;
    attribute_info **attributes;
};

typedef struct field_info field_info;

// Information about Exceptions
struct exception_info
{
    U2 start_pc;
    U2 end_pc;
    U2 handler_pc;
    U2 catch_type;
};

typedef struct exception_info exception_info;

// Information about Line Numbers
struct line_number_info
{
    U2 line_number;
    U2 start_pc;
};

typedef struct line_number_info line_number_info;

// Information about Line Number Tables
struct line_number_table
{
    line_number_info *info;
    U2 line_number_length;
};

typedef struct line_number_table line_number_table;

// General Code Information
struct code_attribute
{
    U1 *code;

    U2 attributes_count;
    U2 exception_info_length;
    U2 max_locals;
    U2 max_stack;

    U4 code_length;

    attribute_info **attributes;

    exception_info *ex_info;
};

typedef struct code_attribute code_attribute;

// Information about Methods
struct method_info
{
    U2 name;
    U2 descriptor;
    U2 access_flags;

    U2 attributes_count;
    attribute_info **attributes;
};

typedef struct method_info method_info;

// Information about Source File Attributes
struct source_file_attribute
{
    U2 source_file;
};

typedef struct source_file_attribute source_file_attribute;

// Information about Constant Value Attributes
struct constant_value_attribute
{
    U2 constant_value;
};

typedef struct constant_value_attribute constant_value_attribute;

// Information about Exception Attributes
struct exception_attribute
{
    U2 num_exceptions;
    U2 *exception_info;
};

typedef struct exception_attribute exception_attribute;

// Information about Classes
struct classes
{
    U2 inner_name;

    U2 inner_class_info;
    U2 inner_class_access_flags;

    U2 outer_class_info;
};

typedef struct classes classes;

// Information about Inner Classes Attributes
struct inner_classes_attribute
{
    U2 num_classes;
    classes **classes_array;
};

typedef struct inner_classes_attribute inner_classes_attribute;

// Information about Signature Attributes
struct signature_attribute
{
    U2 signature;
};

typedef struct signature_attribute signature_attribute;

// Type Verification Information
struct verification_type_info
{
    // Tag byte for entry type
    U1 tag;

    // Union of verification types
    union
    {
        struct 
        {
            U2 cp;
        } object_variable_info;
        
        struct 
        {
            U2 offset;
        } uninitialized_variable_info;
        
    } type_info;
};

typedef struct verification_type_info verification_type_info;

// Stack Frame Mapping
struct stack_map_frame
{
    // Frame type
    U1 frame_type;

    // Union of frame types
    union 
    {
        struct 
        {
            verification_type_info **stack;
        } same_locals_1_stack_item_frame;

        struct 
        {
            U2 offset_delta;
            verification_type_info **stack;
        } same_locals_1_stack_item_frame_extended;        

        struct 
        {
            U2 offset_delta;
        } chop_frame;

        struct 
        {
            U2 offset_delta;
        } same_frame_extended;
        
        struct 
        {
            U2 offset_delta;
            verification_type_info **locals;
        } append_frame;

        struct 
        {
            U2 offset_delta;
            U2 num_locals;
            U2 num_stack_items;

            verification_type_info **locals;
            verification_type_info ** stack;
        } full_frame;        
    } map_frame_type;   
};

typedef struct stack_map_frame stack_map_frame;

// Information about Stack Map Attributes
struct stack_map_attribute
{
    U2 num_entries;
    stack_map_frame **entries;
};

typedef struct stack_map_attribute stack_map_attribute;

struct ClassFile
{
    U4 magic;
    U2 minor_version;
    U2 major_version;
    U2 constant_pool_count;
    cp_info *constant_pool;
    U2 access_flags;
    U2 this_class;
    U2 super_class;
    U2 interfaces_count;
    U2 *interfaces;
    U2 fields_count;
    field_info *fields;
    U2 methods_count;
    method_info *methods;
    U2 attributes_count;
    attribute_info **attributes;
};


typedef struct ClassFile ClassFile;

#endif
