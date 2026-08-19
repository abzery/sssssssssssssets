#pragma once

#include "../resolve.hpp"

inline float (*tdt)() = nullptr;

namespace sdk::unity {

inline bool time_ok = false;

inline bool time_resolve() {
    if (time_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Time");
    if (!c) return false;
    tdt = sdk::resolve_m<float (*)()>(c, "get_deltaTime", 0);
    time_ok = tdt != nullptr;
    return time_ok;
}

inline void time_reset() {
    tdt = nullptr;
    time_ok = false;
}

}
