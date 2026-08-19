#pragma once

#include "../resolve.hpp"

inline const Il2CppMethod* pc_update_m = nullptr;
inline const Il2CppField* cm_delegate_field = nullptr;
inline void* orig_create_move = nullptr;

namespace sdk::so2 {

inline bool ctrls_ok = false;

inline bool ctrls_resolve() {
    if (ctrls_ok) return true;
    Il2CppClass* c = sdk::class_lazy("Assembly-CSharp", "Axlebolt.Standoff.Controls", "PlayerControls");
    if (!c) return false;
    if (!pc_update_m) {
        const Il2CppMethod* um = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(c, "Update", 0) : nullptr;
        if (!um) um = find_method(c, "Update");
        pc_update_m = um;
    }
    if (!cm_delegate_field && il2cpp::class_get_fields && il2cpp::field_get_type &&
        il2cpp::class_from_type && il2cpp::class_get_name) {
        void* fit = nullptr;
        const Il2CppField* f = nullptr;
        while ((f = il2cpp::class_get_fields(c, &fit))) {
            const Il2CppType* ft = il2cpp::field_get_type(f);
            if (!ft) continue;
            Il2CppClass* fc = il2cpp::class_from_type(ft);
            if (!fc) continue;
            const char* fcn = il2cpp::class_get_name(fc);
            if (!fcn || !strstr(fcn, "CreateMove")) continue;
            cm_delegate_field = f;
            break;
        }
    }
    ctrls_ok = pc_update_m && cm_delegate_field;
    return ctrls_ok;
}

inline void ctrls_reset() {
    pc_update_m = nullptr;
    cm_delegate_field = nullptr;
    orig_create_move = nullptr;
    ctrls_ok = false;
}

}
