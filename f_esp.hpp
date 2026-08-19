#pragma once

#include "../core.hpp"
#include "../events.hpp"
#include "../ui/watermark.hpp"
#include "../sdk/include.h"

namespace f_esp {

static ImU32 cv(ImVec4 c) {
    return IM_COL32((int)(c.x * 255.f), (int)(c.y * 255.f), (int)(c.z * 255.f), (int)(c.w * 255.f));
}

static void skb(ImDrawList* dl, const float vm[16], const Vector3* b, const bool* v,
                const int* id, int cnt, ImU32 c, float thick) {
    ImVec2 p[8];
    int n = 0;
    for (int i = 0; i < cnt && n < 8; i++) {
        int j = id[i];
        if (!v[j]) return;
        float sx, sy;
        if (!w2s(b[j], vm, sx, sy)) return;
        p[n++] = ImVec2(sx, sy);
    }
    if (n >= 2)
        dl->AddPolyline(p, n, c, ImDrawFlags_None, thick);
}

static void sk(uint64_t pl, const float vm[16], float thick, ImU32 c) {
    SK s;
    if (!gsb(pl, s) || !s.ok) return;
    Vector3 pp = player_pos(pl);
    sch(s, pp.x, pp.y, pp.z);
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    static const int sp[] = { BONE_NECK, BONE_SPINE, BONE_HIP };
    static const int la[] = { BONE_NECK, BONE_LEFT_UPPERARM, BONE_LEFT_FOREARM, BONE_LEFT_HAND };
    static const int ra[] = { BONE_NECK, BONE_RIGHT_UPPERARM, BONE_RIGHT_FOREARM, BONE_RIGHT_HAND };
    static const int ll[] = { BONE_HIP, BONE_LEFT_LEG, BONE_LEFT_FOOT };
    static const int rl[] = { BONE_HIP, BONE_RIGHT_LEG, BONE_RIGHT_FOOT };
    skb(dl, vm, s.b, s.v, sp, 3, c, thick);
    skb(dl, vm, s.b, s.v, la, 4, c, thick);
    skb(dl, vm, s.b, s.v, ra, 4, c, thick);
    skb(dl, vm, s.b, s.v, ll, 3, c, thick);
    skb(dl, vm, s.b, s.v, rl, 3, c, thick);
}

static int draw_esp() {
    if (!scr_w || !scr_h) return 0;
    if (!opt_esp) return 0;
    if (!opt_box && !opt_health && !opt_dist && !opt_skeleton && !opt_name && !opt_weapon) return 0;
    uint64_t pm = player_manager();
    if (!ok(pm)) return 0;
    uint64_t lp = rd64(pm + OFF_PM_LOCAL_PLAYER);
    if (!ok(lp)) return 0;
    float vm[16];
    if (!view_matrix(vm)) return 0;
    Vector3 lpos = player_pos(lp);
    int lteam = rd8(lp + OFF_PLAYER_TEAM);
    uint64_t players[96];
    int np = collect_players(pm, lp, players, 96);
    if (!np) return 0;
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    int drawn = 0;
    for (int i = 0; i < np; i++) {
        uint64_t e = players[i];
        if (rd8(e + OFF_PLAYER_TEAM) == lteam) continue;
        Vector3 pp = player_pos(e);
        if (!(pp.x > -20000.f && pp.x < 20000.f && pp.y > -20000.f && pp.y < 20000.f && pp.z > -20000.f && pp.z < 20000.f)) continue;
        float hp = (float)health_of(e);
        if (hp <= 0.f) continue;
        float ddx = pp.x - lpos.x, ddy = pp.y - lpos.y, ddz = pp.z - lpos.z;
        float dist = sqrtf(ddx * ddx + ddy * ddy + ddz * ddz);
        if (dist > s_esp_dist) continue;
        Vector3 head(pp.x, pp.y + PLAYER_HEIGHT, pp.z);
        float hx, hy, bx, by;
        if (!w2s(head, vm, hx, hy)) continue;
        if (!w2s(pp, vm, bx, by)) continue;
        float y1 = fminf(hy, by), y2 = fmaxf(hy, by);
        float h = y2 - y1;
        if (h < 2.f) continue;
        float bw = h * 0.25f;
        float cx = (hx + bx) * 0.5f;
        if (opt_box) {
            ImU32 bc = cv(s_box_col);
            if (s_box_filled)
                dl->AddRectFilled(ImVec2(cx - bw, y1), ImVec2(cx + bw, y2), IM_COL32(0, 0, 0, 60));
            if (s_box_shape == 0) {
                dl->AddRect(ImVec2(cx - bw, y1), ImVec2(cx + bw, y2), bc, 0.f, 0, s_box_thick);
            } else {
                float cw = bw * 0.3f, cl = h * 0.12f;
                dl->AddLine(ImVec2(cx - bw, y1), ImVec2(cx - bw + cw, y1), bc, s_box_thick);
                dl->AddLine(ImVec2(cx - bw, y1), ImVec2(cx - bw, y1 + cl), bc, s_box_thick);
                dl->AddLine(ImVec2(cx + bw, y1), ImVec2(cx + bw - cw, y1), bc, s_box_thick);
                dl->AddLine(ImVec2(cx + bw, y1), ImVec2(cx + bw, y1 + cl), bc, s_box_thick);
                dl->AddLine(ImVec2(cx - bw, y2), ImVec2(cx - bw + cw, y2), bc, s_box_thick);
                dl->AddLine(ImVec2(cx - bw, y2), ImVec2(cx - bw, y2 - cl), bc, s_box_thick);
                dl->AddLine(ImVec2(cx + bw, y2), ImVec2(cx + bw - cw, y2), bc, s_box_thick);
                dl->AddLine(ImVec2(cx + bw, y2), ImVec2(cx + bw, y2 - cl), bc, s_box_thick);
            }
        }
        if (s_esp_style == 1) continue;
        if (!watermark::espFont) continue;
        ImFont* ef = watermark::espFont;
        float fs = 14.f;
        if (opt_name) {
            char nm[128];
            player_nick(e, nm, sizeof(nm));
            if (nm[0]) {
                ImVec2 ts = ef->CalcTextSizeA(fs, FLT_MAX, 0.f, nm);
                ImU32 nc = IM_COL32((int)(s_name_col.x * 255), (int)(s_name_col.y * 255), (int)(s_name_col.z * 255), (int)(s_name_col.w * 255));
                dl->AddText(ef, fs, ImVec2(cx - ts.x * 0.5f, y1 - ts.y - 2.f), nc, nm);
            }
        }
        if (opt_health) {
            int hpi = (int)hp;
            if (hpi < 0) hpi = 0;
            if (hpi > 100) hpi = 100;
            float bw2 = s_health_size;
            float pct = hpi / 100.f;
            ImU32 g1 = cv(s_health_col1);
            ImU32 g2 = cv(s_health_col2);
            if (s_health_pos == 0) {
                float bhx = cx - bw - bw2 - 2.f;
                float fh = h * pct;
                dl->AddRectFilled(ImVec2(bhx - 1.f, y1 - 1.f), ImVec2(bhx + bw2 + 1.f, y2 + 1.f), IM_COL32(0, 0, 0, 200));
                dl->AddRectFilledMultiColor(ImVec2(bhx, y2 - fh), ImVec2(bhx + bw2, y2), g1, g1, g2, g2);
            } else if (s_health_pos == 1) {
                float bhx = cx + bw + 2.f;
                float fh = h * pct;
                dl->AddRectFilled(ImVec2(bhx - 1.f, y1 - 1.f), ImVec2(bhx + bw2 + 1.f, y2 + 1.f), IM_COL32(0, 0, 0, 200));
                dl->AddRectFilledMultiColor(ImVec2(bhx, y2 - fh), ImVec2(bhx + bw2, y2), g1, g1, g2, g2);
            } else {
                float topy = y1 - bw2 - 4.f;
                dl->AddRectFilled(ImVec2(cx - bw - 1.f, topy - 1.f), ImVec2(cx + bw + 1.f, topy + bw2 + 1.f), IM_COL32(0, 0, 0, 200));
                dl->AddRectFilledMultiColor(ImVec2(cx - bw, topy), ImVec2(cx - bw + (2.f * bw) * pct, topy + bw2), g1, g2, g2, g1);
            }
            if (s_health_text) {
                char hb[16];
                snprintf(hb, sizeof(hb), "%d", hpi);
                ImVec2 ts = ef->CalcTextSizeA(fs, FLT_MAX, 0.f, hb);
                float htx, hty = y1 + (h - ts.y) * 0.5f;
                if (s_health_pos == 0) {
                    float bhx = cx - bw - bw2 - 2.f;
                    htx = bhx - ts.x - 4.f;
                } else if (s_health_pos == 1) {
                    float bhx = cx + bw + 2.f;
                    htx = bhx + bw2 + 4.f;
                } else {
                    htx = cx + bw + 4.f;
                    hty = y1;
                }
                dl->AddText(ef, fs, ImVec2(htx, hty), IM_COL32(255, 255, 255, 255), hb);
            }
        }
        float bot_y = y2 + 4.f;
        if (opt_weapon) {
            char wpn[128];
            weapon_name(e, wpn, sizeof(wpn));
            if (wpn[0]) {
                ImVec2 ts = ef->CalcTextSizeA(fs, FLT_MAX, 0.f, wpn);
                dl->AddText(ef, fs, ImVec2(cx - ts.x * 0.5f, bot_y), cv(s_weapon_col), wpn);
                bot_y += ts.y + 2.f;
            }
        }
        if (opt_dist) {
            char txt[24];
            snprintf(txt, sizeof(txt), "%.0fm", dist);
            ImVec2 ts = ef->CalcTextSizeA(fs, FLT_MAX, 0.f, txt);
            dl->AddText(ef, fs, ImVec2(cx - ts.x * 0.5f, bot_y), cv(s_dist_col), txt);
        }
        if (opt_skeleton) {
            ImU32 c = IM_COL32((int)(s_skel_color.x * 255), (int)(s_skel_color.y * 255), (int)(s_skel_color.z * 255), (int)(s_skel_color.w * 255));
            sk(e, vm, s_skel_thick, c);
        }
        drawn++;
    }
    return drawn;
}

static void render() {
    draw_esp();
}

}

namespace {
static events::feature _f_esp = {"esp", nullptr, nullptr, nullptr, f_esp::render, nullptr};
static struct _reg_esp { _reg_esp() { events::register_feature(_f_esp); } } _r_esp;
}
