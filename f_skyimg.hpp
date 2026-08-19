#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"
#include "../assets/sky_img.hpp"

namespace skyimg {

inline bool tried_res = false;
inline bool ok_res = false;
inline bool loaded = false;
inline bool applied = false;
inline bool cam_cap = false;
inline int orig_clear = 2;
inline void* s_cam_prev = nullptr;

inline void* our_tex = nullptr;
inline void* our_mat = nullptr;
inline void* orig_skybox = nullptr;
inline bool in_place = false;
inline void* orig_shader = nullptr;
inline void* orig_tex = nullptr;

inline Il2CppString* s_pano_str = nullptr;
inline Il2CppString* s_6side_str = nullptr;
inline Il2CppString* s_unlit_str = nullptr;
inline Il2CppString* s_sprite_str = nullptr;
inline Il2CppString* s_diff_str = nullptr;
inline Il2CppString* s_col_str = nullptr;
inline Il2CppString* s_ft = nullptr;
inline Il2CppString* s_bt = nullptr;
inline Il2CppString* s_lt = nullptr;
inline Il2CppString* s_rt = nullptr;
inline Il2CppString* s_ut = nullptr;
inline Il2CppString* s_dt = nullptr;
inline void* our_shader = nullptr;
inline bool our_shader_6side = false;
inline bool tried_shader = false;

inline Il2CppClass* t2d_cls = nullptr;
inline Il2CppClass* ic_cls = nullptr;
inline Il2CppClass* mat_cls = nullptr;
inline Il2CppClass* byte_cls = nullptr;

inline void (*t2d_ctor)(void*, int, int) = nullptr;
inline int (*t2d_width)(void*) = nullptr;
inline int (*t2d_height)(void*) = nullptr;
inline bool (*ic_loadimage)(void*, void*) = nullptr;
inline bool (*ic_loadimage3)(void*, void*, int) = nullptr;
inline const Il2CppMethod* ic_loadimage_m = nullptr;

static void ensure_shader() {
    if (tried_shader) return;
    tried_shader = true;
    if (il2cpp::string_new) {
        s_pano_str = il2cpp::string_new("Skybox/Panoramic");
        s_6side_str = il2cpp::string_new("Skybox/6 Sided");
        s_unlit_str = il2cpp::string_new("Unlit/Texture");
        s_sprite_str = il2cpp::string_new("Sprites/Default");
        s_diff_str = il2cpp::string_new("Legacy Shaders/Diffuse");
        s_col_str = il2cpp::string_new("_Color");
        s_ft = il2cpp::string_new("_FrontTex");
        s_bt = il2cpp::string_new("_BackTex");
        s_lt = il2cpp::string_new("_LeftTex");
        s_rt = il2cpp::string_new("_RightTex");
        s_ut = il2cpp::string_new("_UpTex");
        s_dt = il2cpp::string_new("_DownTex");
    }
    void* pano = chams_find ? chams_find(s_pano_str) : nullptr;
    void* s6 = chams_find ? chams_find(s_6side_str) : nullptr;
    void* uni = chams_find ? chams_find(s_unlit_str) : nullptr;
    void* spr = chams_find ? chams_find(s_sprite_str) : nullptr;
    void* dif = chams_find ? chams_find(s_diff_str) : nullptr;
    LOG("skyimg: finds pano=%p 6side=%p unlit=%p sprite=%p diff=%p", pano, s6, uni, spr, dif);
    our_shader = pano ? pano : s6 ? s6 : uni ? uni : spr ? spr : dif;
    our_shader_6side = (our_shader == s6);
    if (chams_shadername) {
        char a[64] = {0}, b[64] = {0}, c[64] = {0}, d[64] = {0}, e[64] = {0};
        if (pano) read_str((uint64_t)chams_shadername(pano), a, sizeof(a));
        if (s6) read_str((uint64_t)chams_shadername(s6), b, sizeof(b));
        if (uni) read_str((uint64_t)chams_shadername(uni), c, sizeof(c));
        if (spr) read_str((uint64_t)chams_shadername(spr), d, sizeof(d));
        if (dif) read_str((uint64_t)chams_shadername(dif), e, sizeof(e));
        LOG("skyimg: shaders pano='%s' 6side='%s' unlit='%s' sprite='%s' diff='%s' use6=%d",
            a, b, c, d, e, (int)our_shader_6side);
    }
}

static void set_tex_on(void* mat) {
    if (our_shader_6side && chams_settexture && s_ft && s_bt && s_lt && s_rt && s_ut && s_dt) {
        chams_settexture(mat, s_ft, our_tex);
        chams_settexture(mat, s_bt, our_tex);
        chams_settexture(mat, s_lt, our_tex);
        chams_settexture(mat, s_rt, our_tex);
        chams_settexture(mat, s_ut, our_tex);
        chams_settexture(mat, s_dt, our_tex);
    } else if (chams_settex) {
        chams_settex(mat, our_tex);
    }
}

static bool resolve() {
    if (tried_res) return ok_res;
    tried_res = true;
    if (!sdk::unity::rset_ok) sdk::unity::rset_resolve();
    if (!sdk::unity::mat_ok) sdk::unity::mat_resolve();
    if (!sdk::unity::sh_ok) sdk::unity::sh_resolve();
    t2d_cls = sdk::class_lazy_any("UnityEngine", "Texture2D");
    ic_cls = sdk::class_lazy_any("UnityEngine", "ImageConversion");
    mat_cls = sdk::class_lazy_any("UnityEngine", "Material");
    if (!t2d_cls) t2d_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Texture2D");
    if (!ic_cls) ic_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "ImageConversion");
    if (!mat_cls) mat_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Material");
    if (t2d_cls) {
        t2d_ctor = sdk::resolve_m<void (*)(void*, int, int)>(t2d_cls, ".ctor", 2);
        t2d_width = sdk::resolve_m<int (*)(void*)>(t2d_cls, "get_width", 0);
        t2d_height = sdk::resolve_m<int (*)(void*)>(t2d_cls, "get_height", 0);
    }
    if (ic_cls) {
        ic_loadimage = sdk::resolve_m<bool (*)(void*, void*)>(ic_cls, "LoadImage", 2);
        ic_loadimage3 = sdk::resolve_m<bool (*)(void*, void*, int)>(ic_cls, "LoadImage", 3);
        if (!ic_loadimage) {
            ic_loadimage_m = sdk::m_by_name(ic_cls, "LoadImage", 2);
            if (!ic_loadimage_m) ic_loadimage_m = find_method(ic_cls, "LoadImage");
        }
        if (il2cpp::class_get_methods && il2cpp::method_get_name && il2cpp::method_get_param_count) {
            void* it = nullptr;
            while (const Il2CppMethod* m = il2cpp::class_get_methods(ic_cls, &it)) {
                const char* nm = il2cpp::method_get_name(m);
                if (!nm || !strstr(nm, "LoadImage")) continue;
                void* mp = (void*)(*(void**)((uintptr_t)m + il2cpp::offset::il2cpp_method_pointer));
                LOG("skyimg: ic m %s argc=%d ptr=%p", nm, il2cpp::method_get_param_count(m), mp);
            }
        }
    }
    if (il2cpp::domain_assembly_open) {
        const char* names[] = { "mscorlib.dll", "mscorlib", "System.Private.CoreLib.dll", "System.Runtime.dll" };
        for (int i = 0; i < 4 && !byte_cls; i++)
            byte_cls = sdk::class_lazy(names[i], "System", "Byte");
    }
    ok_res = rs_get_skybox && rs_set_skybox && t2d_cls && t2d_ctor &&
             (ic_loadimage || ic_loadimage_m) && byte_cls &&
             il2cpp::array_new && il2cpp::object_new &&
             chams_settex && chams_getshader && chams_setshader && chams_gettex &&
             chams_find && chams_setcolor && il2cpp::string_new;
    LOG("skyimg: rset=%d gsb=%p ssb=%p t2d=%p ctor=%p load=%p loadm=%p byte=%p matctor=%p find=%p ok=%d",
        (int)sdk::unity::rset_ok, (void*)rs_get_skybox, (void*)rs_set_skybox, (void*)t2d_cls,
        (void*)t2d_ctor, (void*)ic_loadimage, (void*)ic_loadimage_m, (void*)byte_cls,
        (void*)mat_ctor_shader, (void*)chams_find, (int)ok_res);
    return ok_res;
}

static bool load_texture() {
    if (loaded) return our_tex != nullptr;
    if (!ok_res) return false;
    Il2CppArray* arr = il2cpp::array_new(byte_cls, g_sky_jpg_len);
    if (!arr || !readable((uint64_t)arr + 0x20, 32)) return false;
    memcpy((void*)((uintptr_t)arr + 0x20), g_sky_jpg, g_sky_jpg_len);
    void* tex = il2cpp::object_new(t2d_cls);
    if (!tex) return false;
    t2d_ctor(tex, 2, 2);
    bool ok = false;
    if (ic_loadimage) {
        ok = ic_loadimage(tex, arr);
        if (!ok && ic_loadimage3) ok = ic_loadimage3(tex, arr, 0);
    } else if (ic_loadimage_m && il2cpp::runtime_invoke) {
        void* params[2] = { tex, arr };
        void* exc = nullptr;
        il2cpp::runtime_invoke(ic_loadimage_m, nullptr, params, &exc);
        ok = exc == nullptr;
        LOG("skyimg: LoadImage via runtime_invoke exc=%p", exc);
    } else {
        LOG("skyimg: no image loader");
        return false;
    }
    int w = t2d_width ? t2d_width(tex) : -1;
    int h = t2d_height ? t2d_height(tex) : -1;
    LOG("skyimg: LoadImage ret=%d tex=%dx%d", (int)ok, w, h);
    our_tex = tex;
    loaded = true;
    LOG("skyimg: texture loaded len=%zu", (size_t)g_sky_jpg_len);
    return our_tex != nullptr;
}

static void capture() {
    if (cam_cap) return;
    cam_cap = true;
    if (cm) {
        void* c = cm();
        if (c) {
            s_cam_prev = c;
            if (cam_get_clearflags) orig_clear = cam_get_clearflags(c);
        }
    }
}

static void restore() {
    if (in_place) {
        void* gm = rs_get_skybox ? rs_get_skybox() : nullptr;
        if (gm) {
            if (orig_shader && chams_setshader) chams_setshader(gm, orig_shader);
            if (orig_tex && chams_settex) chams_settex(gm, orig_tex);
        }
    } else if (applied && rs_set_skybox) {
        rs_set_skybox(orig_skybox);
    }
    orig_skybox = nullptr;
    orig_shader = nullptr;
    orig_tex = nullptr;
    in_place = false;
    if (cm) {
        void* c = cm();
        if (c && cam_set_clearflags) cam_set_clearflags(c, orig_clear);
    }
    cam_cap = false;
    applied = false;
}

static void apply() {
    if (!applied) {
        if (!ok_res) return;
        ensure_shader();
        if (!our_shader) return;
        if (!load_texture()) return;
        if (!our_mat && !in_place) {
            if (mat_cls && il2cpp::object_new && mat_ctor_shader) {
                our_mat = il2cpp::object_new(mat_cls);
                if (!our_mat) {
                    LOG("skyimg: material alloc failed");
                } else {
                    mat_ctor_shader(our_mat, our_shader);
                    set_tex_on(our_mat);
                    if (chams_setcolor && s_col_str) {
                        Col4 w = { 1.f, 1.f, 1.f, 1.f };
                        chams_setcolor(our_mat, s_col_str, w);
                    }
                    orig_skybox = rs_get_skybox ? rs_get_skybox() : nullptr;
                    rs_set_skybox(our_mat);
                    LOG("skyimg: applied owned mat=%p tex=%p orig=%p", our_mat, our_tex, orig_skybox);
                }
            }
            if (!our_mat && !in_place) {
                void* gm = rs_get_skybox ? rs_get_skybox() : nullptr;
                if (gm && chams_getshader && chams_gettex) {
                    orig_shader = chams_getshader(gm);
                    orig_tex = chams_gettex(gm);
                    if (chams_shadername) {
                        char on[64] = {0}, nn[64] = {0};
                        if (orig_shader) read_str((uint64_t)chams_shadername(orig_shader), on, sizeof(on));
                        if (our_shader) read_str((uint64_t)chams_shadername(our_shader), nn, sizeof(nn));
                        LOG("skyimg: skybox shader orig='%s' new='%s'", on, nn);
                    }
                    chams_setshader(gm, our_shader);
                    set_tex_on(gm);
                    if (chams_setcolor && s_col_str) {
                        Col4 w = { 1.f, 1.f, 1.f, 1.f };
                        chams_setcolor(gm, s_col_str, w);
                    }
                    if (chams_shadername && chams_getshader) {
                        char on[64] = {0};
                        void* now_sh = chams_getshader(gm);
                        if (now_sh) read_str((uint64_t)chams_shadername(now_sh), on, sizeof(on));
                        LOG("skyimg: after set mat shader='%s'", on);
                    }
                    in_place = true;
                    LOG("skyimg: applied in-place mat=%p tex=%p orig_shader=%p orig_tex=%p",
                        gm, our_tex, orig_shader, orig_tex);
                } else {
                    LOG("skyimg: no material to use (create=%d get_sb=%p)", (int)(mat_ctor_shader != nullptr),
                        (void*)rs_get_skybox);
                    return;
                }
            }
        }
        capture();
        applied = true;
    }
    void* c = cm ? cm() : nullptr;
    if (c && c != s_cam_prev) {
        s_cam_prev = c;
        cam_cap = false;
        capture();
    }
    if (c && cam_set_clearflags) cam_set_clearflags(c, 1);
}

static void late(void* p, bool local) {
    (void)p;
    if (!local) return;
    if (!opt_sky_img) {
        if (applied) restore();
        return;
    }
    if (!resolve()) {
        static bool log1 = false;
        if (!log1) {
            log1 = true;
            LOG("skyimg: pending resolve ok=%d", (int)ok_res);
        }
        return;
    }
    apply();
}

static void reset() {
    if (applied) restore();
    s_cam_prev = nullptr;
}

}

namespace {
static events::feature _f_skyimg = {"skyimg", nullptr, skyimg::late, nullptr, nullptr, skyimg::reset};
static struct _reg_skyimg { _reg_skyimg() { events::register_feature(_f_skyimg); } } _r_skyimg;
}
