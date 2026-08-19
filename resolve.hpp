#pragma once

#include "../il2cpp.hpp"
#include "../core.hpp"

struct NC4 { float r, g, b, a; };

namespace sdk {

inline const Il2CppMethod* m_by_name(Il2CppClass* c, const char* name, int argc) {
    if (!c || !il2cpp::class_get_method_from_name) return nullptr;
    const Il2CppMethod* m = il2cpp::class_get_method_from_name(c, name, argc);
    if (!m) m = find_method(c, name);
    return m;
}

inline void* m_ptr(const Il2CppMethod* m) {
    if (!m) return nullptr;
    void* p = *(void**)((uintptr_t)m + il2cpp::offset::il2cpp_method_pointer);
    if (p && (uintptr_t)p > 0x10000) return p;
    return nullptr;
}

template <typename T>
inline T resolve_m(Il2CppClass* c, const char* name, int argc) {
    return (T)m_ptr(m_by_name(c, name, argc));
}

struct class_cache_entry {
    const char* asm_name;
    const char* ns;
    const char* name;
    Il2CppClass* cls;
};

inline class_cache_entry g_class_cache[24];
inline int g_class_cache_n = 0;

inline Il2CppClass* class_lazy(const char* asm_name, const char* ns, const char* name) {
    for (int i = 0; i < g_class_cache_n; i++) {
        if (g_class_cache[i].asm_name == asm_name &&
            g_class_cache[i].ns == ns &&
            g_class_cache[i].name == name)
            return g_class_cache[i].cls;
    }
    Il2CppClass* c = nullptr;
    if (il2cpp::domain_get && il2cpp::domain_assembly_open &&
        il2cpp::assembly_get_image && il2cpp::class_from_name) {
        Il2CppDomain* d = il2cpp::domain_get();
        if (d) {
            if (il2cpp::thread_attach) il2cpp::thread_attach(d);
            Il2CppAssembly* a = il2cpp::domain_assembly_open(d, asm_name);
            if (a) {
                Il2CppImage* im = il2cpp::assembly_get_image(a);
                if (im) c = il2cpp::class_from_name(im, ns, name);
            }
        }
    }
    if (g_class_cache_n < 24 && c) {
        g_class_cache[g_class_cache_n].asm_name = asm_name;
        g_class_cache[g_class_cache_n].ns = ns;
        g_class_cache[g_class_cache_n].name = name;
        g_class_cache[g_class_cache_n].cls = c;
        g_class_cache_n++;
    }
    return c;
}

inline void class_cache_reset() {
    for (int i = 0; i < g_class_cache_n; i++) g_class_cache[i].cls = nullptr;
    g_class_cache_n = 0;
}

inline Il2CppClass* class_lazy_any(const char* ns, const char* name) {
    if (!il2cpp::domain_get || !il2cpp::domain_assembly_open || !il2cpp::assembly_get_image ||
        !il2cpp::class_from_name)
        return nullptr;
    Il2CppDomain* d = il2cpp::domain_get();
    if (!d) return nullptr;
    if (il2cpp::thread_attach) il2cpp::thread_attach(d);
    static const char* mods[] = {
        "UnityEngine.CoreModule", "UnityEngine.ImageConversionModule",
        "UnityEngine.TextureModule", "UnityEngine.AssetBundleModule",
        "UnityEngine.IMGUIModule", "UnityEngine.UIModule", "UnityEngine.AnimationModule",
        "UnityEngine.AudioModule", "UnityEngine.PhysicsModule", "UnityEngine.ParticleSystemModule",
        "UnityEngine.InputLegacyModule", "UnityEngine.InputModule", "UnityEngine.ScreenCaptureModule",
        "UnityEngine.TextRenderingModule", "UnityEngine.TerrainModule", "UnityEngine.VideoModule",
        "UnityEngine.DirectorModule", "UnityEngine.GridModule", "UnityEngine.Physics2DModule",
        "UnityEngine.SubsystemsModule", "UnityEngine.SpriteMaskModule", "UnityEngine.SpriteShapeModule",
        "UnityEngine.TilemapModule", "UnityEngine.UnityWebRequestModule",
        "UnityEngine.UnityWebRequestTextureModule", "UnityEngine.UnityWebRequestAssetBundleModule",
        "UnityEngine.XRModule", "UnityEngine.VRModule", "UnityEngine.VFXModule",
        "UnityEngine.WindModule", "UnityEngine.StreamingModule", "UnityEngine.VehiclesModule",
        "UnityEngine.ProfilerModule", "UnityEngine.JSONSerializeModule",
        "UnityEngine.SharedInternalsModule", "UnityEngine.CoreModule.dll",
        "UnityEngine.ImageConversionModule.dll", "UnityEngine.TextureModule.dll"
    };
    for (size_t i = 0; i < sizeof(mods) / sizeof(mods[0]); i++) {
        Il2CppAssembly* a = il2cpp::domain_assembly_open(d, mods[i]);
        if (!a) continue;
        Il2CppImage* im = il2cpp::assembly_get_image(a);
        if (!im) continue;
        Il2CppClass* c = il2cpp::class_from_name(im, ns, name);
        if (c) return c;
    }
    return nullptr;
}

}
