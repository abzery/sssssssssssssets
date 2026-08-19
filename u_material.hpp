#pragma once

#include "../resolve.hpp"

struct Col4 { float r, g, b, a; };

inline void* (*chams_getshader)(void*) = nullptr;
inline void (*chams_setshader)(void*, void*) = nullptr;
inline void (*chams_setcolor)(void*, Il2CppString*, Col4) = nullptr;
inline void (*chams_setfloat)(void*, Il2CppString*, float) = nullptr;
inline void (*chams_setcolor_id)(void*, int, Col4) = nullptr;
inline void (*chams_setfloat_id)(void*, int, float) = nullptr;
inline int (*shader_property_to_id)(void*) = nullptr;
inline int chams_color_id = 0;
inline int chams_metal_id = 0;
inline int chams_gloss_id = 0;
inline void* (*chams_gettex)(void*) = nullptr;
inline void (*chams_settex)(void*, void*) = nullptr;
inline void (*chams_settexture)(void*, Il2CppString*, void*) = nullptr;
inline void (*mat_ctor_shader)(void*, void*) = nullptr;

namespace sdk::unity {

inline bool mat_ok = false;

static const Il2CppMethod* find_method_by_p0a(Il2CppClass* c, const char* name, const Il2CppType* p0, int argc) {
    const char* want = (il2cpp::type_get_name && p0) ? il2cpp::type_get_name(p0) : nullptr;
    void* it = nullptr;
    while (const Il2CppMethod* m = il2cpp::class_get_methods(c, &it)) {
        const char* nm = il2cpp::method_get_name(m);
        if (!nm || strcmp(nm, name) != 0) continue;
        if (il2cpp::method_get_param_count(m) != argc) continue;
        const Il2CppType* p = il2cpp::method_get_param(m, 0);
        if (p && p == p0) return m;
        if (want && il2cpp::type_get_name && p) {
            const char* have = il2cpp::type_get_name(p);
            if (have && strcmp(have, want) == 0) return m;
        }
    }
    return nullptr;
}

static const Il2CppMethod* find_method_by_p0(Il2CppClass* c, const char* name, const Il2CppType* p0) {
    return find_method_by_p0a(c, name, p0, 2);
}

static const Il2CppMethod* find_method_by_i32(Il2CppClass* c, const char* name, int argc) {
    void* it = nullptr;
    while (const Il2CppMethod* m = il2cpp::class_get_methods(c, &it)) {
        const char* nm = il2cpp::method_get_name(m);
        if (!nm || strcmp(nm, name) != 0) continue;
        if (il2cpp::method_get_param_count(m) != argc) continue;
        const Il2CppType* p = il2cpp::method_get_param(m, 0);
        if (p) {
            const char* tn = il2cpp::type_get_name(p);
            if (tn && strcmp(tn, "System.Int32") == 0) return m;
        }
    }
    return nullptr;
}

inline bool mat_resolve() {
    if (mat_ok) return true;
    Il2CppClass* c = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Material");
    if (!c) return false;
    chams_getshader = sdk::resolve_m<void* (*)(void*)>(c, "get_shader", 0);
    chams_setshader = sdk::resolve_m<void (*)(void*, void*)>(c, "set_shader", 1);
    const Il2CppMethod* mt0 = sdk::m_by_name(c, "get_mainTexture", 0);
    chams_gettex = sdk::resolve_m<void* (*)(void*)>(c, "get_mainTexture", 0);
    chams_settex = sdk::resolve_m<void (*)(void*, void*)>(c, "set_mainTexture", 1);
    if (!mt0 || !chams_gettex) chams_gettex = nullptr;
    if (!chams_gettex || !chams_settex) {
        static bool ml = false;
        if (!ml) {
            ml = true;
            LOG("sdk: mainTexture fns missing gt=%p st=%p", chams_gettex, chams_settex);
        }
    }
    Il2CppClass* sh = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Shader");
    const Il2CppType* strt = nullptr;
    if (sh) {
        const Il2CppMethod* findm = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(sh, "Find", 1) : nullptr;
        if (!findm) findm = find_method(sh, "Find");
        if (findm && il2cpp::method_get_param) strt = il2cpp::method_get_param(findm, 0);
    }
    if (sh && sh->byval_arg) {
        const Il2CppMethod* mctor = find_method_by_p0a(c, ".ctor", sh->byval_arg, 1);
        if (mctor) mat_ctor_shader = (void (*)(void*, void*))sdk::m_ptr(mctor);
    }
    if (!mat_ctor_shader && il2cpp::class_get_methods && il2cpp::method_get_name &&
        il2cpp::method_get_param_count && il2cpp::method_get_param && il2cpp::type_get_name) {
        void* it = nullptr;
        while (const Il2CppMethod* m = il2cpp::class_get_methods(c, &it)) {
            const char* nm = il2cpp::method_get_name(m);
            if (!nm || strcmp(nm, ".ctor") != 0) continue;
            if (il2cpp::method_get_param_count(m) != 1) continue;
            const Il2CppType* p = il2cpp::method_get_param(m, 0);
            const char* pn = p ? il2cpp::type_get_name(p) : nullptr;
            if (pn && strstr(pn, "Shader") != nullptr) {
                mat_ctor_shader = (void (*)(void*, void*))sdk::m_ptr(m);
                break;
            }
        }
    }
    const Il2CppMethod* sc = strt ? find_method_by_p0(c, "SetColor", strt) : nullptr;
    const Il2CppMethod* sf = strt ? find_method_by_p0(c, "SetFloat", strt) : nullptr;
    const Il2CppMethod* stx = strt ? find_method_by_p0(c, "SetTexture", strt) : nullptr;
    if (!sc) sc = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(c, "SetColor", 2) : nullptr;
    if (!sf) sf = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(c, "SetFloat", 2) : nullptr;
    if (sc) chams_setcolor = (void (*)(void*, Il2CppString*, Col4))sdk::m_ptr(sc);
    if (sf) chams_setfloat = (void (*)(void*, Il2CppString*, float))sdk::m_ptr(sf);
    if (stx) chams_settexture = (void (*)(void*, Il2CppString*, void*))sdk::m_ptr(stx);
    const Il2CppMethod* sci = find_method_by_i32(c, "SetColor", 2);
    const Il2CppMethod* sfi = find_method_by_i32(c, "SetFloat", 2);
    if (sci) chams_setcolor_id = (void (*)(void*, int, Col4))sdk::m_ptr(sci);
    if (sfi) chams_setfloat_id = (void (*)(void*, int, float))sdk::m_ptr(sfi);
    if (sh) {
        const Il2CppMethod* pti = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(sh, "PropertyToID", 1) : nullptr;
        if (!pti) pti = find_method(sh, "PropertyToID");
        if (pti) shader_property_to_id = (int (*)(void*))sdk::m_ptr(pti);
    }
    LOG("sdk: mat setcolor str=%p id=%p setfloat str=%p id=%p ptoid=%p",
        (void*)chams_setcolor, (void*)chams_setcolor_id, (void*)chams_setfloat,
        (void*)chams_setfloat_id, (void*)shader_property_to_id);
    mat_ok = chams_getshader && chams_setshader && (chams_setcolor || chams_setcolor_id) &&
             (chams_setfloat || chams_setfloat_id);
    return mat_ok;
}

inline void mat_reset() {
    chams_getshader = nullptr;
    chams_setshader = nullptr;
    chams_setcolor = nullptr;
    chams_setfloat = nullptr;
    chams_setcolor_id = nullptr;
    chams_setfloat_id = nullptr;
    shader_property_to_id = nullptr;
    chams_color_id = 0;
    chams_metal_id = 0;
    chams_gloss_id = 0;
    chams_gettex = nullptr;
    chams_settex = nullptr;
    chams_settexture = nullptr;
    mat_ctor_shader = nullptr;
    mat_ok = false;
}

}
