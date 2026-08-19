#pragma once

#include "../resolve.hpp"

inline void (*rs_set_ambient_mode)(int) = nullptr;
inline void (*rs_set_ambient_light)(NC4) = nullptr;
inline NC4 (*rs_get_ambient_light)() = nullptr;
inline int (*rs_get_ambient_mode)() = nullptr;
inline void* (*rs_get_sun)() = nullptr;
inline void* (*rs_get_skybox)() = nullptr;
inline void (*rs_set_skybox)(void*) = nullptr;
inline void (*rs_set_fog)(int) = nullptr;
inline void (*rs_set_fog_mode)(int) = nullptr;
inline void (*rs_set_fog_color)(NC4) = nullptr;
inline void (*rs_set_fog_density)(float) = nullptr;
inline int (*rs_get_fog)() = nullptr;
inline NC4 (*rs_get_fog_color)() = nullptr;
inline float (*rs_get_fog_density)() = nullptr;
inline void (*rs_set_fog_start)(float) = nullptr;
inline void (*rs_set_fog_end)(float) = nullptr;
inline float (*rs_get_fog_start)() = nullptr;
inline float (*rs_get_fog_end)() = nullptr;

namespace sdk::unity {

inline bool rset_ok = false;
inline bool rset_fog_ok = false;
inline bool rset_has_sun = false;
inline bool rset_has_fog = false;

inline bool rset_resolve() {
    if (rset_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "RenderSettings");
    if (!c) return false;
    rs_set_ambient_mode = sdk::resolve_m<void (*)(int)>(c, "set_ambientMode", 1);
    rs_set_ambient_light = sdk::resolve_m<void (*)(NC4)>(c, "set_ambientLight", 1);
    rs_get_ambient_light = sdk::resolve_m<NC4 (*)()>(c, "get_ambientLight", 0);
    rs_get_ambient_mode = sdk::resolve_m<int (*)()>(c, "get_ambientMode", 0);
    rs_get_sun = sdk::resolve_m<void* (*)()>(c, "get_sun", 0);
    rs_get_skybox = sdk::resolve_m<void* (*)()>(c, "get_skybox", 0);
    rs_set_skybox = sdk::resolve_m<void (*)(void*)>(c, "set_skybox", 1);
    rs_set_fog = sdk::resolve_m<void (*)(int)>(c, "set_fog", 1);
    rs_set_fog_mode = sdk::resolve_m<void (*)(int)>(c, "set_fogMode", 1);
    rs_set_fog_color = sdk::resolve_m<void (*)(NC4)>(c, "set_fogColor", 1);
    rs_set_fog_density = sdk::resolve_m<void (*)(float)>(c, "set_fogDensity", 1);
    rs_get_fog = sdk::resolve_m<int (*)()>(c, "get_fog", 0);
    rs_get_fog_color = sdk::resolve_m<NC4 (*)()>(c, "get_fogColor", 0);
    rs_get_fog_density = sdk::resolve_m<float (*)()>(c, "get_fogDensity", 0);
    rs_set_fog_start = sdk::resolve_m<void (*)(float)>(c, "set_fogStartDistance", 1);
    rs_set_fog_end = sdk::resolve_m<void (*)(float)>(c, "set_fogEndDistance", 1);
    rs_get_fog_start = sdk::resolve_m<float (*)()>(c, "get_fogStartDistance", 0);
    rs_get_fog_end = sdk::resolve_m<float (*)()>(c, "get_fogEndDistance", 0);
    static bool rlog = false;
    if (!rlog) {
        rlog = true;
        LOG("rset: c=%p amode=%p alight=%p gamlight=%p gamode=%p sun=%p fog=%p",
            (void*)c, (void*)rs_set_ambient_mode, (void*)rs_set_ambient_light,
            (void*)rs_get_ambient_light, (void*)rs_get_ambient_mode,
            (void*)rs_get_sun, (void*)rs_set_fog);
        if (il2cpp::class_get_methods && il2cpp::method_get_name) {
            void* it = nullptr;
            int n = 0;
            while (const Il2CppMethod* m = il2cpp::class_get_methods(c, &it)) {
                const char* nm = il2cpp::method_get_name(m);
                if (!nm) continue;
                if (strstr(nm, "mbient") || strstr(nm, "og") || strstr(nm, "un") ||
                    strstr(nm, "ight") || strstr(nm, "ensity")) {
                    LOG("rset m: %s argc=%d", nm,
                        il2cpp::method_get_param_count ? il2cpp::method_get_param_count(m) : -1);
                    if (++n >= 40) break;
                }
            }
            LOG("rset dump done count=%d", n);
        }
    }
    rset_has_sun = rs_get_sun != nullptr;
    rset_has_fog = rs_set_fog && rs_set_fog_color && rs_set_fog_density && rs_get_fog && rs_get_fog_color && rs_get_fog_density;
    rset_fog_ok = rs_set_fog && rs_set_fog_mode && rs_set_fog_color &&
                  rs_set_fog_start && rs_set_fog_end &&
                  rs_get_fog && rs_get_fog_color && rs_get_fog_start && rs_get_fog_end;
    rset_ok = (rs_set_ambient_mode && rs_set_ambient_light && rs_get_ambient_light && rs_get_ambient_mode) ||
              (rs_get_skybox && rs_set_skybox);
    return rset_ok;
}

inline void rset_reset() {
    rs_set_ambient_mode = nullptr;
    rs_set_ambient_light = nullptr;
    rs_get_ambient_light = nullptr;
    rs_get_ambient_mode = nullptr;
    rs_get_sun = nullptr;
    rs_get_skybox = nullptr;
    rs_set_skybox = nullptr;
    rs_set_fog = nullptr;
    rs_set_fog_mode = nullptr;
    rs_set_fog_color = nullptr;
    rs_set_fog_density = nullptr;
    rs_get_fog = nullptr;
    rs_get_fog_color = nullptr;
    rs_get_fog_density = nullptr;
    rs_set_fog_start = nullptr;
    rs_set_fog_end = nullptr;
    rs_get_fog_start = nullptr;
    rs_get_fog_end = nullptr;
    rset_ok = false;
    rset_fog_ok = false;
    rset_has_sun = false;
    rset_has_fog = false;
}

}
