#pragma once

#include "../resolve.hpp"

inline void* (*cgt)(void*) = nullptr;

namespace sdk::unity {

inline bool comp_ok = false;

inline bool comp_resolve() {
    if (comp_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Component");
    if (!c) return false;
    cgt = sdk::resolve_m<void* (*)(void*)>(c, "get_transform", 0);
    comp_ok = cgt != nullptr;
    return comp_ok;
}

inline void comp_reset() {
    cgt = nullptr;
    comp_ok = false;
}

}
