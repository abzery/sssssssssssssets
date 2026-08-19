#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdarg>
#include <mutex>
#include <atomic>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "font.h"
#include "offsets.hpp"
#include "vector3.h"
#include "il2cpp.hpp"
#include "../ui/menu.hpp"
#include "../ui/watermark.hpp"

inline uintptr_t g_base = 0;
inline int scr_w = 0, scr_h = 0;
inline std::mutex g_mtx;
inline std::mutex g_hook_mtx;

#define LOG(...) ((void)0)

static inline bool ok(uint64_t p) {
    return p > 0x10000 && p < 0x0000FFFFFFFFFFFFull;
}

struct map_range { uint64_t start, end; };
inline std::vector<map_range> g_maps;
inline std::mutex g_mtx_maps;

static inline long sc6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a1;
    register long x1 __asm__("x1") = a2;
    register long x2 __asm__("x2") = a3;
    register long x3 __asm__("x3") = a4;
    register long x4 __asm__("x4") = a5;
    register long x5 __asm__("x5") = a6;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory", "cc");
    return x0;
}
#define MAPS_AT_FDCWD (-100)
#define MAPS_SYS_OPENAT 56
#define MAPS_SYS_READ 63
#define MAPS_SYS_CLOSE 57
#define MAPS_O_RDONLY 0

static inline int maps_open(const char* p) { return (int)sc6(MAPS_SYS_OPENAT, MAPS_AT_FDCWD, (long)p, MAPS_O_RDONLY, 0, 0, 0); }
static inline void maps_close(int fd) { sc6(MAPS_SYS_CLOSE, fd, 0, 0, 0, 0, 0); }
static inline long maps_read(int fd, void* b, size_t c) { return sc6(MAPS_SYS_READ, fd, (long)b, c, 0, 0, 0); }

static size_t read_file_raw(const char* path, char* buf, size_t cap) {
    int fd = maps_open(path);
    if (fd < 0) return 0;
    size_t tot = 0;
    while (tot < cap - 1) {
        long n = maps_read(fd, buf + tot, cap - 1 - tot);
        if (n <= 0) break;
        tot += (size_t)n;
    }
    maps_close(fd);
    buf[tot] = 0;
    return tot;
}

static inline int hexv(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
static inline int ishex(char c) { return hexv(c) >= 0; }

static char* load_maps_full(size_t* out_len) {
    const size_t cap = 2u << 20;
    char* buf = (char*)malloc(cap);
    if (!buf) { *out_len = 0; return nullptr; }
    size_t n = read_file_raw("/proc/self/maps", buf, cap);
    *out_len = n;
    if (!n) { free(buf); return nullptr; }
    return buf;
}

static void build_maps() {
    std::vector<map_range> tmp;
    size_t n = 0;
    char* buf = load_maps_full(&n);
    if (!buf) return;
    char* p = buf;
    while (*p) {
        char* le = p;
        while (*le && *le != '\n') le++;
        if (*le) *le = 0;
        uint64_t st = 0, en = 0;
        int k = 0;
        while (ishex(p[k])) { st = st * 16 + (uint64_t)hexv(p[k]); k++; }
        if (p[k] == '-') {
            k++;
            while (ishex(p[k])) { en = en * 16 + (uint64_t)hexv(p[k]); k++; }
        }
        while (p[k] == ' ') k++;
        if (p[k] == 'r') tmp.push_back({st, en});
        p = le + 1;
    }
    free(buf);
    std::lock_guard<std::mutex> lk(g_mtx_maps);
    g_maps.swap(tmp);
}

static inline bool readable(uint64_t a, size_t n) {
    if (!a || n == 0 || a < 0x1000) return false;
    uint64_t end = a + n;
    if (end < a) return false;
    std::lock_guard<std::mutex> lk(g_mtx_maps);
    for (size_t i = 0; i < g_maps.size(); i++) {
        if (a >= g_maps[i].start && end <= g_maps[i].end) return true;
        if (a < g_maps[i].start) return false;
    }
    return false;
}

static inline uint64_t rd64(uint64_t a) { return readable(a, 8) ? *(uint64_t*)a : 0; }
static inline int32_t rd32(uint64_t a) { return readable(a, 4) ? *(int32_t*)a : 0; }
static inline uint8_t rd8(uint64_t a) { return readable(a, 1) ? *(uint8_t*)a : 0; }
static inline uint16_t rd16(uint64_t a) { return readable(a, 2) ? *(uint16_t*)a : 0; }
static inline void wr8(uint64_t a, uint8_t v) { if (readable(a, 1)) *(uint8_t*)a = v; }
static inline void wr32(uint64_t a, int32_t v) { if (readable(a, 4)) *(int32_t*)a = v; }
static inline bool obj_ok(uint64_t a) {
    if (!readable(a, 8)) return false;
    uint64_t k = *(uint64_t*)a;
    return readable(k, 8) && k > 0x1000000;
}
static inline float rdf(uint64_t a) { return readable(a, 4) ? *(float*)a : 0.f; }
static inline void wrf(uint64_t a, float v) { if (readable(a, 4)) *(float*)a = v; }
static inline void rdv(uint64_t a, Vector3& v) {
    if (readable(a, sizeof(Vector3))) memcpy(&v, (void*)a, sizeof(Vector3));
    else memset(&v, 0, sizeof(Vector3));
}
static inline bool rdb(uint64_t a, void* buf, size_t n) {
    if (!readable(a, n)) return false;
    memcpy(buf, (void*)a, n);
    return true;
}
static inline bool sane_world_pos(const Vector3& v) {
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z) &&
           fabsf(v.x) < 100000.f && fabsf(v.y) < 100000.f && fabsf(v.z) < 100000.f &&
           !(v.x == 0.f && v.y == 0.f && v.z == 0.f);
}

static const Il2CppMethod* find_method(Il2CppClass* c, const char* name) {
    if (!c || !il2cpp::class_get_methods || !il2cpp::method_get_name) return nullptr;
    for (Il2CppClass* k = c; k && il2cpp::class_get_parent; k = il2cpp::class_get_parent(k)) {
        void* it = nullptr;
        while (const Il2CppMethod* m = il2cpp::class_get_methods(k, &it)) {
            const char* nm = il2cpp::method_get_name(m);
            if (nm && strcmp(nm, name) == 0) return m;
        }
        if (k == c) { if (il2cpp::class_get_method_from_name) { const Il2CppMethod* m = il2cpp::class_get_method_from_name(c, name, 0); if (m) return m; } }
    }
    return nullptr;
}

static bool str_contains(uint64_t s, const char* needle) {
    if (!ok(s)) return false;
    int len = rd32(s + OFF_UNITY_STRING_LENGTH);
    if (len <= 0 || len > 200) return false;
    char buf[220];
    int n = len < 219 ? len : 219;
    for (int i = 0; i < n; i++) {
        uint16_t c = rd16(s + OFF_UNITY_STRING_CHARS + 2 * (uint64_t)i);
        buf[i] = (c < 0x80) ? (char)c : '?';
    }
    buf[n] = 0;
    return strstr(buf, needle) != nullptr;
}

static void read_str(uint64_t s, char* out, size_t cap) {
    out[0] = '\0';
    if (!ok(s)) return;
    int len = rd32(s + OFF_UNITY_STRING_LENGTH);
    if (len <= 0 || len > 64) return;
    size_t o = 0;
    for (int i = 0; i < len && o + 3 < cap; i++) {
        uint16_t c = rd16(s + OFF_UNITY_STRING_CHARS + 2 * (uint64_t)i);
        if (c < 0x80) out[o++] = (char)c;
        else if (c < 0x800) {
            out[o++] = (char)(0xC0 | (c >> 6));
            out[o++] = (char)(0x80 | (c & 0x3F));
        } else if (c >= 0xD800 && c <= 0xDBFF && i + 1 < len) {
            uint16_t c2 = rd16(s + OFF_UNITY_STRING_CHARS + 2 * (uint64_t)(i + 1));
            if (c2 >= 0xDC00 && c2 <= 0xDFFF) {
                i++;
                uint32_t cp = 0x10000 + ((c - 0xD800) << 10) + (c2 - 0xDC00);
                out[o++] = (char)(0xF0 | (cp >> 18));
                out[o++] = (char)(0x80 | ((cp >> 12) & 0x3F));
                out[o++] = (char)(0x80 | ((cp >> 6) & 0x3F));
                out[o++] = (char)(0x80 | (cp & 0x3F));
            } else out[o++] = '?';
        } else {
            out[o++] = (char)(0xE0 | (c >> 12));
            out[o++] = (char)(0x80 | ((c >> 6) & 0x3F));
            out[o++] = (char)(0x80 | (c & 0x3F));
        }
    }
    out[o] = '\0';
}

static inline void* mp(const void* m){return m?*(void**)((uintptr_t)m+il2cpp::offset::il2cpp_method_pointer):nullptr;}

static inline bool mi_make_rw(uintptr_t slot) {
    long pg = sysconf(_SC_PAGESIZE);
    uintptr_t page = slot & ~(uintptr_t)(pg - 1);
    return mprotect((void*)page, (size_t)pg * 2, PROT_READ | PROT_WRITE) == 0;
}

static inline void flush(void* a, size_t n) {
    __builtin___clear_cache((char*)a, (char*)a + n);
}

static inline uint32_t a64_ldr_lit(int rt, int bytes_from_pc) {
    uint32_t imm19 = (uint32_t)(bytes_from_pc >> 2) & 0x7FFFF;
    return 0x58000000u | (imm19 << 5) | (uint32_t)(rt & 31);
}

static void build_stub(uint32_t* w, uint64_t target) {
    w[0] = a64_ldr_lit(17, 8);
    w[1] = 0xD61F0220u;
    *(uint64_t*)(w + 2) = target;
}

static void* hook_tramp(void* target, size_t n) {
    void* m = mmap(nullptr, n + 16, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (m == MAP_FAILED) return nullptr;
    uint32_t* t = (uint32_t*)m;
    memcpy(t, target, n);
    t[n / 4] = a64_ldr_lit(17, 8);
    t[n / 4 + 1] = 0xD61F0220u;
    *(uint64_t*)(t + n / 4 + 2) = (uint64_t)target + n;
    flush(m, n + 16);
    if (mprotect(m, n + 16, PROT_READ | PROT_EXEC) != 0) {
        munmap(m, n + 16);
        return nullptr;
    }
    return m;
}

static void inline_hook(void* target, void* hook, void** orig) {
    long pg = sysconf(_SC_PAGESIZE);
    uintptr_t page = (uintptr_t)target & ~(uintptr_t)(pg - 1);
    *orig = hook_tramp(target, 16);
    if (!*orig) return;
    if (mprotect((void*)page, (size_t)pg * 2, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) return;
    build_stub((uint32_t*)target, (uint64_t)hook);
    flush(target, 16);
    mprotect((void*)page, (size_t)pg * 2, PROT_READ | PROT_EXEC);
}
