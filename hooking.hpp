#pragma once

#include "core.hpp"
#include "events.hpp"
#include "sdk/include.h"
#include "features/f_include.hpp"

namespace hooking {

inline bool lu_hooked = false;
inline bool lu_page_rw = false;
inline bool pc_hooked = false;
inline bool pc_page_rw = false;
inline void* orig_pc_update = nullptr;

static void hk_lu(void* p);

static void hk_lu(void* p) {
    static bool hkl = false;
    if (!hkl) { hkl = true; LOG("hk_lu first call p=%p", p); }
    bool local = false;
    if (p && ok((uint64_t)p)) {
        uint64_t pm = player_manager();
        if (ok(pm) && (uint64_t)p == rd64(pm + OFF_PM_LOCAL_PLAYER)) local = true;
    }
    events::fire_pre_late(p, local);
    if (lu_mp) ((void (*)(void*))lu_mp)(p);
    events::fire_late(p, local);
}

static bool hook_lu() {
    if (!lu_mi) return false;
    uintptr_t* s = (uintptr_t*)((uintptr_t)lu_mi + il2cpp::offset::il2cpp_method_pointer);
    if (!ok(*s)) return false;
    if (!lu_page_rw && !mi_make_rw((uintptr_t)s)) return false;
    if (!lu_mp) lu_mp = (void*)*s;
    *s = (uintptr_t)hk_lu;
    return true;
}

static void try_hook_lu() {
    if (!lu_mi) {
        static bool nlf = false;
        if (!nlf) { nlf = true; LOG("hooking: resolve lu FAILED"); }
        return;
    }
    if (lu_hooked) {
        if (*(uintptr_t*)((uintptr_t)lu_mi + 0x8) == (uintptr_t)hk_lu) return;
        LOG("hooking: lateupdate lost, re-applying");
        lu_hooked = false;
    }
    if (hook_lu()) {
        lu_hooked = true;
        static bool hla = false;
        if (!hla) { hla = true; LOG("hooking: lu applied mi=%p mp=%p", (void*)lu_mi, (void*)lu_mp); }
    } else {
        static bool hlf = false;
        if (!hlf) { hlf = true; LOG("hooking: lu hook FAILED mi=%p", (void*)lu_mi); }
    }
}

static void hk_create_move(void* obj, void* cmd) {
    events::fire_create_move(obj, cmd);
    if (orig_create_move) ((void (*)(void*, void*))orig_create_move)(obj, cmd);
}

static void hook_create_move(void* thiz) {
    void* dlg = nullptr;
    if (cm_delegate_field && il2cpp::field_get_value)
        il2cpp::field_get_value((const Il2CppObject*)thiz, cm_delegate_field, &dlg);
    else
        dlg = *(void**)((uintptr_t)thiz + 0xC0);
    if (!dlg) return;
    void* invoke = *(void**)((uintptr_t)dlg + 0x18);
    if (!invoke) return;
    static bool hcm = false;
    if (!hcm) { hcm = true; LOG("hooking: create_move hooked dlg=%p invoke=%p", dlg, invoke); }
    if (!orig_create_move) orig_create_move = invoke;
    *(void**)((uintptr_t)dlg + 0x18) = (void*)hk_create_move;
}

static void hk_pc_update(void* thiz) {
    static bool pcok = false;
    if (!pcok) { pcok = true; LOG("hooking: hk_pc_update first call thiz=%p", thiz); }
    if (thiz) hook_create_move(thiz);
    if (orig_pc_update) ((void (*)(void*))orig_pc_update)(thiz);
}

static bool hook_pc() {
    if (!pc_update_m) return false;
    uintptr_t* s = (uintptr_t*)((uintptr_t)pc_update_m + il2cpp::offset::il2cpp_method_pointer);
    if (!ok(*s)) return false;
    if (!pc_page_rw && !mi_make_rw((uintptr_t)s)) return false;
    if (!orig_pc_update) orig_pc_update = (void*)*s;
    *s = (uintptr_t)hk_pc_update;
    return true;
}

static void try_hook_pc() {
    if (!pc_update_m) {
        static bool npf = false;
        if (!npf) { npf = true; LOG("hooking: pc_update_m null"); }
        return;
    }
    if (pc_hooked) {
        if (*(uintptr_t*)((uintptr_t)pc_update_m + 0x8) == (uintptr_t)hk_pc_update) return;
        LOG("hooking: pc hook lost, re-applying");
        pc_hooked = false;
    }
    if (hook_pc()) {
        pc_hooked = true;
        static bool hp = false;
        if (!hp) {
            hp = true;
            LOG("hooking: pc hook applied mi=%p mp=%p", (void*)pc_update_m,
                pc_update_m ? *(void**)((uintptr_t)pc_update_m + 0x8) : nullptr);
        }
    } else {
        static bool hpf = false;
        if (!hpf) {
            hpf = true;
            LOG("hooking: pc hook FAILED mi=%p mp=%llx",
                (void*)pc_update_m,
                (unsigned long long)(pc_update_m ? *(uintptr_t*)((uintptr_t)pc_update_m + 0x8) : 0));
        }
    }
}

static void pump() {
    std::lock_guard<std::mutex> lg(g_hook_mtx);
    if (!g_base) return;
    if (!il2cpp::domain_get) il2cpp::init_api(g_base);
    Il2CppDomain* d = il2cpp::domain_get();
    if (d && il2cpp::thread_attach) il2cpp::thread_attach(d);
    if (!sdk::so2::pctrl_resolve()) return;
    if (!sdk::so2::ctrls_resolve()) {
        static bool cfl = false;
        if (!cfl) { cfl = true; LOG("hooking: ctrls resolve pending"); }
    }
    sdk::unity::comp_resolve();
    sdk::unity::cam_resolve();
    sdk::unity::tr_resolve();
    sdk::unity::time_resolve();
    sdk::unity::re_resolve();
    sdk::unity::mat_resolve();
    sdk::unity::sh_resolve();
    sdk::unity::beh_resolve();
    sdk::unity::rset_resolve();
    sdk::unity::light_resolve();
    sdk::unity::obj_resolve();
    static bool sd = false;
    if (!sd) {
        sd = true;
        LOG("sdk: comp=%d cam=%d tr=%d time=%d re=%d mat=%d sh=%d beh=%d rset=%d light=%d obj=%d",
            (int)sdk::unity::comp_ok, (int)sdk::unity::cam_ok,
            (int)sdk::unity::tr_ok, (int)sdk::unity::time_ok,
            (int)sdk::unity::re_ok, (int)sdk::unity::mat_ok,
            (int)sdk::unity::sh_ok, (int)sdk::unity::beh_ok,
            (int)sdk::unity::rset_ok, (int)sdk::unity::light_ok,
            (int)sdk::unity::obj_ok);
    }
    try_hook_lu();
    try_hook_pc();
}

static void reset() {
    std::lock_guard<std::mutex> lg(g_hook_mtx);
    lu_hooked = false;
    lu_page_rw = false;
    pc_hooked = false;
    pc_page_rw = false;
    orig_pc_update = nullptr;
    sdk::reset_all();
    events::fire_reset();
}

}
