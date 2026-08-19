#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace aimbot {

inline bool has_target = false;

static inline float normalize_yaw(float yaw) {
    while (yaw > 180.f) yaw -= 360.f;
    while (yaw < -180.f) yaw += 360.f;
    return yaw;
}

static inline float fov_radius_px(float fov_deg, float screen_h) {
    if (screen_h <= 0.f) return 0.f;
    float clamped = (fov_deg < 1.f) ? 1.f : (fov_deg > 179.f) ? 179.f : fov_deg;
    float half_v = (70.0f * 0.5f) * (3.14159265f / 180.f);
    float half_a = (clamped * 0.5f) * (3.14159265f / 180.f);
    float tan_v = tanf(half_v);
    if (fabsf(tan_v) < 0.00001f) return 0.f;
    return ((screen_h * 0.5f) / tan_v) * tanf(half_a);
}

static inline float calc_dist(const Vector3& a, const Vector3& b) {
    float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline int aim_bone_idx() {
    switch (s_aim_bone) {
        case 0: return BONE_HEAD;
        case 1: return BONE_NECK;
        case 2: return BONE_SPINE2;
        case 3: return BONE_HIP;
        default: return BONE_HEAD;
    }
}

static bool compute_aim(uint64_t lp, float fov_deg, int bone_idx, bool vis_only, float& pitch, float& yaw) {
    if (!ok(lp)) return false;
    uint64_t pm = player_manager();
    if (!ok(pm)) return false;
    uint64_t lp2 = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp2) || lp2 != lp) return false;
    Vector3 lp_pos = player_pos(lp2);
    int lteam = rd8(lp2 + OFF_PLAYER_TEAM);
    float vm[16];
    if (!view_matrix(vm)) return false;
    float cx = (float)scr_w * 0.5f, cy = (float)scr_h * 0.5f;
    float rad = fov_radius_px(fov_deg, (float)scr_h);
    float bd = rad + 1.f;
    Vector3 bw{};
    bool fd = false;

    uint64_t players[96];
    int np = collect_players(pm, lp2, players, 96);
    for (int i = 0; i < np; i++) {
        uint64_t e = players[i];
        if (!ok(e)) continue;
        if (rd8(e + OFF_PLAYER_TEAM) == lteam) continue;
        if (health_of(e) <= 0) continue;
        if (vis_only && !player_visible(e)) continue;
        Vector3 pp = player_pos(e);
        if (!(pp.x > -20000.f && pp.x < 20000.f && pp.y > -20000.f && pp.y < 20000.f && pp.z > -20000.f && pp.z < 20000.f)) continue;
        SK s;
        if (!gsb(e, s) || !s.ok) continue;
        if (!s.v[bone_idx]) continue;
        Vector3 bone = s.b[bone_idx];
        if (bone_idx == BONE_HEAD) bone.y -= 0.25f;
        else if (bone_idx == BONE_NECK) bone.y -= 0.30f;
        else if (bone_idx == BONE_SPINE2) bone.y -= 0.25f;
        else if (bone_idx == BONE_HIP) bone.y -= 0.30f;
        if (!sane_world_pos(bone)) continue;
        if (calc_dist(bone, pp) > 3.f) continue;
        float sx, sy;
        if (!w2s(bone, vm, sx, sy)) continue;
        float dx = sx - cx, dy = sy - cy;
        float d = sqrtf(dx * dx + dy * dy);
        if (d <= rad && d < bd) { bd = d; bw = bone; fd = true; }
    }
    has_target = fd;
    if (!fd) return false;

    Vector3 cam(lp_pos.x, lp_pos.y + 1.18f, lp_pos.z);
    float dx = bw.x - cam.x, dy = bw.y - cam.y, dz = bw.z - cam.z;
    float dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist < 0.001f) return false;
    float s = dy / dist;
    if (s < -1.f) s = -1.f;
    if (s > 1.f) s = 1.f;
    pitch = -asinf(s) * 57.2957795f;
    yaw = normalize_yaw(atan2f(dx, dz) * 57.2957795f);
    return true;
}

static void tick(uint64_t lp) {
    if (!opt_aim || !ok(lp)) return;
    float pitch = 0.f, yaw = 0.f;
    if (!compute_aim(lp, s_aim_fov, aim_bone_idx(), opt_aim_visible, pitch, yaw)) return;
    uint64_t pm = player_manager();
    if (!ok(pm)) return;
    uint64_t lp2 = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp2)) return;
    uint64_t ac = rd64(lp2 + OFF_PLAYER_AIM);
    if (!ok(ac)) return;
    uint64_t ad = rd64(ac + OFF_AIM_AIMING_DATA);
    if (!ok(ad)) return;

    float op = pitch, oy = yaw;

    if (s_aim_smooth > 0.0001f) {
        float cp = rdf(ad + OFF_AIMING_CUR_AIM_ANG);
        float cy2 = normalize_yaw(rdf(ad + OFF_AIMING_CUR_AIM_ANG + 4));
        float t = 1.f - s_aim_smooth;
        if (t < 0.01f) t = 0.01f;
        if (t > 0.99f) t = 0.99f;
        op = cp + (pitch - cp) * t;
        oy = normalize_yaw(cy2 + normalize_yaw(yaw - cy2) * t);
    }

    wrf(ad + OFF_AIMING_CUR_AIM_ANG, op);
    wrf(ad + OFF_AIMING_CUR_AIM_ANG + 4, oy);
    wrf(ad + OFF_AIMING_CUR_AIM_ANG + 8, 0.f);
    wrf(ad + OFF_AIMING_CUR_EULER_ANG, op);
    wrf(ad + OFF_AIMING_CUR_EULER_ANG + 4, oy);
    wrf(ad + OFF_AIMING_CUR_EULER_ANG + 8, 0.f);
}

static void draw_fov() {
    if (!opt_aim || !opt_aim_fov_draw || scr_w <= 0 || scr_h <= 0) return;
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    float cx = (float)scr_w * 0.5f, cy = (float)scr_h * 0.5f;
    float rad = fov_radius_px(s_aim_fov, (float)scr_h);
    dl->AddCircle(ImVec2(cx, cy), rad, IM_COL32(0, 0, 0, 130), 64, 2.f);
    dl->AddCircle(ImVec2(cx, cy), rad, IM_COL32(255, 255, 255, 180), 64, 1.f);
}

static void late(void* p, bool local) {
    if (!local) return;
    tick((uint64_t)p);
}

static void render() {
    draw_fov();
}

}

namespace {
static events::feature _f_aimbot = {"aimbot", nullptr, aimbot::late, nullptr, aimbot::render, nullptr};
static struct _reg_aimbot { _reg_aimbot() { events::register_feature(_f_aimbot); } } _r_aimbot;
}
