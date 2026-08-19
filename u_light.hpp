#pragma once

#include "../resolve.hpp"

inline float (*light_get_intensity)(void*) = nullptr;
inline void (*light_set_intensity)(void*, float) = nullptr;
inline int (*light_get_type)(void*) = nullptr;

namespace sdk::unity {

inline bool light_ok = false;

inline bool light_resolve() {
    if (light_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Light");
    if (!c) return false;
    light_get_intensity = sdk::resolve_m<float (*)(void*)>(c, "get_intensity", 0);
    light_set_intensity = sdk::resolve_m<void (*)(void*, float)>(c, "set_intensity", 1);
    light_get_type = sdk::resolve_m<int (*)(void*)>(c, "get_type", 0);
    light_ok = light_get_intensity && light_set_intensity;
    return light_ok;
}

inline void light_reset() {
    light_get_intensity = nullptr;
    light_set_intensity = nullptr;
    light_get_type = nullptr;
    light_ok = false;
}

}
