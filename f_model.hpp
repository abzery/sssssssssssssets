#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"
#include "../assets/model_data.hpp"
#include "../assets/model_tex.hpp"

namespace f_model {

inline bool s_res_done = false;
inline bool s_res_ok = false;
inline bool s_built = false;
inline void* s_go = nullptr;
inline void* s_renderer = nullptr;
inline void* s_transform = nullptr;
inline void* s_mat = nullptr;
inline void* s_tex = nullptr;
inline uint64_t s_cur_mr = 0;
inline bool s_orig_hidden = false;

struct model_data {
    const char* b64_pos;
    const char* b64_nrm;
    const char* b64_uvs;
    const char* b64_tri;
    int vcount;
    int tcount;
    const char* b64_png;
    size_t png_len;
};

static const model_data md_sahur = {
    g_model_pos_b64, g_model_nrm_b64, g_model_uvs_b64, g_model_tri_b64,
    g_model_vert_count, g_model_tri_count,
    g_model_png_b64, g_model_png_len
};

static const model_data* cur_md() {
    return &md_sahur;
}

inline Il2CppClass* s_vec3_cls = nullptr;
inline Il2CppClass* s_vec2_cls = nullptr;
inline Il2CppClass* s_int_cls = nullptr;
inline Il2CppClass* s_byte_cls = nullptr;
inline Il2CppClass* s_go_cls = nullptr;
inline Il2CppClass* s_mesh_cls = nullptr;

inline void (*s_mat_ctor)(void*, void*) = nullptr;
inline void* (*s_find_shader)(Il2CppString*) = nullptr;
inline void (*s_settex)(void*, void*) = nullptr;
inline void (*s_setcolor)(void*, Il2CppString*, Col4) = nullptr;
inline void (*s_setmat)(void*, void*) = nullptr;
inline void (*s_setenabled)(void*, int) = nullptr;
inline void* (*s_getmat)(void*) = nullptr;

inline Il2CppString* s_shader_diff = nullptr;
inline Il2CppString* s_maintex = nullptr;
inline Il2CppString* s_col = nullptr;

inline void (*s_t2d_ctor)(void*, int, int) = nullptr;
inline bool (*s_ic_load)(void*, void*) = nullptr;
inline bool (*s_ic_load3)(void*, void*, int) = nullptr;

static int b64v(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static bool b64_decode(const char* s, unsigned char* out, size_t out_cap, size_t* out_len) {
    size_t o = 0;
    int buf = 0, bits = 0;
    for (; *s; s++) {
        if (*s == '"') continue;
        if (*s == ';') break;
        int v = b64v(*s);
        if (v < 0) continue;
        buf = (buf << 6) | v;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            if (o >= out_cap) return false;
            out[o++] = (unsigned char)((buf >> bits) & 0xFF);
        }
    }
    *out_len = o;
    return true;
}

static void* dec_alloc(const char* b64, size_t* n) {
    size_t cap = 0;
    for (const char* p = b64; *p; p++) {
        int v = b64v(*p);
        if (v >= 0) cap++;
    }
    cap = cap * 3 / 4 + 8;
    unsigned char* buf = (unsigned char*)malloc(cap);
    if (!buf) return nullptr;
    if (!b64_decode(b64, buf, cap, n)) {
        free(buf);
        return nullptr;
    }
    return buf;
}

static void* mk_arr(Il2CppClass* elem, size_t count, const void* data, size_t bytes) {
    if (!il2cpp::array_new || !elem || !count) return nullptr;
    Il2CppArray* a = il2cpp::array_new(elem, count);
    if (!a) return nullptr;
    if (data && bytes) {
        size_t need = count * (size_t)il2cpp::class_array_element_size(elem);
        if (bytes > need) bytes = need;
        if (readable((uint64_t)(uintptr_t)a + 0x20, bytes))
            memcpy((void*)((uintptr_t)a + 0x20), data, bytes);
    }
    return a;
}

static Il2CppClass* find_int32_cls() {
    const char* names[] = { "mscorlib.dll", "mscorlib", "System.Private.CoreLib.dll", "System.Runtime.dll" };
    for (int i = 0; i < 4; i++) {
        Il2CppClass* c = sdk::class_lazy(names[i], "System", "Int32");
        if (c) return c;
    }
    return nullptr;
}

static uint64_t local_player() {
    uint64_t pm = player_manager();
    if (!ok(pm)) return 0;
    return rd64(pm + OFF_PM_LOCAL_PLAYER);
}

static uint64_t player_char_renderer(uint64_t p) {
    if (!ok(p)) return 0;
    uint64_t lod = rd64(p + OFF_PLAYER_CHAR_LOD);
    if (!ok(lod)) return 0;
    uint64_t mr = rd64(lod + OFF_CHAR_LOD_MESH_RENDERER);
    return ok(mr) ? mr : 0;
}

static bool resolve() {
    if (s_res_done) return s_res_ok;
    s_res_done = true;
    if (!sdk::unity::mesh_ok) sdk::unity::mesh_resolve();
    if (!sdk::unity::mat_ok) sdk::unity::mat_resolve();
    if (!sdk::unity::sh_ok) sdk::unity::sh_resolve();
    if (!sdk::unity::comp_ok) sdk::unity::comp_resolve();
    if (!sdk::unity::tr_ok) sdk::unity::tr_resolve();
    if (!sdk::unity::obj_ok) sdk::unity::obj_resolve();
    if (!sdk::unity::re_ok) sdk::unity::re_resolve();

    s_vec3_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Vector3");
    s_vec2_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Vector2");
    s_int_cls = find_int32_cls();
    s_go_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "GameObject");
    s_mesh_cls = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Mesh");

    s_mat_ctor = mat_ctor_shader;
    s_find_shader = chams_find;
    s_settex = chams_settex;
    s_setcolor = chams_setcolor;
    s_setmat = chams_setmat;
    s_setenabled = chams_setenabled;
    s_getmat = chams_getmat;

    if (il2cpp::string_new) {
        s_shader_diff = il2cpp::string_new("Legacy Shaders/Diffuse");
        s_maintex = il2cpp::string_new("_MainTex");
        s_col = il2cpp::string_new("_Color");
    }

    {
        Il2CppClass* t2d = sdk::class_lazy_any("UnityEngine", "Texture2D");
        if (!t2d) t2d = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Texture2D");
        Il2CppClass* ic = sdk::class_lazy_any("UnityEngine", "ImageConversion");
        if (!ic) ic = sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "ImageConversion");
        if (t2d) s_t2d_ctor = (void (*)(void*, int, int))sdk::resolve_m<void (*)(void*, int, int)>(t2d, ".ctor", 2);
        if (ic) {
            s_ic_load = (bool (*)(void*, void*))sdk::resolve_m<bool (*)(void*, void*)>(ic, "LoadImage", 2);
            s_ic_load3 = (bool (*)(void*, void*, int))sdk::resolve_m<bool (*)(void*, void*, int)>(ic, "LoadImage", 3);
        }
        s_byte_cls = nullptr;
        const char* names[] = { "mscorlib.dll", "mscorlib", "System.Private.CoreLib.dll", "System.Runtime.dll" };
        for (int i = 0; i < 4 && !s_byte_cls; i++)
            s_byte_cls = sdk::class_lazy(names[i], "System", "Byte");
    }

    s_res_ok = sdk::unity::mesh_ok && s_vec3_cls && s_vec2_cls && s_int_cls && s_go_cls &&
               s_mesh_cls && s_setmat && s_setenabled && il2cpp::object_new && il2cpp::array_new;
    LOG("model: resolve ok=%d mesh=%d mctor=%p getmat=%p setmat=%p seten=%p",
        (int)s_res_ok, (int)sdk::unity::mesh_ok, (void*)s_mat_ctor, (void*)s_getmat,
        (void*)s_setmat, (void*)s_setenabled);
    return s_res_ok;
}

static void* load_texture(const model_data* md) {
    if (!s_byte_cls || !il2cpp::array_new || !s_t2d_ctor) return nullptr;
    size_t n = 0;
    unsigned char* png = (unsigned char*)dec_alloc(md->b64_png, &n);
    if (!png || n != md->png_len) {
        LOG("model: png decode failed n=%zu want=%zu", n, md->png_len);
        if (png) free(png);
        return nullptr;
    }
    Il2CppArray* arr = il2cpp::array_new(s_byte_cls, n);
    if (!arr || !readable((uint64_t)(uintptr_t)arr + 0x20, n)) {
        free(png);
        return nullptr;
    }
    memcpy((void*)((uintptr_t)arr + 0x20), png, n);
    free(png);
    void* tex = il2cpp::object_new(sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Texture2D"));
    if (!tex) {
        LOG("model: tex alloc failed");
        return nullptr;
    }
    s_t2d_ctor(tex, 2, 2);
    bool ok = false;
    if (s_ic_load) {
        ok = s_ic_load(tex, arr);
        if (!ok && s_ic_load3) ok = s_ic_load3(tex, arr, 0);
    } else {
        LOG("model: no image loader");
        return nullptr;
    }
    LOG("model: texture load=%d", (int)ok);
    return tex;
}

static bool build() {
    if (s_built) return s_go != nullptr;
    if (!resolve()) return false;
    const model_data* md = cur_md();

    size_t pn = 0, nn = 0, un = 0, tn = 0;
    void* pbuf = dec_alloc(md->b64_pos, &pn);
    void* nbuf = dec_alloc(md->b64_nrm, &nn);
    void* ubuf = dec_alloc(md->b64_uvs, &un);
    void* tbuf = dec_alloc(md->b64_tri, &tn);
    if (!pbuf || !nbuf || !ubuf || !tbuf) {
        LOG("model: decode failed pn=%zu nn=%zu un=%zu tn=%zu", pn, nn, un, tn);
        if (pbuf) free(pbuf);
        if (nbuf) free(nbuf);
        if (ubuf) free(ubuf);
        if (tbuf) free(tbuf);
        return false;
    }

    void* varr = mk_arr(s_vec3_cls, md->vcount, pbuf, pn);
    void* narr = mk_arr(s_vec3_cls, md->vcount, nbuf, nn);
    void* uarr = mk_arr(s_vec2_cls, md->vcount, ubuf, un);
    void* tarr = mk_arr(s_int_cls, md->tcount * 3, tbuf, tn);
    free(pbuf);
    free(nbuf);
    free(ubuf);
    free(tbuf);
    if (!varr || !narr || !uarr || !tarr) {
        LOG("model: array alloc failed v=%p n=%p u=%p t=%p", varr, narr, uarr, tarr);
        return false;
    }

    void* mesh = il2cpp::object_new(s_mesh_cls);
    if (!mesh) {
        LOG("model: mesh alloc failed");
        return false;
    }
    mesh_ctor(mesh);
    mesh_set_index_format(mesh, 1);
    mesh_set_vertices(mesh, varr);
    mesh_set_normals(mesh, narr);
    mesh_set_uv(mesh, uarr);
    mesh_set_triangles(mesh, tarr);
    if (mesh_recalc_bounds) mesh_recalc_bounds(mesh);

    void* go = il2cpp::object_new(s_go_cls);
    if (!go) {
        LOG("model: go alloc failed");
        return false;
    }
    if (mgo_ctor0) mgo_ctor0(go);

    void* type_mf = nullptr;
    void* type_mr = nullptr;
    if (type_get_type && il2cpp::string_new) {
        type_mf = type_get_type(il2cpp::string_new("UnityEngine.MeshFilter, UnityEngine.CoreModule"), 0);
        type_mr = type_get_type(il2cpp::string_new("UnityEngine.MeshRenderer, UnityEngine.CoreModule"), 0);
    }
    if (!type_mf || !type_mr) {
        LOG("model: type resolve failed mf=%p mr=%p", type_mf, type_mr);
        return false;
    }
    void* mf = mgo_add_component(go, type_mf);
    void* mr = mgo_add_component(go, type_mr);
    if (!mf || !mr) {
        LOG("model: addcomponent failed mf=%p mr=%p", mf, mr);
        return false;
    }
    if (mfilter_set_shared_mesh) mfilter_set_shared_mesh(mf, mesh);

    void* mat = nullptr;
    uint64_t lp = local_player();
    uint64_t pmr = player_char_renderer(lp);
    if (s_getmat && ok(pmr)) mat = s_getmat((void*)pmr);
    if (!mat && s_mat_ctor) {
        void* shader = s_find_shader(s_shader_diff);
        if (shader) {
            void* nm = il2cpp::object_new(sdk::class_lazy("UnityEngine.CoreModule", "UnityEngine", "Material"));
            if (nm) {
                s_mat_ctor(nm, shader);
                mat = nm;
            }
        }
    }
    if (!mat) {
        LOG("model: no material source");
        return false;
    }
    s_tex = load_texture(md);
    if (s_tex && s_settex) s_settex(mat, s_tex);
    if (s_setcolor && s_col) {
        Col4 w = { 0.9f, 0.9f, 0.9f, 1.f };
        s_setcolor(mat, s_col, w);
    }
    if (s_setmat) s_setmat(mr, mat);

    s_go = go;
    s_renderer = mr;
    s_transform = mgo_get_transform(go);
    s_mat = mat;
    s_built = true;
    LOG("model: built go=%p mr=%p mesh=%p mat=%p tex=%p tr=%p",
        go, mr, mesh, mat, s_tex, s_transform);
    return true;
}

static void show_orig(uint64_t p) {
    uint64_t mr = player_char_renderer(p);
    if (ok(mr) && s_setenabled) s_setenabled((void*)mr, 1);
    s_orig_hidden = false;
    s_cur_mr = 0;
}

static void hide_orig(uint64_t p) {
    uint64_t mr = player_char_renderer(p);
    if (!ok(mr)) return;
    if (s_setenabled) s_setenabled((void*)mr, 0);
    s_cur_mr = mr;
    s_orig_hidden = true;
}

static void destroy() {
    if (s_go && mgo_set_active) mgo_set_active(s_go, 0);
    s_go = nullptr;
    s_renderer = nullptr;
    s_transform = nullptr;
    s_mat = nullptr;
    s_tex = nullptr;
    s_built = false;
}

static void late(void* p, bool local) {
    if (!local) return;
    if (!opt_model) {
        if (s_orig_hidden) show_orig((uint64_t)p);
        if (s_built) destroy();
        return;
    }
    if (!build()) {
        static int lg = 0;
        if ((lg++ & 0x3F) == 0) LOG("model: pending build res=%d", (int)s_res_ok);
        return;
    }
    uint64_t pp = (uint64_t)p;
    hide_orig(pp);
    if (!s_transform || !tsp || !tse || !cgt) return;

    uint64_t mr = player_char_renderer(pp);
    if (!ok(mr)) return;
    void* ptrans = cgt((void*)mr);
    if (!ptrans) return;

    Vector3 pos = player_pos(pp);
    if (!sane_world_pos(pos)) return;
    Vector3 fwd = { 0.f, 0.f, 1.f };
    if (tgf) fwd = tgf(ptrans);
    pos.x += fwd.x * s_model_dist;
    pos.z += fwd.z * s_model_dist;
    pos.y += s_model_height;
    tsp(s_transform, pos);

    float yaw = atan2f(fwd.x, fwd.z) * 180.f / 3.14159265f;
    Vector3 rot = { 0.f, yaw + s_model_rot, 0.f };
    tse(s_transform, rot);

    if (sls) {
        Vector3 scl = { 1.f, 1.f, 1.f };
        sls(s_transform, scl);
    }
}

static void reset() {
    if (s_orig_hidden) {
        uint64_t lp = local_player();
        if (ok(lp)) show_orig(lp);
    }
    destroy();
}

}

namespace {
static events::feature _f_model = {"model", nullptr, f_model::late, nullptr, nullptr, f_model::reset};
static struct _reg_model { _reg_model() { events::register_feature(_f_model); } } _r_model;
}
