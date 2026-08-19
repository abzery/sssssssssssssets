#pragma once

#include "../resolve.hpp"

inline void* (*obj_find_objects_of_type)(void*) = nullptr;
inline void* (*type_get_type)(Il2CppString*, int) = nullptr;

namespace sdk::unity {

inline bool obj_ok = false;

inline bool obj_resolve() {
    if (obj_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Object");
    if (!c) return false;
    obj_find_objects_of_type = sdk::resolve_m<void* (*)(void*)>(c, "FindObjectsOfType", 1);
    if (!type_get_type) {
        Il2CppClass* tc = nullptr;
        if (il2cpp::domain_assembly_open) {
            const char* names[] = { "mscorlib.dll", "mscorlib", "System.Private.CoreLib.dll", "System.Runtime.dll" };
            for (int i = 0; i < 4 && !tc; i++)
                tc = sdk::class_lazy(names[i], "System", "Type");
        }
        if (tc) {
            const Il2CppMethod* gm = il2cpp::class_get_method_from_name
                ? il2cpp::class_get_method_from_name(tc, "GetType", 2) : nullptr;
            if (gm) type_get_type = (void* (*)(Il2CppString*, int))sdk::m_ptr(gm);
        }
        if (!type_get_type)
            LOG("sdk: Type.GetType resolve failed");
    }
    obj_ok = obj_find_objects_of_type != nullptr;
    return obj_ok;
}

inline void obj_reset() {
    obj_find_objects_of_type = nullptr;
    type_get_type = nullptr;
    obj_ok = false;
}

}
