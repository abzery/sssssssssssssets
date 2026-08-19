#pragma once

#include "../resolve.hpp"

inline void* (*chams_find)(Il2CppString*) = nullptr;
inline Il2CppString* (*chams_shadername)(void*) = nullptr;

namespace sdk::unity {

inline bool sh_ok = false;

inline bool sh_resolve() {
    if (sh_ok) return true;
    Il2CppClass* c = sdk::class_lazy_any("UnityEngine", "Shader");
    if (!c) c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Shader");
    if (!c) return false;
    chams_find = sdk::resolve_m<void* (*)(Il2CppString*)>(c, "Find", 1);
    chams_shadername = sdk::resolve_m<Il2CppString* (*)(void*)>(c, "get_name", 0);
    sh_ok = chams_find != nullptr;
    return sh_ok;
}

inline void sh_reset() {
    chams_find = nullptr;
    chams_shadername = nullptr;
    sh_ok = false;
}

}
