#pragma once

#include "../resolve.hpp"

inline void* (*chams_getmat)(void*) = nullptr;
inline void* (*chams_getsharedmat)(void*) = nullptr;
inline void (*chams_setmat)(void*, void*) = nullptr;
inline void (*chams_setenabled)(void*, int) = nullptr;

namespace sdk::unity {

inline bool re_ok = false;

inline bool re_resolve() {
    if (re_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Renderer");
    if (!c) return false;
    chams_getmat = sdk::resolve_m<void* (*)(void*)>(c, "get_material", 0);
    chams_getsharedmat = sdk::resolve_m<void* (*)(void*)>(c, "get_sharedMaterial", 0);
    chams_setmat = sdk::resolve_m<void (*)(void*, void*)>(c, "set_material", 1);
    chams_setenabled = sdk::resolve_m<void (*)(void*, int)>(c, "set_enabled", 1);
    re_ok = chams_getmat && chams_setmat && chams_setenabled;
    return re_ok;
}

inline void re_reset() {
    chams_getmat = nullptr;
    chams_getsharedmat = nullptr;
    chams_setmat = nullptr;
    chams_setenabled = nullptr;
    re_ok = false;
}

}
