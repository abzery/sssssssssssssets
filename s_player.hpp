#pragma once

#include "../../core.hpp"

inline Vector3 player_pos(uint64_t p) {
    Vector3 r{};
    if (!ok(p)) return r;
    uint64_t mc = rd64(p + OFF_PLAYER_MOVEMENT_CTRL);
    if (!ok(mc)) return r;
    uint64_t td = rd64(mc + OFF_MC_TRANSFORM_DATA);
    if (!ok(td)) return r;
    rdv(td + OFF_TD_POSITION, r);
    return r;
}

inline bool valid_pm(uint64_t pm) {
    return ok(pm) && ok(rd64(pm + OFF_PM_LOCAL_PLAYER));
}

inline uint64_t pm_lazy() {
    uint64_t v1 = rd64(g_base + OFF_PLAYER_MANAGER);
    if (!ok(v1)) return 0;
    uint64_t v2 = rd64(v1 + 0x58);
    if (!ok(v2)) return 0;
    uint64_t v3 = rd64(v2 + 0xB8);
    if (!ok(v3)) return 0;
    return rd64(v3 + 0x0);
}

inline uint64_t pm_static() {
    uint64_t cls = rd64(g_base + OFF_PLAYER_MANAGER);
    if (!ok(cls)) return 0;
    uint64_t obj = rd64(cls + 0x90);
    if (!ok(obj)) return 0;
    return rd64(obj + 0x10);
}

inline bool has_lp(uint64_t pm) {
    return ok(pm) && ok(rd64(pm + OFF_PM_LOCAL_PLAYER));
}

inline uint64_t player_manager() {
    uint64_t a = pm_static();
    uint64_t b = pm_lazy();
    uint64_t best = 0;
    if (ok(a)) best = a;
    if (has_lp(b) && (!has_lp(best) || b != a)) best = b;
    return best;
}

inline void player_nick(uint64_t p, char* out, size_t cap) {
    uint64_t pp = rd64(p + OFF_PLAYER_PHOTON_PTR);
    if (!ok(pp)) { out[0] = '\0'; return; }
    read_str(rd64(pp + OFF_PHOTON_NAME), out, cap);
}

inline void weapon_name(uint64_t p, char* out, size_t cap) {
    uint64_t wr = rd64(p + OFF_PLAYER_WEAPONRY);
    if (!ok(wr)) { out[0] = '\0'; return; }
    uint64_t wp = rd64(wr + OFF_WEAPONRY_CURRENT);
    if (!ok(wp)) { out[0] = '\0'; return; }
    uint64_t pr = rd64(wp + OFF_WC_WEAPON_PROPS);
    if (!ok(pr)) { out[0] = '\0'; return; }
    read_str(rd64(pr + 0x20), out, cap);
}

inline int health_of(uint64_t p) {
    float hp = rdf(p + OFF_PLAYER_HEALTH);
    if (hp > 0.f && hp < 1000.f) return (int)hp;
    uint64_t pp = rd64(p + OFF_PLAYER_PHOTON_PTR);
    if (!ok(pp)) return 0;
    uint64_t pr = rd64(pp + OFF_PHOTON_PROPS_REG);
    if (!ok(pr)) return 0;
    int cnt = rd32(pr + OFF_PROPS_COUNT);
    if (cnt <= 0 || cnt > 4096) return 0;
    uint64_t pl = rd64(pr + OFF_PROPS_LIST);
    if (!ok(pl)) return 0;
    for (int i = 0; i < cnt; i++) {
        uint64_t k = rd64(pl + OFF_PROPS_KEY_BASE + 0x18 * (uint64_t)i);
        if (!ok(k)) continue;
        if (str_contains(k, "health")) {
            uint64_t v = rd64(pl + OFF_PROPS_VAL_BASE + 0x18 * (uint64_t)i);
            if (ok(v)) {
                int val = rd32(v + OFF_PROPS_VALUE_DATA);
                if (val > 0 && val < 1000) return val;
            }
        }
    }
    return 0;
}

struct TM { float pos[4], rot[4], scl[4]; };

inline bool tp(uint64_t tr, Vector3& o, Quaternion* qr = nullptr) {
    if (!ok(tr)) return false;
    uint64_t n = rd64(tr + 0x10);
    if (!ok(n)) return false;
    uint64_t m = rd64(n + OFF_TRANSFORM_MATRIX);
    if (!ok(m)) return false;
    int32_t idx = rd32(n + OFF_TRANSFORM_INDEX);
    if (idx < 0 || idx > 100000) return false;
    uint64_t ml = rd64(m + OFF_MATRIX_LIST);
    if (!ok(ml)) return false;
    uint64_t mi = rd64(m + OFF_MATRIX_INDICES);
    if (!ok(mi)) return false;
    TM b;
    if (!rdb(ml + (uint64_t)idx * TRANSFORM_MATRIX_SIZE, &b, sizeof(b))) return false;
    float gx = b.pos[0], gy = b.pos[1], gz = b.pos[2];
    float qx = b.rot[0], qy = b.rot[1], qz = b.rot[2], qw = b.rot[3];
    if (ok(mi)) {
        int32_t pi = 0;
        if (!rdb(mi + (uint64_t)idx * 4, &pi, sizeof(pi))) goto done;
        int lp = 0;
        while (pi >= 0 && lp < 100) {
            lp++;
            TM p;
            if (!rdb(ml + (uint64_t)pi * TRANSFORM_MATRIX_SIZE, &p, sizeof(p))) break;
            float px = p.pos[0], py = p.pos[1], pz = p.pos[2];
            float prx = p.rot[0], pry = p.rot[1], prz = p.rot[2], prw = p.rot[3];
            float sx = p.scl[0], sy = p.scl[1], sz = p.scl[2];
            float scx = gx * sx, scy = gy * sy, scz = gz * sz;
            float qx2 = prx * 2.f, qy2 = pry * 2.f, qz2 = prz * 2.f;
            float xx = prx * qx2, yy = pry * qy2, zz = prz * qz2;
            float xy = prx * qy2, xz = prx * qz2, yz = pry * qz2;
            float wx = prw * qx2, wy = prw * qy2, wz = prw * qz2;
            gx = (1.f - (yy + zz)) * scx + (xy - wz) * scy + (xz + wy) * scz + px;
            gy = (xy + wz) * scx + (1.f - (xx + zz)) * scy + (yz - wx) * scz + py;
            gz = (xz - wy) * scx + (yz + wx) * scy + (1.f - (xx + yy)) * scz + pz;
            float nqx = prw * qx + prx * qw + pry * qz - prz * qy;
            float nqy = prw * qy - prx * qz + pry * qw + prz * qx;
            float nqz = prw * qz + prx * qy - pry * qx + prz * qw;
            float nqw = prw * qw - prx * qx - pry * qy - prz * qz;
            qx = nqx; qy = nqy; qz = nqz; qw = nqw;
            if (!rdb(mi + (uint64_t)pi * 4, &pi, sizeof(pi))) break;
        }
    }
done:
    o.x = gx; o.y = gy; o.z = gz;
    if (qr) { qr->x = qx; qr->y = qy; qr->z = qz; qr->w = qw; }
    return sane_world_pos(o);
}

struct SK { Vector3 b[BIPED_BONE_COUNT]; Quaternion q[BIPED_BONE_COUNT]; bool v[BIPED_BONE_COUNT]; bool ok; };

inline uint64_t bm(uint64_t p) {
    uint64_t v = rd64(p + OFF_PLAYER_CHAR_VIEW);
    if (!ok(v)) v = rd64(p + OFF_PLAYER_VIEW_1);
    if (!ok(v)) v = rd64(p + OFF_PLAYER_VIEW_2);
    if (!ok(v)) return 0;
    uint64_t m = rd64(v + OFF_CHAR_VIEW_BIPED_MAP);
    if (!ok(m)) m = rd64(v + OFF_VIEW_BIPED_MAP);
    return ok(m) ? m : 0;
}

inline uint64_t bm2(uint64_t p) {
    static const uint64_t vo[] = { 0x48, 0x50, 0x58, 0x40, 0x38, 0x60, 0x68, 0x70, 0x78, 0x30, 0x28, 0x20, 0x80, 0x88, 0x98, 0xA0 };
    static const uint64_t bo[] = { 0x48, 0x50, 0x58, 0x40, 0x38, 0x28, 0x60, 0x68, 0x70, 0x78, 0x20, 0x30, 0x18, 0x10, 0x08 };
    for (int i = 0; i < 16; i++) {
        uint64_t v = rd64(p + vo[i]);
        if (!ok(v)) continue;
        for (int j = 0; j < 15; j++) {
            uint64_t m = rd64(v + bo[j]);
            if (!ok(m)) continue;
            uint64_t b0 = rd64(m + OFF_BIPED_START);
            Vector3 t0{};
            if (!ok(b0) || !tp(b0, t0)) continue;
            float mag = fabsf(t0.x) + fabsf(t0.y) + fabsf(t0.z);
            if (mag < 0.5f) continue;
            return m;
        }
    }
    return bm(p);
}

inline bool gsb(uint64_t p, SK& s) {
    memset(&s, 0, sizeof(s));
    if (!p || p < 0x10000) return false;
    uint64_t m = bm2(p);
    if (!ok(m)) return false;
    bool hh = false;
    for (int i = 0; i < BIPED_BONE_COUNT; i++) {
        uint64_t bp = rd64(m + OFF_BIPED_START + (uint64_t)(i * OFF_BIPED_STRIDE));
        if (!ok(bp)) continue;
        s.v[i] = tp(bp, s.b[i], &s.q[i]);
        if (s.v[i] && (i == BONE_HEAD || i == BONE_NECK)) hh = true;
    }
    s.ok = true;
    return hh;
}

inline void sch(SK& s, float px, float py, float pz) {
    if (!s.v[BONE_HIP]) return;
    float hx = s.b[BONE_HIP].x, hy = s.b[BONE_HIP].y, hz = s.b[BONE_HIP].z;
    float dx = fabsf(hx - px), dy = fabsf(hy - py), dz = fabsf(hz - pz);
    if (dx <= 0.23f && dz <= 0.23f && dy <= 1.1f) return;
    float ox = px - hx, oy = py - hy + 0.9f, oz = pz - hz;
    for (int i = 0; i < BIPED_BONE_COUNT; i++)
        if (s.v[i]) { s.b[i].x += ox; s.b[i].y += oy; s.b[i].z += oz; }
}

inline bool view_matrix(float out[16]) {
    if (!g_base) return false;
    uint64_t pm = player_manager();
    if (!ok(pm)) return false;
    uint64_t lp = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp)) return false;
    uint64_t cam = rd64(lp + OFF_PLAYER_MAIN_CAMERA);
    if (!ok(cam)) return false;
    uint64_t cc = rd64(cam + 0x20);
    if (!ok(cc)) return false;
    uint64_t ct = rd64(cc + 0x10);
    if (!ok(ct)) return false;
    if (!readable(ct + OFF_CAM_MATRIX_DATA, 64)) return false;
    memcpy(out, (void*)(ct + OFF_CAM_MATRIX_DATA), 64);
    for (int i = 0; i < 16; i++)
        if (!std::isfinite(out[i])) return false;
    float pw = out[3] * out[3] + out[7] * out[7] + out[11] * out[11];
    if (!(pw > 1e-6f)) return false;
    return true;
}

inline bool w2s(const Vector3& w, const float m[16], float& sx, float& sy) {
    float X = m[0] * w.x + m[4] * w.y + m[8] * w.z + m[12];
    float Y = m[1] * w.x + m[5] * w.y + m[9] * w.z + m[13];
    float W = m[3] * w.x + m[7] * w.y + m[11] * w.z + m[15];
    if (W <= 0.0001f) return false;
    float iw = 1.f / W;
    sx = (X * iw + 1.f) * 0.5f * (float)scr_w;
    sy = (1.f - Y * iw) * 0.5f * (float)scr_h;
    return true;
}

inline bool valid_controller(uint64_t p, uint64_t lp) {
    return ok(p) && p != lp;
}

inline int try_list(uint64_t list, int layout, uint64_t lp, uint64_t* out, int cap) {
    int count;
    uint64_t buf;
    if (layout == 0) {
        count = rd32(list + 0x20);
        buf = rd64(list + 0x18);
    } else {
        count = rd32(list + 0x18);
        buf = rd64(list + 0x10);
    }
    if (count <= 0 || count > 96) return 0;
    if (!ok(buf) || buf == list) return 0;
    int m = 0;
    for (int i = 0; i < count && m < cap; i++) {
        uint64_t off = (layout == 0) ? (OFF_LIST_ENTRY_BASE + OFF_LIST_ENTRY_STRIDE * (uint64_t)i) : ((uint64_t)i << 3);
        uint64_t e = rd64(buf + off);
        if (!valid_controller(e, lp)) continue;
        out[m++] = e;
    }
    return m;
}

inline int collect_players(uint64_t pm, uint64_t lp, uint64_t* out, int cap) {
    uint64_t best[96];
    int bestn = 0;
    uint64_t la = rd64(pm + OFF_PM_PLAYER_LIST);
    if (ok(la)) {
        int n = try_list(la, 0, lp, best, cap);
        if (n > bestn) bestn = n;
        n = try_list(la, 1, lp, best, cap);
        if (n > bestn) bestn = n;
    }
    uint64_t lb = rd64(pm + 0x10);
    if (ok(lb)) {
        int n = try_list(lb, 1, lp, best, cap);
        if (n > bestn) bestn = n;
    }
    int m = 0;
    for (int i = 0; i < bestn; i++) {
        bool dup = false;
        for (int j = 0; j < m; j++) if (best[j] == best[i]) { dup = true; break; }
        if (!dup) out[m++] = best[i];
    }
    return m;
}

inline bool player_visible(uint64_t e) {
    uint64_t oc = rd64(e + OFF_PLAYER_OCCLUSION);
    if (!ok(oc)) return false;
    int cur = rd32(oc + OFF_OCCLUSION_CURRENT);
    int nxt = rd32(oc + OFF_OCCLUSION_NEXT);
    return cur == 2 && nxt != 1;
}
