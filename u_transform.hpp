#pragma once

#include "../resolve.hpp"

inline Vector3 (*tgf)(void*) = nullptr;
inline Vector3 (*tgr)(void*) = nullptr;
inline void (*tsp)(void*, Vector3) = nullptr;
inline void (*tse)(void*, Vector3) = nullptr;
inline void (*tsl)(void*, Vector3) = nullptr;
inline Vector3 (*glp)(void*) = nullptr;
inline void (*slp)(void*, Vector3) = nullptr;
inline Vector3 (*gls)(void*) = nullptr;
inline void (*sls)(void*, Vector3) = nullptr;

namespace sdk::unity {

inline bool tr_ok = false;

inline bool tr_resolve() {
    if (tr_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Transform");
    if (!c) return false;
    tgf = sdk::resolve_m<Vector3 (*)(void*)>(c, "get_forward", 0);
    tgr = sdk::resolve_m<Vector3 (*)(void*)>(c, "get_right", 0);
    tsp = sdk::resolve_m<void (*)(void*, Vector3)>(c, "set_position", 1);
    tse = sdk::resolve_m<void (*)(void*, Vector3)>(c, "set_eulerAngles", 1);
    tsl = sdk::resolve_m<void (*)(void*, Vector3)>(c, "set_localEulerAngles", 1);
    glp = sdk::resolve_m<Vector3 (*)(void*)>(c, "get_localPosition", 0);
    slp = sdk::resolve_m<void (*)(void*, Vector3)>(c, "set_localPosition", 1);
    gls = sdk::resolve_m<Vector3 (*)(void*)>(c, "get_localScale", 0);
    sls = sdk::resolve_m<void (*)(void*, Vector3)>(c, "set_localScale", 1);
    tr_ok = tgf && tsp;
    return tr_ok;
}

inline void tr_reset() {
    tgf = nullptr;
    tgr = nullptr;
    tsp = nullptr;
    tse = nullptr;
    tsl = nullptr;
    glp = nullptr;
    slp = nullptr;
    gls = nullptr;
    sls = nullptr;
    tr_ok = false;
}

}
