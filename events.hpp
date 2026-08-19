#pragma once

#include <vector>

namespace events {

using pre_late_fn = void (*)(void*, bool);
using late_fn = void (*)(void*, bool);
using create_move_fn = void (*)(void*, void*);
using render_fn = void (*)();
using reset_fn = void (*)();

struct feature {
    const char* name;
    pre_late_fn pre;
    late_fn late;
    create_move_fn cm;
    render_fn render;
    reset_fn reset;
};

inline std::vector<feature> registry;

inline void register_feature(const feature& f) {
    registry.push_back(f);
}

inline void fire_pre_late(void* p, bool local) {
    for (size_t i = 0; i < registry.size(); i++)
        if (registry[i].pre) registry[i].pre(p, local);
}

inline void fire_late(void* p, bool local) {
    for (size_t i = 0; i < registry.size(); i++)
        if (registry[i].late) registry[i].late(p, local);
}

inline void fire_create_move(void* obj, void* cmd) {
    for (size_t i = 0; i < registry.size(); i++)
        if (registry[i].cm) registry[i].cm(obj, cmd);
}

inline void fire_render() {
    for (size_t i = 0; i < registry.size(); i++)
        if (registry[i].render) registry[i].render();
}

inline void fire_reset() {
    for (size_t i = 0; i < registry.size(); i++)
        if (registry[i].reset) registry[i].reset();
}

}
