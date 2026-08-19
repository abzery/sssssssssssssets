#pragma once

#include "../resolve.hpp"

inline Il2CppClass* pc_cls = nullptr;
inline MethodInfo* lu_mi = nullptr;
inline void* lu_mp = nullptr;
inline void* vm_apply = nullptr;
inline void* vm_settps2 = nullptr;
inline void* vm_set_visible = nullptr;

namespace sdk::so2 {

inline bool pctrl_ok = false;

inline bool pctrl_resolve() {
    if (pctrl_ok) return true;
    Il2CppClass* c = sdk::class_lazy("Assembly-CSharp", "Axlebolt.Standoff.Player", "PlayerController");
    if (!c) return false;
    pc_cls = c;
    const Il2CppMethod* lm = il2cpp::class_get_method_from_name ? il2cpp::class_get_method_from_name(c, "LateUpdate", 0) : nullptr;
    if (!lm) lm = find_method(c, "LateUpdate");
    if (lm) {
        lu_mi = (MethodInfo*)lm;
        lu_mp = sdk::m_ptr(lm);
    }
    if (il2cpp::class_get_methods && il2cpp::method_get_name) {
        void* it = nullptr;
        while (const Il2CppMethod* m = il2cpp::class_get_methods(c, &it)) {
            const char* nm = il2cpp::method_get_name(m);
            if (!nm) continue;
            if (!vm_apply && strcmp(nm, "ADEAAACFHADDAFG") == 0) vm_apply = sdk::m_ptr(m);
            else if (!vm_settps2 && strcmp(nm, "EHDGAFDBHCAECDH") == 0) vm_settps2 = sdk::m_ptr(m);
            else if (!vm_set_visible && strcmp(nm, "DDAEGGGBBCBGGGF") == 0) vm_set_visible = sdk::m_ptr(m);
        }
    }
    pctrl_ok = lu_mi && lu_mp;
    return pctrl_ok;
}

inline void pctrl_reset() {
    pc_cls = nullptr;
    lu_mi = nullptr;
    lu_mp = nullptr;
    vm_apply = nullptr;
    vm_settps2 = nullptr;
    vm_set_visible = nullptr;
    pctrl_ok = false;
}

}
