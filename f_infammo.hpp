#pragma once

#include "../core.hpp"
#include "../events.hpp"

namespace infammo {

#define IAMMO_WP_AMMUNITION    0x130
#define IAMMO_AMMO_MAG_CAP     0x10
#define IAMMO_AMMO_CAP         0x12
#define IAMMO_AMMO_MAG_CAP_SV  0x14
#define IAMMO_AMMO_CAP_SV      0x20
#define IAMMO_AMMO_VALUE       9999

static void tick(uint64_t p) {
    if (!opt_inf_ammo) return;
    if (!ok(p)) return;
    uint64_t wr = rd64(p + OFF_PLAYER_WEAPONRY);
    if (!ok(wr)) return;
    uint64_t wc = rd64(wr + OFF_WEAPONRY_CURRENT);
    if (!ok(wc)) return;
    uint64_t pr = rd64(wc + OFF_WC_WEAPON_PROPS);
    if (!ok(pr)) return;
    uint64_t am = rd64(pr + IAMMO_WP_AMMUNITION);
    if (!ok(am)) return;

    if (readable(am + IAMMO_AMMO_MAG_CAP, 2))
        *(uint16_t*)(am + IAMMO_AMMO_MAG_CAP) = IAMMO_AMMO_VALUE;
    if (readable(am + IAMMO_AMMO_CAP, 2))
        *(uint16_t*)(am + IAMMO_AMMO_CAP) = IAMMO_AMMO_VALUE;

    uint8_t sv[12] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    *(int*)(sv + 8) = IAMMO_AMMO_VALUE;

    if (readable(am + IAMMO_AMMO_MAG_CAP_SV, sizeof(sv)))
        memcpy((void*)(am + IAMMO_AMMO_MAG_CAP_SV), sv, sizeof(sv));
    if (readable(am + IAMMO_AMMO_CAP_SV, sizeof(sv)))
        memcpy((void*)(am + IAMMO_AMMO_CAP_SV), sv, sizeof(sv));
}

static void late(void* p, bool local) {
    if (!local) return;
    tick((uint64_t)p);
}

}

namespace {
static events::feature _f_infammo = {"infammo", nullptr, infammo::late, nullptr, nullptr, nullptr};
static struct _reg_infammo { _reg_infammo() { events::register_feature(_f_infammo); } } _r_infammo;
}
