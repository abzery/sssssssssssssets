#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../sdk/include.h"

namespace tps {

static void tick_view(uint64_t p) {
    if (!opt_tps) return;
    if (rd32((uint64_t)p + 0x134) == 2) return;
    if (vm_settps2) ((void(*)(void*))vm_settps2)((void*)p);
    if (vm_apply) ((void(*)(void*, int))vm_apply)((void*)p, 2);
}

static void tps_move(uint64_t p) {
    if (!cm || !cgt || !tgf || !tsp) return;
    Vector3 pos = player_pos(p);
    if (!(pos.x > -20000 && pos.x < 20000 && pos.y > -20000 && pos.y < 20000 && pos.z > -20000 && pos.z < 20000)) return;
    void* c = cm(); if (!c) return;
    void* t = cgt(c); if (!t) return;
    Vector3 f = tgf(t);
    if (!(f.x > -10 && f.x < 10 && f.y > -10 && f.y < 10 && f.z > -10 && f.z < 10)) return;
    if (f.x * f.x + f.y * f.y + f.z * f.z < 0.01f) return;
    float td = 3.0f;
    Vector3 s;
    s.x = pos.x + f.x * -td;
    s.y = pos.y + 1.5f + f.y * -td;
    s.z = pos.z + f.z * -td;
    static Vector3 sm;
    sm.x += (s.x - sm.x) * 0.688f;
    sm.y += (s.y - sm.y) * 0.688f;
    sm.z += (s.z - sm.z) * 0.688f;
    if (!(sm.x > -20000 && sm.x < 20000 && sm.y > -20000 && sm.y < 20000 && sm.z > -20000 && sm.z < 20000))
        sm = s;
    tsp(t, sm);
}

inline int tps_vis_state = 0;

static void sync_view(uint64_t p) {
    if (tps_vis_state != opt_tps) {
        tps_vis_state = opt_tps;
        int cur = rd32((uint64_t)p + 0x134);
        uint64_t cfps = rd64((uint64_t)p + 0x50);
        if (!opt_tps && cur != 1) {
            if (vm_apply) ((void(*)(void*, int))vm_apply)((void*)p, 1);
            if (cfps) wr32(cfps + 0x30, 1);
        }
        wr32((uint64_t)p + 0x134, opt_tps ? 2 : 1);
    }
}

static void pre(void* p, bool local) {
    if (!local) return;
    tick_view((uint64_t)p);
}

static void late(void* p, bool local) {
    if (!local) return;
    uint64_t pp = (uint64_t)p;
    if (opt_tps) tps_move(pp);
    sync_view(pp);
}

static void reset() {
    tps_vis_state = 0;
}

}

namespace {
static events::feature _f_tps = {"tps", tps::pre, tps::late, nullptr, nullptr, tps::reset};
static struct _reg_tps { _reg_tps() { events::register_feature(_f_tps); } } _r_tps;
}
