#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace sky {

inline bool applied = false;
inline bool cam_captured = false;
inline int orig_clear = 2;
inline NC4 orig_bg = { 0.5f, 0.6f, 0.8f, 1.f };
inline void* s_cam_prev = nullptr;

static void capture() {
    if (cam_captured) return;
    cam_captured = true;
    void* c = cm ? cm() : nullptr;
    if (!c) return;
    s_cam_prev = c;
    if (cam_get_clearflags) orig_clear = cam_get_clearflags(c);
}

static void restore() {
    if (cm) {
        void* c = cm();
        if (c) {
            if (cam_set_clearflags) cam_set_clearflags(c, orig_clear);
            if (cam_set_bgcolor) cam_set_bgcolor(c, orig_bg);
        }
    }
    cam_captured = false;
    applied = false;
}

static void apply() {
    void* c = cm ? cm() : nullptr;
    if (!c) return;
    if (c != s_cam_prev) {
        s_cam_prev = c;
        cam_captured = false;
        capture();
    }
    if (cam_set_clearflags) cam_set_clearflags(c, 2);
    if (cam_set_bgcolor) {
        NC4 bg = { s_sky_col.x, s_sky_col.y, s_sky_col.z, 1.f };
        cam_set_bgcolor(c, bg);
    }
}

static void late(void* p, bool local) {
    (void)p;
    if (!local) return;
    if (!opt_sky) {
        if (applied) restore();
        return;
    }
    if (!sdk::unity::cam_ok) {
        static bool skylog = false;
        if (!skylog) {
            skylog = true;
            LOG("sky: pending cam_ok=0");
        }
        return;
    }
    if (!applied) {
        capture();
        applied = true;
    }
    apply();
}

static void reset() {
    if (applied) restore();
    s_cam_prev = nullptr;
}

}

namespace {
static events::feature _f_sky = {"sky", nullptr, sky::late, nullptr, nullptr, sky::reset};
static struct _reg_sky { _reg_sky() { events::register_feature(_f_sky); } } _r_sky;
}
