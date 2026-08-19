#pragma once

#include "../resolve.hpp"

inline void (*vm_set_enabled)(void*, int) = nullptr;

namespace sdk::unity {

inline bool beh_ok = false;

inline bool beh_resolve() {
    if (beh_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Behaviour");
    if (!c) return false;
    vm_set_enabled = sdk::resolve_m<void (*)(void*, int)>(c, "set_enabled", 1);
    beh_ok = vm_set_enabled != nullptr;
    return beh_ok;
}

inline void beh_reset() {
    vm_set_enabled = nullptr;
    beh_ok = false;
}

}
