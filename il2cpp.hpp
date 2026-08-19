#pragma once
#include <cstdint>
#include <cstddef>
#include <dlfcn.h>

struct Il2CppDomain;
struct Il2CppImage;
struct Il2CppAssembly;
struct Il2CppClass;
struct Il2CppMethod;
struct Il2CppObject;
struct Il2CppString;
struct Il2CppType;
struct Il2CppField;
struct Il2CppProperty;
struct Il2CppEvent;

struct Il2CppArray {
    uintptr_t klass;       // 0x00
    uintptr_t monitor;     // 0x08
    uintptr_t bounds;      // 0x10
    uintptr_t max_length;  // 0x18
    // data at 0x20
};

struct Il2CppAssemblyName {
    const char* name;
    const char* culture;
    const char* hash_value;
    const char* public_key;
    int32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[8];
};

struct Il2CppImage {
    const char*   name;
    const char*   name_no_ext;
    Il2CppAssembly* assembly;
    void*         type_cache;
    void*         nameToClassHashTable;
    uint32_t      token;
};

struct Il2CppAssembly {
    Il2CppImage*   image;
    Il2CppDomain*  domain;
    int32_t        major_version;
    int32_t        minor_version;
    int32_t        build_number;
    int32_t        revision_number;
    uint8_t        public_key_token[8];
    const char*    name;
    const char*    culture;
    const char*    hash_value;
    const char*    public_key;
    uint32_t       hash_alg;
    uint32_t       hash_len;
    uint32_t       flags;
};

struct Il2CppClass {
    const Il2CppImage* image;
    void*              gc_desc;
    const char*        name;
    const char*        namespaze;
    Il2CppType*        byval_arg;
    Il2CppType*        this_arg;
    Il2CppClass*       element_class;
    Il2CppClass*       declaringType;
    Il2CppClass*       parent;
    void*              generic_class;
    const void*        typeDefinition;
    void*              interopData;
    const Il2CppClass* klass;
    void*              fields;
    void*              events;
    void*              properties;
    const Il2CppMethod** methods;
    Il2CppClass**      nestedTypes;
    Il2CppClass**      implementedInterfaces;
    void*              interfaceOffsets;
    void**             static_fields;
    void*              rgctx_data;
    Il2CppClass**      typeHierarchy;
    void*              unity_user_data;
    const void*        serializationData;
    void*              generic_containers;
    void*              m_cache;
    void*              native_size;
    uint32_t           actualSize;
    uint32_t           element_size;
    int32_t            native_align;
    uint32_t           method_count;
    uint32_t           field_count;
    uint32_t           property_count;
    uint32_t           event_count;
    uint32_t           nested_type_count;
    uint32_t           vtable_count;
    uint32_t           interfaces_count;
    uint32_t           interface_offsets_count;
    uint32_t           genericMethodIndex;
    uint32_t           genericContainerIndex;
    uint32_t           instance_size;
    uint32_t           actualSlotCount;
    uint32_t           token;
    uint16_t           flags;
    uint16_t           iflags;
    uint16_t           prolog;
    uint16_t           cctor_started;
    uint32_t           cctor_method_count;
    uint32_t           thread_static_fields_size;
    int32_t            thread_static_fields_offset;
    uint32_t           flags2;
    uint32_t           module;
    void*              realm;
    void*              native_compiler_info;
    int32_t            rank;
    uint8_t            minimumAlignment;
    uint8_t            naturalAlingment;
    uint8_t            packingSize;
    uint8_t            initialized_and_no_error : 1;
    uint8_t            valuetype : 1;
    uint8_t            initialized : 1;
    uint8_t            enumtype : 1;
    uint8_t            is_generic : 1;
    uint8_t            has_references : 1;
    uint8_t            init_pending : 1;
    uint8_t            size_inited : 1;
    uint8_t            has_finalize : 1;
    uint8_t            has_cctor : 1;
    uint8_t            is_blittable : 1;
    uint8_t            is_import_or_windows_runtime : 1;
    uint8_t            is_vtable_initialized : 1;
    uint8_t            has_initialization_error : 1;
    void*              vtable[32];
};

struct MethodInfo {
    void*         methodPointer;
    void*         invoker_method;
    const char*   name;
    Il2CppClass*  klass;
    const Il2CppType* return_type;
    const Il2CppType* parameters;
    void*         methodDefinition;
    void*         genericContainer;
    uint32_t      methodIndex;
    uint32_t      token;
    uint16_t      flags;
    uint16_t      iflags;
    uint16_t      slot;
    uint16_t      arguments_count;
    uint16_t      generic_method_index;
    uint16_t      is_generic : 1;
    uint16_t      is_inflated : 1;
    uint16_t      wrapper_type : 1;
    uint16_t      is_marshaled_from_native : 1;
    void*         virtualMethodPointers[1];
};

struct Il2CppType {
    void*         data;
    unsigned int  attrs : 16;
    unsigned int  type : 8;
    unsigned int  num_mods : 6;
    unsigned int  byref : 1;
    unsigned int  pinned : 1;
};

struct Il2CppField {
    const char*   name;
    const Il2CppType* type;
    Il2CppClass*  parent;
    int32_t       offset;
    uint32_t      token;
};

constexpr uintptr_t globalMetadata        = 0xB04AC00;
constexpr uintptr_t globalMetadataHeader  = 0xB04AC08;
constexpr uintptr_t typeInfoDefinitionTable = 0xB04AC38;

namespace il2cpp {

inline void* (*resolve_rva)(uint64_t) = nullptr;

inline void* (*alloc)(size_t size);
inline void  (*free)(void* ptr);

inline Il2CppAssembly** (*domain_get_assemblies)(Il2CppDomain* domain, size_t* size);
inline Il2CppDomain*    (*domain_get)();
inline Il2CppAssembly*  (*domain_assembly_open)(Il2CppDomain* domain, const char* name);
inline Il2CppImage*     (*assembly_get_image)(const Il2CppAssembly* assembly);

inline Il2CppClass* (*class_from_name)(const Il2CppImage* image, const char* namespaze, const char* name);
inline Il2CppClass* (*class_from_il2cpp_type)(const Il2CppType* type);
inline Il2CppClass* (*class_from_type)(const Il2CppType* type);
inline const char*  (*class_get_name)(const Il2CppClass* klass);
inline const char*  (*class_get_namespace)(const Il2CppClass* klass);
inline Il2CppClass* (*class_get_parent)(const Il2CppClass* klass);
inline Il2CppClass* (*class_get_element_class)(const Il2CppClass* klass);
inline Il2CppClass* (*class_get_declaring_type)(const Il2CppClass* klass);
inline const Il2CppImage* (*class_get_image)(const Il2CppClass* klass);
inline Il2CppType*  (*class_get_type)(const Il2CppClass* klass);
inline uint32_t     (*class_get_flags)(const Il2CppClass* klass);
inline uint32_t     (*class_instance_size)(const Il2CppClass* klass);
inline int32_t      (*class_get_rank)(const Il2CppClass* klass);
inline uint32_t     (*class_array_element_size)(const Il2CppClass* klass);
inline const Il2CppType* (*class_enum_basetype)(const Il2CppClass* klass);
inline bool         (*class_is_abstract)(const Il2CppClass* klass);
inline bool         (*class_is_interface)(const Il2CppClass* klass);
inline bool         (*class_is_enum)(const Il2CppClass* klass);
inline bool         (*class_is_generic)(const Il2CppClass* klass);
inline bool         (*class_is_valuetype)(const Il2CppClass* klass);
inline bool         (*class_is_blittable)(const Il2CppClass* klass);
inline const char*  (*class_get_assemblyname)(const Il2CppClass* klass);

inline Il2CppClass* (*class_get_nested_types)(const Il2CppClass* klass, void** iter);
inline Il2CppClass* (*class_get_interfaces)(const Il2CppClass* klass, void** iter);
inline Il2CppField* (*class_get_fields)(const Il2CppClass* klass, void** iter);
inline Il2CppField* (*class_get_field_from_name)(const Il2CppClass* klass, const char* name);

inline const Il2CppMethod* (*class_get_methods)(const Il2CppClass* klass, void** iter);
inline const Il2CppMethod* (*class_get_method_from_name)(const Il2CppClass* klass, const char* name, int argc);
inline size_t (*class_get_method_count)(const Il2CppClass* klass);

inline const char*       (*field_get_name)(const Il2CppField* field);
inline const Il2CppType* (*field_get_type)(const Il2CppField* field);
inline Il2CppClass*      (*field_get_parent)(const Il2CppField* field);
inline uint32_t          (*field_get_flags)(const Il2CppField* field);
inline uint32_t          (*field_get_offset)(const Il2CppField* field);
inline void              (*field_get_value)(const Il2CppObject* obj, const Il2CppField* field, void* value);
inline void              (*field_static_get_value)(const Il2CppField* field, void* value);
inline void              (*field_static_set_value)(const Il2CppField* field, void* value);
inline void              (*field_set_value)(const Il2CppObject* obj, const Il2CppField* field, void* value);

inline const char*       (*method_get_name)(const Il2CppMethod* method);
inline Il2CppClass*      (*method_get_class)(const Il2CppMethod* method);
inline Il2CppClass*      (*method_get_declaring_type)(const Il2CppMethod* method);
inline const Il2CppType* (*method_get_return_type)(const Il2CppMethod* method);
inline const Il2CppType* (*method_get_param)(const Il2CppMethod* method, uint32_t index);
inline const char*       (*method_get_param_name)(const Il2CppMethod* method, uint32_t index);
inline uint32_t          (*method_get_param_count)(const Il2CppMethod* method);
inline uint32_t          (*method_get_flags)(const Il2CppMethod* method, uint32_t* iflags);
inline bool              (*method_is_generic)(const Il2CppMethod* method);
inline bool              (*method_is_inflated)(const Il2CppMethod* method);
inline bool              (*method_is_instance)(const Il2CppMethod* method);
inline bool              (*method_has_attribute)(const Il2CppMethod* method, Il2CppClass* attr_class);
inline Il2CppObject*     (*method_get_object)(const Il2CppMethod* method, Il2CppClass* refclass);
inline const Il2CppMethod* (*method_get_from_reflection)(const Il2CppObject* method);

inline Il2CppObject* (*object_new)(const Il2CppClass* klass);
inline Il2CppArray*  (*array_new)(const Il2CppClass* array_class, size_t size);
inline void*         (*object_unbox)(const Il2CppObject* obj);
inline void*         (*value_box)(Il2CppClass* klass, void* data);
inline const Il2CppMethod* (*object_get_virtual_method)(const Il2CppObject* obj, const Il2CppMethod* method);

inline Il2CppString* (*string_new)(const char* str);
inline Il2CppString* (*string_new_len)(const char* str, uint32_t length);
inline Il2CppString* (*string_new_utf16)(const uint16_t* text, int32_t length);
inline Il2CppString* (*string_new_wrapper)(const char* str);
inline const uint16_t* (*string_chars)(const Il2CppString* str);
inline uint32_t      (*string_length)(const Il2CppString* str);

inline Il2CppObject* (*runtime_invoke)(const Il2CppMethod* method, void* obj, void** params, void** exc);
inline void          (*runtime_invoke_convert_args)(const Il2CppMethod* method, void* obj, Il2CppObject** params, int param_count, void** exc);
inline void          (*runtime_object_init_exception)(Il2CppObject* ex, void** exc);
inline void          (*runtime_unhandled_exception_policy_set)(int policy);

inline void* (*thread_attach)(Il2CppDomain* domain);
inline void* (*thread_current)();
inline void  (*thread_detach)(void* thread);

inline const Il2CppType* (*type_get_attrs)(const Il2CppType* type);
inline Il2CppClass*      (*type_get_class_or_element_class)(const Il2CppType* type);
inline const char*       (*type_get_name)(const Il2CppType* type);
inline Il2CppObject*     (*type_get_object)(const Il2CppType* type);
inline const char*       (*type_get_reflection_name)(const Il2CppType* type);
inline int               (*type_get_type)(const Il2CppType* type);
inline bool              (*type_is_byref)(const Il2CppType* type);

inline void  (*init)(const char* domain_name);
inline void  (*shutdown)();
inline void  (*set_commandline_arguments)(int argc, const char* argv[], const char* basedir);
inline void  (*set_config_dir)(const char* configdir);
inline void  (*set_temp_dir)(const char* temp_dir);
inline void  (*set_memory_callbacks)(void* cbs);
inline void* (*get_corlib)();
inline size_t (*memory_pool_get_region_size)(void* pool);
inline void  (*register_log_callback)(void* callback);
inline void* (*resolve_icall)(const char* name);

namespace offset {

constexpr uintptr_t il2cpp_alloc                           = 0x9D7BFB0;
constexpr uintptr_t il2cpp_array_class_get                 = 0x5C86BD8;
constexpr uintptr_t il2cpp_array_length                    = 0x5C86BDC;
constexpr uintptr_t il2cpp_array_new                       = 0x5C86BE0;
constexpr uintptr_t il2cpp_assembly_get_image              = 0x5C86BFC;
constexpr uintptr_t il2cpp_class_array_element_size        = 0x5C86D20;
constexpr uintptr_t il2cpp_class_enum_basetype             = 0x5C86C00;
constexpr uintptr_t il2cpp_class_from_il2cpp_type          = 0x5C86C18;
constexpr uintptr_t il2cpp_class_from_name                 = 0x9D5B510;
constexpr uintptr_t il2cpp_class_from_type                 = 0x5C86D24;
constexpr uintptr_t il2cpp_class_get_assemblyname          = 0x5C86D48;
constexpr uintptr_t il2cpp_class_get_declaring_type        = 0x5C86CC8;
constexpr uintptr_t il2cpp_class_get_element_class         = 0x5C86CA8;
constexpr uintptr_t il2cpp_class_get_field_from_name       = 0x5C86CB4;
constexpr uintptr_t il2cpp_class_get_fields                = 0x5C86CAC;
constexpr uintptr_t il2cpp_class_get_flags                 = 0x5C86CE8;
constexpr uintptr_t il2cpp_class_get_image                 = 0x5C86D44;
constexpr uintptr_t il2cpp_class_get_interfaces            = 0x9D593F8;
constexpr uintptr_t il2cpp_class_get_method_from_name      = 0x9D5959C;
constexpr uintptr_t il2cpp_class_get_methods               = 0x5C86CB8;
constexpr uintptr_t il2cpp_class_get_name                  = 0x5C86CBC;
constexpr uintptr_t il2cpp_class_get_namespace             = 0x5C86CC0;
constexpr uintptr_t il2cpp_class_get_nested_types          = 0x5C86CB0;
constexpr uintptr_t il2cpp_class_get_parent                = 0x5C86CC4;
constexpr uintptr_t il2cpp_class_get_rank                  = 0x5C86D4C;
constexpr uintptr_t il2cpp_class_get_type                  = 0x5C86D2C;
constexpr uintptr_t il2cpp_class_instance_size             = 0x5C86CCC;
constexpr uintptr_t il2cpp_class_is_abstract               = 0x5C86CF0;
constexpr uintptr_t il2cpp_class_is_blittable              = 0x5C86CDC;
constexpr uintptr_t il2cpp_class_is_enum                   = 0x5C86D38;
constexpr uintptr_t il2cpp_class_is_generic                = 0x5C86C08;
constexpr uintptr_t il2cpp_class_is_interface              = 0x5C86CFC;
constexpr uintptr_t il2cpp_class_is_valuetype              = 0x5C86CD0;
constexpr uintptr_t il2cpp_domain_assembly_open            = 0x5C86D58;
constexpr uintptr_t il2cpp_domain_get                      = 0x5C86D54;
constexpr uintptr_t il2cpp_field_get_flags                 = 0x5C86E20;
constexpr uintptr_t il2cpp_field_get_name                  = 0x5C86E1C;
constexpr uintptr_t il2cpp_field_get_offset                = 0x5C86E28;
constexpr uintptr_t il2cpp_field_get_parent                = 0x5C86E24;
constexpr uintptr_t il2cpp_field_get_type                  = 0x5C86E2C;
constexpr uintptr_t il2cpp_field_get_value                 = 0x5C871E8;
constexpr uintptr_t il2cpp_field_static_get_value          = 0x5C87900;
constexpr uintptr_t il2cpp_field_static_set_value          = 0x5C879AC;
constexpr uintptr_t il2cpp_free                            = 0x5C86BD4;
constexpr uintptr_t il2cpp_get_corlib                      = 0x5C86BCC;
constexpr uintptr_t il2cpp_init                            = 0x5C86B74;
constexpr uintptr_t il2cpp_memory_pool_get_region_size     = 0x5C86BC8;
constexpr uintptr_t il2cpp_method_get_class                = 0x5C86EE8;
constexpr uintptr_t il2cpp_method_get_declaring_type       = 0x5C86EF0;
constexpr uintptr_t il2cpp_method_get_from_reflection      = 0x5C86EC8;
constexpr uintptr_t il2cpp_method_get_name                 = 0x5C86ED0;
constexpr uintptr_t il2cpp_method_get_object               = 0x5C86ECC;
constexpr uintptr_t il2cpp_method_get_param                = 0x5C86EE4;
constexpr uintptr_t il2cpp_method_get_param_count          = 0x5C86EE0;
constexpr uintptr_t il2cpp_method_get_param_name           = 0x9D8C26C;
constexpr uintptr_t il2cpp_method_get_return_type          = 0x5C86EC4;
constexpr uintptr_t il2cpp_method_has_attribute            = 0x5C86EEC;
constexpr uintptr_t il2cpp_method_is_generic               = 0x5C86ED4;
constexpr uintptr_t il2cpp_method_is_inflated              = 0x5C86ED8;
constexpr uintptr_t il2cpp_method_is_instance              = 0x5C86EDC;
constexpr uintptr_t il2cpp_object_get_virtual_method       = 0x5C86EF4;
constexpr uintptr_t il2cpp_object_new                      = 0x5C86EF8;
constexpr uintptr_t il2cpp_object_unbox                    = 0x5CAB060;
constexpr uintptr_t il2cpp_register_log_callback           = 0x5C87058;
constexpr uintptr_t il2cpp_resolve_icall                   = 0x5CAAE90;
constexpr uintptr_t il2cpp_runtime_invoke                  = 0x5C86F28;
constexpr uintptr_t il2cpp_runtime_invoke_convert_args     = 0x5C86F24;
constexpr uintptr_t il2cpp_runtime_object_init_exception   = 0x5C86F2C;
constexpr uintptr_t il2cpp_runtime_unhandled_exception_policy_set = 0x5C86F30;
constexpr uintptr_t il2cpp_set_commandline_arguments       = 0x5C86BAC;
constexpr uintptr_t il2cpp_set_config_dir                  = 0x5C86BA4;
constexpr uintptr_t il2cpp_set_memory_callbacks            = 0x5C86BC0;
constexpr uintptr_t il2cpp_set_temp_dir                    = 0x5C86BA8;
constexpr uintptr_t il2cpp_shutdown                        = 0x5C86BA0;
constexpr uintptr_t il2cpp_string_chars                    = 0x5C86F38;
constexpr uintptr_t il2cpp_string_length                   = 0x5C86F34;
constexpr uintptr_t il2cpp_string_new                      = 0x5C86F3C;
constexpr uintptr_t il2cpp_string_new_len                  = 0x5C86F48;
constexpr uintptr_t il2cpp_string_new_utf16                = 0x5C86F44;
constexpr uintptr_t il2cpp_string_new_wrapper              = 0x5C86F40;
constexpr uintptr_t il2cpp_thread_attach                   = 0x5C86F50;
constexpr uintptr_t il2cpp_thread_current                  = 0x5C86F4C;
constexpr uintptr_t il2cpp_thread_detach                   = 0x5C86F54;
constexpr uintptr_t il2cpp_type_get_attrs                  = 0x5C87038;
constexpr uintptr_t il2cpp_type_get_class_or_element_class = 0x5C86F64;
constexpr uintptr_t il2cpp_type_get_name                   = 0x5C86F68;
constexpr uintptr_t il2cpp_type_get_object                 = 0x5C86F5C;
constexpr uintptr_t il2cpp_type_get_reflection_name        = 0x5C86F70;
constexpr uintptr_t il2cpp_type_get_type                   = 0x5C86F60;
constexpr uintptr_t il2cpp_type_is_byref                   = 0x5C8702C;
constexpr uintptr_t il2cpp_value_box                       = 0x5CAB05C;

constexpr size_t il2cpp_class_name          = 0xF8;
constexpr size_t il2cpp_class_namespace     = 0xB0;
constexpr size_t il2cpp_class_parent        = 0x40;
constexpr size_t il2cpp_class_static_fields = 0x90;
constexpr size_t il2cpp_class_vtable        = 0x1B8;

constexpr size_t il2cpp_method_pointer     = 0x8;
constexpr size_t il2cpp_method_virtual_ptr = 0x30;
constexpr size_t il2cpp_method_invoker     = 0x20;
constexpr size_t il2cpp_method_name        = 0x40;

} // namespace offset

inline bool init_api(uintptr_t base) {
    if (!resolve_rva) return false;
    (void)base;

    alloc                = (decltype(alloc))(resolve_rva(offset::il2cpp_alloc));
    free                 = (decltype(free))(resolve_rva(offset::il2cpp_free));

    domain_get           = (decltype(domain_get))(resolve_rva(offset::il2cpp_domain_get));
    domain_assembly_open = (decltype(domain_assembly_open))(resolve_rva(offset::il2cpp_domain_assembly_open));
    assembly_get_image   = (decltype(assembly_get_image))(resolve_rva(offset::il2cpp_assembly_get_image));

    class_from_name           = (decltype(class_from_name))(resolve_rva(offset::il2cpp_class_from_name));
    class_from_il2cpp_type    = (decltype(class_from_il2cpp_type))(resolve_rva(offset::il2cpp_class_from_il2cpp_type));
    class_from_type           = (decltype(class_from_type))(resolve_rva(offset::il2cpp_class_from_type));
    class_get_name            = (decltype(class_get_name))(resolve_rva(offset::il2cpp_class_get_name));
    class_get_namespace       = (decltype(class_get_namespace))(resolve_rva(offset::il2cpp_class_get_namespace));
    class_get_parent          = (decltype(class_get_parent))(resolve_rva(offset::il2cpp_class_get_parent));
    class_get_element_class   = (decltype(class_get_element_class))(resolve_rva(offset::il2cpp_class_get_element_class));
    class_get_declaring_type  = (decltype(class_get_declaring_type))(resolve_rva(offset::il2cpp_class_get_declaring_type));
    class_get_image           = (decltype(class_get_image))(resolve_rva(offset::il2cpp_class_get_image));
    class_get_type            = (decltype(class_get_type))(resolve_rva(offset::il2cpp_class_get_type));
    class_get_flags           = (decltype(class_get_flags))(resolve_rva(offset::il2cpp_class_get_flags));
    class_instance_size       = (decltype(class_instance_size))(resolve_rva(offset::il2cpp_class_instance_size));
    class_get_rank            = (decltype(class_get_rank))(resolve_rva(offset::il2cpp_class_get_rank));
    class_array_element_size  = (decltype(class_array_element_size))(resolve_rva(offset::il2cpp_class_array_element_size));
    class_enum_basetype       = (decltype(class_enum_basetype))(resolve_rva(offset::il2cpp_class_enum_basetype));
    class_is_abstract         = (decltype(class_is_abstract))(resolve_rva(offset::il2cpp_class_is_abstract));
    class_is_interface        = (decltype(class_is_interface))(resolve_rva(offset::il2cpp_class_is_interface));
    class_is_enum             = (decltype(class_is_enum))(resolve_rva(offset::il2cpp_class_is_enum));
    class_is_generic          = (decltype(class_is_generic))(resolve_rva(offset::il2cpp_class_is_generic));
    class_is_valuetype        = (decltype(class_is_valuetype))(resolve_rva(offset::il2cpp_class_is_valuetype));
    class_is_blittable        = (decltype(class_is_blittable))(resolve_rva(offset::il2cpp_class_is_blittable));
    class_get_assemblyname    = (decltype(class_get_assemblyname))(resolve_rva(offset::il2cpp_class_get_assemblyname));
    class_get_nested_types    = (decltype(class_get_nested_types))(resolve_rva(offset::il2cpp_class_get_nested_types));
    class_get_interfaces      = (decltype(class_get_interfaces))(resolve_rva(offset::il2cpp_class_get_interfaces));
    class_get_fields          = (decltype(class_get_fields))(resolve_rva(offset::il2cpp_class_get_fields));
    class_get_field_from_name = (decltype(class_get_field_from_name))(resolve_rva(offset::il2cpp_class_get_field_from_name));
    class_get_methods         = (decltype(class_get_methods))(resolve_rva(offset::il2cpp_class_get_methods));
    class_get_method_from_name = (decltype(class_get_method_from_name))(resolve_rva(offset::il2cpp_class_get_method_from_name));

    field_get_name           = (decltype(field_get_name))(resolve_rva(offset::il2cpp_field_get_name));
    field_get_type           = (decltype(field_get_type))(resolve_rva(offset::il2cpp_field_get_type));
    field_get_parent         = (decltype(field_get_parent))(resolve_rva(offset::il2cpp_field_get_parent));
    field_get_flags          = (decltype(field_get_flags))(resolve_rva(offset::il2cpp_field_get_flags));
    field_get_offset         = (decltype(field_get_offset))(resolve_rva(offset::il2cpp_field_get_offset));
    field_get_value          = (decltype(field_get_value))(resolve_rva(offset::il2cpp_field_get_value));
    field_static_get_value   = (decltype(field_static_get_value))(resolve_rva(offset::il2cpp_field_static_get_value));
    field_static_set_value   = (decltype(field_static_set_value))(resolve_rva(offset::il2cpp_field_static_set_value));
    field_set_value          = (decltype(field_set_value))(resolve_rva(offset::il2cpp_field_get_value));

    method_get_name            = (decltype(method_get_name))(resolve_rva(offset::il2cpp_method_get_name));
    method_get_class           = (decltype(method_get_class))(resolve_rva(offset::il2cpp_method_get_class));
    method_get_declaring_type  = (decltype(method_get_declaring_type))(resolve_rva(offset::il2cpp_method_get_declaring_type));
    method_get_return_type     = (decltype(method_get_return_type))(resolve_rva(offset::il2cpp_method_get_return_type));
    method_get_param           = (decltype(method_get_param))(resolve_rva(offset::il2cpp_method_get_param));
    method_get_param_name      = (decltype(method_get_param_name))(resolve_rva(offset::il2cpp_method_get_param_name));
    method_get_param_count     = (decltype(method_get_param_count))(resolve_rva(offset::il2cpp_method_get_param_count));
    method_is_generic          = (decltype(method_is_generic))(resolve_rva(offset::il2cpp_method_is_generic));
    method_is_inflated         = (decltype(method_is_inflated))(resolve_rva(offset::il2cpp_method_is_inflated));
    method_is_instance         = (decltype(method_is_instance))(resolve_rva(offset::il2cpp_method_is_instance));
    method_has_attribute       = (decltype(method_has_attribute))(resolve_rva(offset::il2cpp_method_has_attribute));
    method_get_object          = (decltype(method_get_object))(resolve_rva(offset::il2cpp_method_get_object));
    method_get_from_reflection = (decltype(method_get_from_reflection))(resolve_rva(offset::il2cpp_method_get_from_reflection));

    object_new                = (decltype(object_new))(resolve_rva(offset::il2cpp_object_new));
    array_new                 = (decltype(array_new))(resolve_rva(offset::il2cpp_array_new));
    object_unbox              = (decltype(object_unbox))(resolve_rva(offset::il2cpp_object_unbox));
    value_box                 = (decltype(value_box))(resolve_rva(offset::il2cpp_value_box));
    object_get_virtual_method = (decltype(object_get_virtual_method))(resolve_rva(offset::il2cpp_object_get_virtual_method));

    string_new         = (decltype(string_new))(resolve_rva(offset::il2cpp_string_new));
    string_new_len     = (decltype(string_new_len))(resolve_rva(offset::il2cpp_string_new_len));
    string_new_utf16   = (decltype(string_new_utf16))(resolve_rva(offset::il2cpp_string_new_utf16));
    string_new_wrapper = (decltype(string_new_wrapper))(resolve_rva(offset::il2cpp_string_new_wrapper));
    string_chars       = (decltype(string_chars))(resolve_rva(offset::il2cpp_string_chars));
    string_length      = (decltype(string_length))(resolve_rva(offset::il2cpp_string_length));

    runtime_invoke                      = (decltype(runtime_invoke))(resolve_rva(offset::il2cpp_runtime_invoke));
    runtime_invoke_convert_args         = (decltype(runtime_invoke_convert_args))(resolve_rva(offset::il2cpp_runtime_invoke_convert_args));
    runtime_object_init_exception       = (decltype(runtime_object_init_exception))(resolve_rva(offset::il2cpp_runtime_object_init_exception));
    runtime_unhandled_exception_policy_set = (decltype(runtime_unhandled_exception_policy_set))(resolve_rva(offset::il2cpp_runtime_unhandled_exception_policy_set));

    thread_attach   = (decltype(thread_attach))(resolve_rva(offset::il2cpp_thread_attach));
    thread_current  = (decltype(thread_current))(resolve_rva(offset::il2cpp_thread_current));
    thread_detach   = (decltype(thread_detach))(resolve_rva(offset::il2cpp_thread_detach));

    type_get_attrs                 = (decltype(type_get_attrs))(resolve_rva(offset::il2cpp_type_get_attrs));
    type_get_class_or_element_class = (decltype(type_get_class_or_element_class))(resolve_rva(offset::il2cpp_type_get_class_or_element_class));
    type_get_name                  = (decltype(type_get_name))(resolve_rva(offset::il2cpp_type_get_name));
    type_get_object                = (decltype(type_get_object))(resolve_rva(offset::il2cpp_type_get_object));
    type_get_reflection_name       = (decltype(type_get_reflection_name))(resolve_rva(offset::il2cpp_type_get_reflection_name));
    type_get_type                  = (decltype(type_get_type))(resolve_rva(offset::il2cpp_type_get_type));
    type_is_byref                  = (decltype(type_is_byref))(resolve_rva(offset::il2cpp_type_is_byref));

    init                           = (decltype(init))(resolve_rva(offset::il2cpp_init));
    shutdown                       = (decltype(shutdown))(resolve_rva(offset::il2cpp_shutdown));
    set_commandline_arguments      = (decltype(set_commandline_arguments))(resolve_rva(offset::il2cpp_set_commandline_arguments));
    set_config_dir                 = (decltype(set_config_dir))(resolve_rva(offset::il2cpp_set_config_dir));
    set_temp_dir                   = (decltype(set_temp_dir))(resolve_rva(offset::il2cpp_set_temp_dir));
    set_memory_callbacks           = (decltype(set_memory_callbacks))(resolve_rva(offset::il2cpp_set_memory_callbacks));
    get_corlib                     = (decltype(get_corlib))(resolve_rva(offset::il2cpp_get_corlib));
    resolve_icall                  = (decltype(resolve_icall))(resolve_rva(offset::il2cpp_resolve_icall));
    register_log_callback          = (decltype(register_log_callback))(resolve_rva(offset::il2cpp_register_log_callback));

    return true;
}

inline Il2CppAssembly** get_assemblies(size_t* count) {
    return domain_get_assemblies(domain_get(), count);
}

inline const Il2CppClass* image_get_class(const Il2CppImage* image, size_t index) {
    (void)image; (void)index;
    return nullptr;
}

inline uint32_t image_get_class_count(const Il2CppImage* image) {
    return *(uint32_t*)((uintptr_t)image + 0xC);
}

inline uint32_t method_get_flags_raw(const Il2CppMethod* method, uint32_t* iflags) {
    if (iflags) {
        *iflags = *(uint16_t*)((uintptr_t)method + 0x14);
    }
    return *(uint16_t*)((uintptr_t)method + 0x12);
}

} // namespace il2cpp
