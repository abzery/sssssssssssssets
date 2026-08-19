#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace fog {

inline bool applied = false;
inline bool captured = false;
inline bool orig_on = false;
inline NC4 orig_c = { 0.5f, 0.5f, 0.5f, 1.f };
inline float orig_start = 0.f;
inline float orig_end = 0.f;

static void capture() {
    if (captured) return;
    captured = true;
    if (rs_get_fog) orig_on = rs_get_fog() ? true : false;
    if (rs_get_fog_color) orig_c = rs_get_fog_color();
    if (rs_get_fog_start) orig_start = rs_get_fog_start();
    if (rs_get_fog_end) orig_end = rs_get_fog_end();
}

static void restore() {
    if (rs_set_fog) rs_set_fog(orig_on ? 1 : 0);
    if (rs_set_fog_color) rs_set_fog_color(orig_c);
    if (rs_set_fog_start) rs_set_fog_start(orig_start);
    if (rs_set_fog_end) rs_set_fog_end(orig_end);
    captured = false;
    applied = false;
}

static void apply() {
    if (!captured) capture();
    if (rs_set_fog) rs_set_fog(1);
    if (rs_set_fog_mode) rs_set_fog_mode(1);
    NC4 c = { s_fog_col.x, s_fog_col.y, s_fog_col.z, 1.f };
    if (rs_set_fog_color) rs_set_fog_color(c);
    if (rs_set_fog_start) rs_set_fog_start(s_fog_start);
    if (rs_set_fog_end) rs_set_fog_end(s_fog_end);
}

static void late(void* p, bool local) {
    (void)p;
    if (!local) return;
    if (!opt_fog) {
        if (applied) restore();
        return;
    }
    if (!sdk::unity::rset_fog_ok) {
        static bool flog = false;
        if (!flog) {
            flog = true;
            LOG("fog: pending rset_fog_ok=0");
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
}

}

namespace {
static events::feature _f_fog = {"fog", nullptr, fog::late, nullptr, nullptr, fog::reset};
static struct _reg_fog { _reg_fog() { events::register_feature(_f_fog); } } _r_fog;
}
