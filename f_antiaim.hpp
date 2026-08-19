#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace aa {

inline bool orig_valid = false;
inline float orig_pitch = 0.f;
inline float orig_yaw = 0.f;

static float norm360(float a) {
    while (a >= 360.f) a -= 360.f;
    while (a < 0.f) a += 360.f;
    return a;
}

static float norm180(float a) {
    while (a > 180.f) a -= 360.f;
    while (a < -180.f) a += 360.f;
    return a;
}

static bool cam_angles(float& pitch, float& yaw) {
    float vm[16];
    if (!view_matrix(vm)) return false;
    float fx = vm[2], fy = vm[6], fz = vm[10];
    float m = sqrtf(fx * fx + fy * fy + fz * fz);
    if (m < 1e-4f) return false;
    fx /= m; fy /= m; fz /= m;
    float s = fy;
    if (s < -1.f) s = -1.f;
    if (s > 1.f) s = 1.f;
    pitch = -asinf(s) * 57.2957795f;
    yaw = atan2f(fx, fz) * 57.2957795f;
    return true;
}

static void move_fix(float oyaw, float ayaw, void* cmd) {
    if (!cmd) return;
    uint64_t base = (uintptr_t)cmd;
    float h = *(float*)(base + 0x10);
    float v = *(float*)(base + 0x14);
    if (h == 0.f && v == 0.f) return;
    float f1 = norm360(oyaw);
    float f2 = norm360(ayaw);
    float delta = f2 - f1;
    if (delta > 180.f) delta -= 360.f;
    else if (delta < -180.f) delta += 360.f;
    float rad = delta * 0.0174532925f;
    float c = cosf(rad);
    float s = sinf(rad);
    float fx = c * h - s * v;
    float fy = s * h + c * v;
    float len = sqrtf(fx * fx + fy * fy);
    if (len > 1.f) { fx /= len; fy /= len; }
    *(float*)(base + 0x10) = fx;
    *(float*)(base + 0x14) = fy;
}

static void tick(void* cmd) {
    if (!opt_aa) {
        if (orig_valid) orig_valid = false;
        return;
    }
    if (!cmd) return;
    uint64_t pm = player_manager();
    if (!ok(pm)) return;
    uint64_t lp = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp)) return;
    uint64_t aim = rd64(lp + OFF_PLAYER_AIM);
    if (!ok(aim)) return;
    uint64_t ad = rd64(aim + OFF_AIM_AIMING_DATA);
    if (!ok(ad)) return;

    if (!orig_valid) {
        float vp = 0.f, vy = 0.f;
        if (cam_angles(vp, vy)) {
            orig_pitch = vp;
            orig_yaw = vy;
        } else {
            orig_pitch = rdf(ad + OFF_AIMING_CUR_AIM_ANG);
            orig_yaw = rdf(ad + OFF_AIMING_CUR_EULER_ANG + 4);
        }
        orig_valid = true;
    } else {
        orig_pitch -= rdf((uintptr_t)cmd + 0x28);
        orig_yaw += rdf((uintptr_t)cmd + 0x2C);
    }
    orig_yaw = norm180(orig_yaw);

    float pitch = 0.f;
    switch (s_aa_pitch) {
        case 1: pitch = -70.f; break;
        case 2: pitch = 70.f; break;
        default: pitch = 0.f; break;
    }

    bool firing = *(uint8_t*)((uintptr_t)cmd + 0x21) != 0;
    bool jumping = *(uint8_t*)((uintptr_t)cmd + 0x22) != 0;

    float fire_p = orig_pitch, fire_y = orig_yaw;

    float yaw = orig_yaw;
    if (jumping) {
        static float spin = 0.f;
        float base_yaw = 0.f;
        switch (s_aa_yaw) {
            case 1: base_yaw = 165.f; break;
            case 2: base_yaw = spin; break;
            case 3:
                pitch = (float)(rand() % 179 - 89);
                base_yaw = (float)(rand() % 360);
                break;
            default:
                base_yaw = 0.f;
                break;
        }
        if (s_aa_speed != 0.f) {
            spin += s_aa_speed;
            if (spin >= 360.f) spin -= 360.f;
            else if (spin < 0.f) spin += 360.f;
            base_yaw = spin;
        } else if (s_aa_jitter) {
            static int f = 0;
            static bool flip = false;
            if (f >= 6) { f = 0; flip = !flip; }
            ++f;
            base_yaw = norm360(base_yaw + (flip ? s_aa_range : -s_aa_range));
        }
        yaw = norm180(orig_yaw + base_yaw);
    }

    if (pitch > 70.f) pitch = 70.f;
    else if (pitch < -70.f) pitch = -70.f;

    if (firing) {
        wrf(ad + OFF_AIMING_CUR_AIM_ANG, fire_p);
        wrf(ad + OFF_AIMING_CUR_AIM_ANG + 4, fire_y);
        wrf(ad + OFF_AIMING_CUR_EULER_ANG, fire_p);
        wrf(ad + OFF_AIMING_CUR_EULER_ANG + 4, fire_y);
    } else {
        wrf(ad + OFF_AIMING_CUR_AIM_ANG, pitch);
        wrf(ad + OFF_AIMING_CUR_AIM_ANG + 4, yaw);
        wrf(ad + OFF_AIMING_CUR_EULER_ANG, pitch);
        wrf(ad + OFF_AIMING_CUR_EULER_ANG + 4, yaw);
    }
    move_fix(orig_yaw, yaw, cmd);
}

static void camera(uint64_t p) {
    if (!opt_aa || !orig_valid) return;
    if (!ok(p)) return;
    uint64_t ch = rd64(p + OFF_PLAYER_CAMERA_HOLDER);
    if (!ok(ch)) return;
    if (!cgt || !tse) return;
    void* t = cgt((void*)ch);
    if (!t) return;
    Vector3 e;
    e.x = orig_pitch;
    e.y = orig_yaw;
    e.z = 0.f;
    tse(t, e);
}

static void cm(void* obj, void* cmd) {
    (void)obj;
    tick(cmd);
}

static void late(void* p, bool local) {
    if (!local) return;
    camera((uint64_t)p);
}

}

namespace {
static events::feature _f_aa = {"antiaim", nullptr, aa::late, aa::cm, nullptr, nullptr};
static struct _reg_aa { _reg_aa() { events::register_feature(_f_aa); } } _r_aa;
}
