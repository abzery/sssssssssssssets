#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace f_chams {

inline Il2CppString* s_chams_shader_flat = nullptr;
inline Il2CppString* s_chams_shader_solid = nullptr;
inline Il2CppString* s_chams_color = nullptr;
inline void* chams_shader_flat_obj = nullptr;
inline void* chams_shader_solid_obj = nullptr;
inline bool s_chams_tried_shaders = false;
inline bool s_chams_strings_ready = false;
inline bool s_chams_logged_shaders = false;
inline int s_chams_apply_n = 0;

static bool obj_alive(uint64_t a) {
    if (!obj_ok(a)) return false;
    uint64_t cp = rd64(a + 0x10);
    return cp > 0x1000000 && readable(cp, 8);
}

static bool renderer_like(uint64_t a) {
    if (!obj_alive(a)) return false;
    if (!il2cpp::class_get_name) return false;
    uint64_t k = *(uint64_t*)a;
    if (!readable(k, 0x20)) return false;
    const char* n = il2cpp::class_get_name((Il2CppClass*)k);
    if (!n || !readable((uint64_t)n, 96)) return false;
    char buf[96];
    for (int i = 0; i < 95; i++) {
        buf[i] = n[i];
        buf[i + 1] = 0;
        if (buf[i] == 0) break;
    }
    return strstr(buf, "Renderer") != nullptr;
}

static void chams_reset_all() {
    s_chams_shader_flat = nullptr;
    s_chams_shader_solid = nullptr;
    s_chams_color = nullptr;
    chams_shader_flat_obj = nullptr;
    chams_shader_solid_obj = nullptr;
    s_chams_tried_shaders = false;
    s_chams_strings_ready = false;
}

static void chams_ensure_strings() {
    if (s_chams_strings_ready) return;
    if (!il2cpp::string_new) return;
    s_chams_shader_flat = il2cpp::string_new("Hidden/Internal-Colored");
    s_chams_shader_solid = il2cpp::string_new("Legacy Shaders/Diffuse");
    s_chams_color = il2cpp::string_new("_Color");
    if (!s_chams_shader_flat || !s_chams_shader_solid || !s_chams_color) {
        LOG("chams: string_new failed");
        return;
    }
    if (shader_property_to_id) {
        chams_color_id = shader_property_to_id(s_chams_color);
        LOG("chams: ids color=%d", chams_color_id);
    }
    s_chams_strings_ready = true;
}

static bool chams_core_ok() {
    if (!sdk::unity::re_ok || !sdk::unity::mat_ok || !sdk::unity::sh_ok) {
        static bool clog = false;
        if (!clog) {
            clog = true;
            LOG("chams: core pending re=%d mat=%d sh=%d", (int)sdk::unity::re_ok,
                (int)sdk::unity::mat_ok, (int)sdk::unity::sh_ok);
        }
        return false;
    }
    chams_ensure_strings();
    return s_chams_strings_ready;
}

static void chams_ensure_shaders() {
    if (s_chams_tried_shaders) return;
    s_chams_tried_shaders = true;
    if (!chams_shader_solid_obj)
        chams_shader_solid_obj = chams_find(s_chams_shader_solid);
    if (!chams_shader_flat_obj)
        chams_shader_flat_obj = chams_find(s_chams_shader_flat);
    if (!chams_shader_solid_obj && chams_shader_flat_obj)
        chams_shader_solid_obj = chams_shader_flat_obj;
    if (!chams_shader_flat_obj && chams_shader_solid_obj)
        chams_shader_flat_obj = chams_shader_solid_obj;
    if (!s_chams_logged_shaders) {
        s_chams_logged_shaders = true;
        char a[64] = {0}, c[64] = {0};
        if (chams_shadername) {
            if (chams_shader_flat_obj) read_str((uint64_t)chams_shadername(chams_shader_flat_obj), a, sizeof(a));
            if (chams_shader_solid_obj) read_str((uint64_t)chams_shadername(chams_shader_solid_obj), c, sizeof(c));
        }
        LOG("chams: shaders flat='%s' solid='%s'", a, c);
    }
}

static void chams_apply_material_to(void* renderer, int type, const ImVec4& col) {
    if (!chams_core_ok() || !chams_getshader || !chams_setshader || !chams_setcolor)
        return;
    uint64_t r = (uint64_t)renderer;
    if (!obj_alive(r)) return;
    void* m = chams_getmat ? chams_getmat(renderer) : (chams_getsharedmat ? chams_getsharedmat(renderer) : nullptr);
    if (!obj_alive((uint64_t)m)) {
        if ((s_chams_apply_n & 0x1FF) == 0)
            LOG("chams: dead mat r=%llx m=%llx", (unsigned long long)r, (unsigned long long)(uintptr_t)m);
        return;
    }
    void* shader = nullptr;
    switch (type) {
        case 0: shader = chams_shader_solid_obj; break;
        case 1: shader = chams_shader_flat_obj; break;
        default: break;
    }
    if (shader) {
        void* cursh = chams_getshader(m);
        if ((s_chams_apply_n & 0x3FF) == 0) {
            char sn[64] = {0}, cn[64] = {0};
            if (chams_shadername) {
                if (shader) read_str((uint64_t)chams_shadername(shader), sn, sizeof(sn));
                if (cursh) read_str((uint64_t)chams_shadername(cursh), cn, sizeof(cn));
            }
            LOG("chams: tick r=%llx m=%llx type=%d sh='%s' cur='%s'",
                (unsigned long long)r, (unsigned long long)(uintptr_t)m, type, sn, cn);
        }
        if (cursh != shader) {
            chams_setshader(m, shader);
            if (chams_gettex && chams_settex && chams_gettex(m))
                chams_settex(m, 0);
        }
    }
    Col4 c = { col.x, col.y, col.z, col.w };
    if (chams_setcolor_id)
        chams_setcolor_id(m, chams_color_id, c);
    else if (chams_setcolor)
        chams_setcolor(m, s_chams_color, c);
    s_chams_apply_n++;
}

static void chams_apply_material(void* renderer) {
    chams_apply_material_to(renderer, s_chams_type, s_chams_col);
}

static void chams_check_match(uint64_t p) {
    static uint64_t s_chams_lp = 0;
    uint64_t pm = player_manager();
    if (!ok(pm)) return;
    uint64_t lp = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp)) return;
    if (s_chams_lp && lp != s_chams_lp) {
        LOG("chams: match transition detected, resetting");
        chams_reset_all();
    }
    s_chams_lp = lp;
    (void)p;
}

static void chams_force_visible(uint64_t p) {
    uint64_t oc = rd64(p + OFF_PLAYER_OCCLUSION);
    if (ok(oc) && vm_set_enabled) ((void(*)(void*, int))vm_set_enabled)((void*)oc, 1);
    uint64_t v = rd64(p + OFF_PLAYER_CHAR_VIEW);
    if (ok(v)) {
        wr8(v + OFF_CHAR_VIEW_OCCLUSION, 2);
        uint64_t vt = rd64(p + OFF_PLAYER_CHAR_VIEW_TPS);
        if (ok(vt)) wr8(vt + OFF_CHAR_VIEW_OCCLUSION, 2);
    }
    wr8(p + OFF_PLAYER_CHAR_VISIBLE, 1);
    if (vm_set_visible) ((void(*)(void*))vm_set_visible)((void*)p);
    uint64_t lod = rd64(p + OFF_PLAYER_CHAR_LOD);
    if (ok(lod)) {
        uint64_t mr = rd64(lod + OFF_CHAR_LOD_MESH_RENDERER);
        if (ok(mr) && chams_setenabled) chams_setenabled((void*)mr, 1);
    }
}

static void chams_tick_player(uint64_t p) {
    if (!opt_chams) return;
    chams_check_match(p);
    if (!chams_core_ok()) return;
    chams_ensure_shaders();
    chams_force_visible(p);
    uint64_t lod = rd64(p + OFF_PLAYER_CHAR_LOD);
    if (!ok(lod)) return;
    uint64_t mr = rd64(lod + OFF_CHAR_LOD_MESH_RENDERER);
    if (!renderer_like(mr)) return;
    chams_apply_material((void*)mr);
}

static void chams_tick_local(uint64_t p) {
    if (!opt_chams || !opt_chams_self) return;
    chams_check_match(p);
    if (!chams_core_ok()) return;
    chams_ensure_shaders();
    uint64_t lod = rd64(p + OFF_PLAYER_CHAR_LOD);
    if (!ok(lod)) return;
    uint64_t mr = rd64(lod + OFF_CHAR_LOD_MESH_RENDERER);
    if (!renderer_like(mr)) return;
    chams_apply_material((void*)mr);
}

static void hands_tick(uint64_t p) {
    if (!opt_hands) return;
    if (rd32(p + 0x134) == 2) return;
    if (!chams_core_ok()) return;
    if (s_hands_shader) chams_ensure_shaders();
    uint64_t alod = rd64(p + OFF_PLAYER_ARMS_LOD);
    if (!ok(alod)) return;
    uint64_t arms = rd64(alod + OFF_ARMS_MESH_RENDERER);
    uint64_t gloves = rd64(alod + OFF_ARMS_GLOVES_RENDERER);
    if (!ok(arms) && !ok(gloves)) return;
    if (renderer_like(arms)) chams_apply_material_to((void*)arms, s_hands_type, s_hands_col);
    if (renderer_like(gloves)) chams_apply_material_to((void*)gloves, s_hands_type, s_hands_col);
}

static void weapon_tick(uint64_t p) {
    if (!opt_weapon_chams) return;
    if (rd32(p + 0x134) == 2) return;
    if (!chams_core_ok()) return;
    if (s_weapon_chams_shader) chams_ensure_shaders();
    uint64_t wr = rd64(p + OFF_PLAYER_WEAPONRY);
    if (!ok(wr)) return;
    uint64_t wc = rd64(wr + OFF_WEAPONRY_CURRENT);
    if (!ok(wc)) return;
    uint64_t wl = rd64(wc + OFF_WEAPON_LOD);
    if (!ok(wl)) return;
    {
        static bool wl1 = false;
        if (!wl1) {
            wl1 = true;
            LOG("weapon: wr=%llx wc=%llx wl=%llx",
                (unsigned long long)wr, (unsigned long long)wc, (unsigned long long)wl);
        }
    }
    uint64_t smesh = rd64(wl + OFF_LOD_SKINNED_ARRAY);
    uint64_t mmesh = rd64(wl + OFF_LOD_MESH_ARRAY);
    {
        static int wfb = 0;
        if ((wfb++ & 0x7FF) == 0) {
            int sn = ok(smesh) ? rd32(smesh + OFF_UNITY_ARRAY_LENGTH) : -1;
            int mn = ok(mmesh) ? rd32(mmesh + OFF_UNITY_ARRAY_LENGTH) : -1;
            LOG("weapon: wc=%llx wl=%llx sk=%d mesh=%d",
                (unsigned long long)wc, (unsigned long long)wl, sn, mn);
        }
    }
    {
        static bool wd1 = false;
        if (!wd1) {
            wd1 = true;
            int sn = ok(smesh) ? rd32(smesh + OFF_UNITY_ARRAY_LENGTH) : -1;
            int mn = ok(mmesh) ? rd32(mmesh + OFF_UNITY_ARRAY_LENGTH) : -1;
            LOG("weapon: smesh=%llx n=%d mmesh=%llx n=%d",
                (unsigned long long)smesh, sn, (unsigned long long)mmesh, mn);
            for (int ai = 0; ai < 2; ai++) {
                uint64_t arr = ai == 0 ? smesh : mmesh;
                const char* tag = ai == 0 ? "sk" : "mesh";
                if (!ok(arr)) continue;
                int n = rd32(arr + OFF_UNITY_ARRAY_LENGTH);
                LOG("weapon: %s n=%d", tag, n);
                for (int i = 0; i < n && i < 4; i++) {
                    uint64_t r = rd64(arr + OFF_UNITY_ARRAY_DATA + 8 * (uint64_t)i);
                    uint64_t k = r ? *(uint64_t*)r : 0;
                    const char* nm = nullptr;
                    if (k && readable(k, 8) && il2cpp::class_get_name)
                        nm = il2cpp::class_get_name((Il2CppClass*)k);
                    LOG("weapon: %s[%d] r=%llx k=%llx name=%s like=%d",
                        tag, i, (unsigned long long)r, (unsigned long long)k,
                        nm ? nm : "?", (int)renderer_like(r));
                }
            }
        }
    }
    if (ok(smesh)) {
        int n = rd32(smesh + OFF_UNITY_ARRAY_LENGTH);
        if (n > 0 && n < 64) {
            for (int i = 0; i < n; i++) {
                uint64_t r = rd64(smesh + OFF_UNITY_ARRAY_DATA + 8 * (uint64_t)i);
                if (renderer_like(r)) chams_apply_material_to((void*)r, s_weapon_chams_type, s_weapon_chams_col);
            }
        }
    }
    if (ok(mmesh)) {
        int n = rd32(mmesh + OFF_UNITY_ARRAY_LENGTH);
        if (n > 0 && n < 64) {
            for (int i = 0; i < n; i++) {
                uint64_t r = rd64(mmesh + OFF_UNITY_ARRAY_DATA + 8 * (uint64_t)i);
                if (renderer_like(r)) chams_apply_material_to((void*)r, s_weapon_chams_type, s_weapon_chams_col);
            }
        }
    }
}

static void late(void* p, bool local) {
    uint64_t pp = (uint64_t)p;
    if (local) {
        chams_tick_local(pp);
        hands_tick(pp);
        weapon_tick(pp);
        return;
    }
    chams_tick_player(pp);
}

static void reset() {
    chams_reset_all();
}

}

namespace {
static events::feature _f_chams = {"chams", nullptr, f_chams::late, nullptr, nullptr, f_chams::reset};
static struct _reg_chams { _reg_chams() { events::register_feature(_f_chams); } } _r_chams;
}
