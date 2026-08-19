#pragma once

#include "imgui.h"
#include "../src/config.hpp"
#include "watermark.hpp"
#include "lang/lang.h"
#include "lang/ru.h"
#include "lang/en.h"

namespace menu {

    inline bool visible = false;
    static int tab = 0;
    static int sub = 0;

    static void set_tab(int t) { tab = t; sub = 0; }

    static int sub_count() {
        if (tab == 0) return 2;
        if (tab == 1) return 1;
        if (tab == 2) return 1;
        if (tab == 4) return 1;
        return 0;
    }

    static const char* sub_icon(int s) {
        if (tab == 0) return s == 0 ? "\uf06e" : "\uf0ac";
        if (tab == 1) return "\uf0e7";
        if (tab == 2) return "\uf072";
        return "\uf013";
    }

    static const char* sub_name(int s) {
        if (tab == 0) return s == 0 ? g_lang->esp : g_lang->world;
        if (tab == 1) return g_lang->play;
        if (tab == 2) return g_lang->misc;
        return g_lang->general;
    }

    enum set_kind { SET_BOOL, SET_SLIDER, SET_TYPE, SET_COLOR, SET_COLOR2 };

    struct setting {
        const char* name;
        set_kind kind;
        bool* b;
        float* f;
        int* i;
        int opts;
        const char** opt_names;
        float lo, hi, step;
        ImVec4* c;
        ImVec4* c2 = nullptr;
    };

    struct cell {
        const char* name;
        const char* desc;
        bool* val;
        setting* sets;
        int nset;
    };

    static setting s_sets_esp[2];
    static setting s_sets_box[4];
    static setting s_sets_health[4];
    static setting s_sets_skel[2];
    static setting s_sets_name[1];
    static setting s_sets_dist[1];
    static setting s_sets_weapon[1];
    static setting s_sets_chams[3];
    static setting s_sets_hands[3];
    static setting s_sets_weapon_chams[3];
    static setting s_sets_fog[3];
    static setting s_sets_sky[1];
    static setting s_sets_model[3];
    static setting s_sets_lang[1];
    static setting s_sets_aa[5];
    static setting s_sets_aim[5];
    static setting s_sets_airstrafe[1];

    static void fill_sets() {
        static const char* st_style[2];
        static const char* st_shape[2];
        static const char* st_pos[3];
        static const char* st_chams[2];
        static const char* st_lang[2];
        static const char* st_pitch[3];
        static const char* st_yaw[4];
        static const char* st_bone[4];
        st_style[0] = g_lang->full; st_style[1] = g_lang->minimal;
        st_shape[0] = g_lang->box_shape; st_shape[1] = g_lang->corners;
        st_pos[0] = g_lang->left; st_pos[1] = g_lang->right; st_pos[2] = g_lang->top;
        st_chams[0] = g_lang->solid; st_chams[1] = g_lang->flat;
        st_lang[0] = g_lang->russian; st_lang[1] = g_lang->english;
        st_pitch[0] = g_lang->aa_off; st_pitch[1] = g_lang->aa_up; st_pitch[2] = g_lang->aa_down;
        st_yaw[0] = g_lang->aa_forward; st_yaw[1] = g_lang->aa_backward; st_yaw[2] = g_lang->aa_spin; st_yaw[3] = g_lang->aa_chaos;
        st_bone[0] = g_lang->bone_head; st_bone[1] = g_lang->bone_neck; st_bone[2] = g_lang->bone_chest; st_bone[3] = g_lang->bone_hips;

        s_sets_esp[0] = { g_lang->dist_render, SET_SLIDER, NULL, &s_esp_dist, NULL, 0, NULL, 50.f, 500.f, 10.f, NULL };
        s_sets_esp[1] = { g_lang->style, SET_TYPE, NULL, NULL, &s_esp_style, 2, st_style, 0, 0, 0, NULL };

        s_sets_box[0] = { g_lang->thickness, SET_SLIDER, NULL, &s_box_thick, NULL, 0, NULL, 1.f, 5.f, 1.f, NULL };
        s_sets_box[1] = { g_lang->shape, SET_TYPE, NULL, NULL, &s_box_shape, 2, st_shape, 0, 0, 0, NULL };
        s_sets_box[2] = { g_lang->filled, SET_BOOL, &s_box_filled, NULL, NULL, 0, NULL, 0, 0, 0, NULL };
        s_sets_box[3] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_box_col };

        s_sets_health[0] = { g_lang->size, SET_SLIDER, NULL, &s_health_size, NULL, 0, NULL, 1.f, 8.f, 1.f, NULL };
        s_sets_health[1] = { g_lang->position, SET_TYPE, NULL, NULL, &s_health_pos, 3, st_pos, 0, 0, 0, NULL };
        s_sets_health[2] = { g_lang->color, SET_COLOR2, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_health_col1, &s_health_col2 };
        s_sets_health[3] = { g_lang->text, SET_BOOL, &s_health_text, NULL, NULL, 0, NULL, 0, 0, 0, NULL };

        s_sets_skel[0] = { g_lang->thickness, SET_SLIDER, NULL, &s_skel_thick, NULL, 0, NULL, 1.f, 5.f, 1.f, NULL };
        s_sets_skel[1] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_skel_color };

        s_sets_name[0] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_name_col };

        s_sets_dist[0] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_dist_col };

        s_sets_weapon[0] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_weapon_col };

        s_sets_chams[0] = { g_lang->type, SET_TYPE, NULL, NULL, &s_chams_type, 2, st_chams, 0, 0, 0, NULL };
        s_sets_chams[1] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_chams_col };
        s_sets_chams[2] = { g_lang->self, SET_BOOL, &opt_chams_self, NULL, NULL, 0, NULL, 0, 0, 0, NULL };

        s_sets_hands[0] = { g_lang->shader, SET_BOOL, &s_hands_shader, NULL, NULL, 0, NULL, 0, 0, 0, NULL };
        s_sets_hands[1] = { g_lang->type, SET_TYPE, NULL, NULL, &s_hands_type, 2, st_chams, 0, 0, 0, NULL };
        s_sets_hands[2] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_hands_col };

        s_sets_weapon_chams[0] = { g_lang->shader, SET_BOOL, &s_weapon_chams_shader, NULL, NULL, 0, NULL, 0, 0, 0, NULL };
        s_sets_weapon_chams[1] = { g_lang->type, SET_TYPE, NULL, NULL, &s_weapon_chams_type, 2, st_chams, 0, 0, 0, NULL };
        s_sets_weapon_chams[2] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_weapon_chams_col };

        s_sets_fog[0] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_fog_col };
        s_sets_fog[1] = { g_lang->fog_start, SET_SLIDER, NULL, &s_fog_start, NULL, 0, NULL, 0.f, 100.f, 1.f, NULL };
        s_sets_fog[2] = { g_lang->fog_end, SET_SLIDER, NULL, &s_fog_end, NULL, 0, NULL, 10.f, 500.f, 5.f, NULL };

        s_sets_sky[0] = { g_lang->color, SET_COLOR, NULL, NULL, NULL, 0, NULL, 0, 0, 0, &s_sky_col };

        s_sets_lang[0] = { g_lang->language, SET_TYPE, NULL, NULL, &s_lang, 2, st_lang, 0, 0, 0, NULL };

        s_sets_aa[0] = { g_lang->aa_pitch, SET_TYPE, NULL, NULL, &s_aa_pitch, 3, st_pitch, 0, 0, 0, NULL };
        s_sets_aa[1] = { g_lang->aa_yaw, SET_TYPE, NULL, NULL, &s_aa_yaw, 4, st_yaw, 0, 0, 0, NULL };
        s_sets_aa[2] = { g_lang->aa_speed, SET_SLIDER, NULL, &s_aa_speed, NULL, 0, NULL, 0.f, 180.f, 1.f, NULL };
        s_sets_aa[3] = { g_lang->aa_jitter, SET_BOOL, &s_aa_jitter, NULL, NULL, 0, NULL, 0, 0, 0, NULL };
        s_sets_aa[4] = { g_lang->aa_range, SET_SLIDER, NULL, &s_aa_range, NULL, 0, NULL, 0.f, 180.f, 5.f, NULL };

        s_sets_aim[0] = { g_lang->aim_fov, SET_SLIDER, NULL, &s_aim_fov, NULL, 0, NULL, 5.f, 90.f, 5.f, NULL };
        s_sets_aim[1] = { g_lang->aim_bone, SET_TYPE, NULL, NULL, &s_aim_bone, 4, st_bone, 0, 0, 0, NULL };
        s_sets_aim[2] = { g_lang->aim_smooth, SET_SLIDER, NULL, &s_aim_smooth, NULL, 0, NULL, 0.f, 1.f, 0.05f, NULL };
        s_sets_aim[3] = { g_lang->aim_visible, SET_BOOL, &opt_aim_visible, NULL, NULL, 0, NULL, 0, 0, 0, NULL };
        s_sets_aim[4] = { g_lang->aim_fov_draw, SET_BOOL, &opt_aim_fov_draw, NULL, NULL, 0, NULL, 0, 0, 0, NULL };

        s_sets_model[0] = { g_lang->model_dist, SET_SLIDER, NULL, &s_model_dist, NULL, 0, NULL, 0.f, 10.f, 0.1f, NULL };
        s_sets_model[1] = { g_lang->model_height, SET_SLIDER, NULL, &s_model_height, NULL, 0, NULL, -2.f, 5.f, 0.1f, NULL };
        s_sets_model[2] = { g_lang->model_rot, SET_SLIDER, NULL, &s_model_rot, NULL, 0, NULL, 0.f, 360.f, 1.f, NULL };

        s_sets_airstrafe[0] = { g_lang->air_strafe_strength, SET_SLIDER, NULL, &s_air_strafe, NULL, 0, NULL, 1.f, 20.f, 0.5f, NULL };
    }

    static int build_cells(cell* c) {
        fill_sets();
        int n = 0;
        if (tab == 0 && sub == 0) {
            c[n++] = { g_lang->enable_esp, g_lang->enable_esp_desc, &opt_esp, s_sets_esp, 2 };
            c[n++] = { g_lang->box, g_lang->box_desc, &opt_box, s_sets_box, 4 };
            c[n++] = { g_lang->health, g_lang->health_desc, &opt_health, s_sets_health, 4 };
            c[n++] = { g_lang->distance, g_lang->distance_desc, &opt_dist, s_sets_dist, 1 };
            c[n++] = { g_lang->skeleton, g_lang->skeleton_desc, &opt_skeleton, s_sets_skel, 2 };
            c[n++] = { g_lang->nick, g_lang->nick_desc, &opt_name, s_sets_name, 1 };
            c[n++] = { g_lang->weapon, g_lang->weapon_desc, &opt_weapon, s_sets_weapon, 1 };
        }
        else if (tab == 0 && sub == 1) {
            c[n++] = { g_lang->chams, g_lang->chams_desc, &opt_chams, s_sets_chams, 3 };
            c[n++] = { g_lang->hands, g_lang->hands_desc, &opt_hands, s_sets_hands, 3 };
            c[n++] = { g_lang->weapon_chams, g_lang->weapon_chams_desc, &opt_weapon_chams, s_sets_weapon_chams, 3 };
            c[n++] = { g_lang->fog, g_lang->fog_desc, &opt_fog, s_sets_fog, 3 };
            c[n++] = { g_lang->sky, g_lang->sky_desc, &opt_sky, s_sets_sky, 1 };
            c[n++] = { g_lang->third_person, g_lang->third_person_desc, &opt_tps, NULL, 0 };
            c[n++] = { g_lang->model, g_lang->model_desc, &opt_model, s_sets_model, 3 };
        }
        else if (tab == 1 && sub == 0) {
            c[n++] = { g_lang->aimbot, g_lang->aimbot_desc, &opt_aim, s_sets_aim, 5 };
        }
        else if (tab == 2 && sub == 0) {
            c[n++] = { g_lang->antiaim, g_lang->antiaim_desc, &opt_aa, s_sets_aa, 5 };
            c[n++] = { g_lang->air_strafe, g_lang->air_strafe_desc, &opt_air_strafe, s_sets_airstrafe, 1 };
            c[n++] = { g_lang->inf_ammo, g_lang->inf_ammo_desc, &opt_inf_ammo, NULL, 0 };
            c[n++] = { g_lang->rapid_fire, g_lang->rapid_fire_desc, &opt_rapid_fire, NULL, 0 };
            c[n++] = { g_lang->wallshot, g_lang->wallshot_desc, &opt_wallshot, NULL, 0 };
        }
        else if (tab == 4 && sub == 0) {
            c[n++] = { g_lang->language, g_lang->language_desc, NULL, s_sets_lang, 1 };
        }
        return n;
    }

    static bool inside(float x, float y, float x0, float y0, float x1, float y1) {
        return x >= x0 && x <= x1 && y >= y0 && y <= y1;
    }

    static void tog(ImDrawList* dl, float x, float y, float w, float h, float a) {
        float r = h * 0.5f;
        int or_ = (int)(90 + (255 - 90) * a);
        int og = (int)(90 + (222 - 90) * a);
        int ob = (int)(95 + (176 - 95) * a);
        dl->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), IM_COL32(or_, og, ob, 255), r);
        float kx = x + r + (w - r * 2.f) * a;
        dl->AddCircleFilled(ImVec2(kx, y + r), r - 2.f, IM_COL32(255, 255, 255, 255));
    }

    static void rgb2hsv(float r, float g, float b, float& h, float& s, float& v) {
        float mx = r > g ? r : g;
        mx = mx > b ? mx : b;
        float mn = r < g ? r : g;
        mn = mn < b ? mn : b;
        float d = mx - mn;
        v = mx;
        if (d < 0.0001f) { h = 0.f; s = 0.f; return; }
        s = d / mx;
        if (mx == r) h = fmodf((g - b) / d, 6.f);
        else if (mx == g) h = (b - r) / d + 2.f;
        else h = (r - g) / d + 4.f;
        h /= 6.f;
        if (h < 0.f) h += 1.f;
    }

    static void hsv2rgb(float h, float s, float v, float& r, float& g, float& b) {
        h = h - floorf(h);
        int i = (int)(h * 6.f);
        float f = h * 6.f - i;
        float p = v * (1.f - s);
        float q = v * (1.f - s * f);
        float t = v * (1.f - s * (1.f - f));
        switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
        }
    }

    static bool cpop = false;
    static ImVec4* cpop_t = nullptr;
    static float cpop_sv_x = 1.f, cpop_sv_y = 0.f;
    static float cpop_hue = 0.f;
    static int cpop_drag = 0;
    static int cpop_open_frame = -1;

    static void cpop_open(ImVec4* c) {
        cpop_t = c;
        cpop = true;
        cpop_drag = 0;
        cpop_open_frame = ImGui::GetFrameCount();
        float h, s, v;
        rgb2hsv(c->x, c->y, c->z, h, s, v);
        cpop_hue = h;
        cpop_sv_x = s;
        cpop_sv_y = 1.f - v;
    }

    static float clamp01(float x) { return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); }

    static void cpop_update() {
        if (!cpop_t) return;
        float r, g, b;
        hsv2rgb(cpop_hue, cpop_sv_x, 1.f - cpop_sv_y, r, g, b);
        cpop_t->x = r;
        cpop_t->y = g;
        cpop_t->z = b;
    }

    static void cpop_draw(ImDrawList* dl, float sc, float mx, float my, float wx, float wy, float ww, float wh, bool press, bool tap, bool down) {
        if (!cpop || !cpop_t) return;
        float pw = 280.f * sc;
        float pad = 12.f * sc;
        float huew = 14.f * sc;
        float svsz = pw - pad * 2.f - huew - 8.f * sc;
        float ph = 40.f * sc + svsz + 8.f * sc + 12.f * sc + 14.f * sc;
        float px = wx + ww * 0.5f - pw * 0.5f;
        float py = wy + wh * 0.5f - ph * 0.5f;
        float svx = px + pad;
        float svy = py + 38.f * sc;
        float huex = svx + svsz + 6.f * sc;
        float hueh = svsz;
        float abx = svx;
        float aby = svy + svsz + 8.f * sc;
        float abh = 12.f * sc;

        dl->AddRectFilled(ImVec2(px, py), ImVec2(px + pw, py + ph), IM_COL32(24, 24, 27, 245), 10.f * sc);
        dl->AddRect(ImVec2(px, py), ImVec2(px + pw, py + ph), IM_COL32(45, 45, 50, 255), 10.f * sc, 0, 1.f);

        if (watermark::espFont) {
            dl->AddText(watermark::espFont, 14.f * sc, ImVec2(px + pad, py + (38.f * sc - 14.f * sc) * 0.5f), IM_COL32(255, 255, 255, 255), g_lang->color);
            float xb = 20.f * sc;
            float bx = px + pw - pad - xb;
            float by = py + (38.f * sc - xb) * 0.5f;
            bool xh = inside(mx, my, bx, by, bx + xb, by + xb);
            if (xh)
                dl->AddRectFilled(ImVec2(bx, by), ImVec2(bx + xb, by + xb), IM_COL32(255, 255, 255, 22), 4.f * sc);
            ImVec2 xz = watermark::espFont->CalcTextSizeA(12.f * sc, FLT_MAX, 0.f, "X");
            dl->AddText(watermark::espFont, 12.f * sc, ImVec2(bx + (xb - xz.x) * 0.5f, by + (xb - xz.y) * 0.5f), IM_COL32(255, 255, 255, 255), "X");
            if ((press || tap) && xh) { cpop = false; return; }
        }

        float r, g, b;
        hsv2rgb(cpop_hue, 1.f, 1.f, r, g, b);
        ImU32 hue_c = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
        dl->AddRectFilledMultiColor(ImVec2(svx, svy), ImVec2(svx + svsz, svy + svsz),
            IM_COL32(255, 255, 255, 255), hue_c, hue_c, IM_COL32(255, 255, 255, 255));
        dl->AddRectFilledMultiColor(ImVec2(svx, svy), ImVec2(svx + svsz, svy + svsz),
            IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255));
        dl->AddRect(ImVec2(svx, svy), ImVec2(svx + svsz, svy + svsz), IM_COL32(0, 0, 0, 120), 0.f, 0, 1.f);

        float mkx = svx + cpop_sv_x * svsz;
        float mky = svy + cpop_sv_y * svsz;
        dl->AddCircle(ImVec2(mkx, mky), 5.f * sc, IM_COL32(0, 0, 0, 180), 16, 1.2f);
        dl->AddCircleFilled(ImVec2(mkx, mky), 4.f * sc, IM_COL32(255, 255, 255, 255), 16);

        ImU32 hc[7] = {
            IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255),
            IM_COL32(0, 255, 255, 255), IM_COL32(0, 0, 255, 255), IM_COL32(255, 0, 255, 255),
            IM_COL32(255, 0, 0, 255)
        };
        for (int k = 0; k < 6; k++) {
            float y0 = svy + hueh * k / 6.f;
            float y1 = svy + hueh * (k + 1) / 6.f;
            dl->AddRectFilledMultiColor(ImVec2(huex, y0), ImVec2(huex + huew, y1), hc[k], hc[k], hc[k + 1], hc[k + 1]);
        }
        dl->AddRect(ImVec2(huex, svy), ImVec2(huex + huew, svy + hueh), IM_COL32(0, 0, 0, 120), 0.f, 0, 1.f);
        float hby = svy + cpop_hue * hueh;
        dl->AddRectFilled(ImVec2(huex - 3.f * sc, hby - 1.f * sc), ImVec2(huex + huew + 3.f * sc, hby + 1.f * sc), IM_COL32(255, 255, 255, 255));

        hsv2rgb(cpop_hue, cpop_sv_x, 1.f - cpop_sv_y, r, g, b);
        ImU32 ac = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
        ImU32 at = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 0);
        dl->AddRectFilled(ImVec2(abx, aby), ImVec2(abx + svsz, aby + abh), IM_COL32(0, 0, 0, 255));
        dl->AddRectFilledMultiColor(ImVec2(abx, aby), ImVec2(abx + svsz, aby + abh), ac, at, at, ac);
        dl->AddRect(ImVec2(abx, aby), ImVec2(abx + svsz, aby + abh), IM_COL32(0, 0, 0, 120), 0.f, 0, 1.f);
        float abx2 = abx + cpop_t->w * svsz;
        dl->AddRectFilled(ImVec2(abx2 - 1.f * sc, aby - 3.f * sc), ImVec2(abx2 + 1.f * sc, aby + abh + 3.f * sc), IM_COL32(255, 255, 255, 255));

        bool over = inside(mx, my, px, py, px + pw, py + ph);
        if (press) {
            if (!over) { cpop = false; return; }
            if (inside(mx, my, svx, svy, svx + svsz, svy + svsz)) cpop_drag = 1;
            else if (inside(mx, my, huex, svy, huex + huew, svy + hueh)) cpop_drag = 2;
            else if (inside(mx, my, abx, aby, abx + svsz, aby + abh)) cpop_drag = 3;
        }
        if (tap && !over && ImGui::GetFrameCount() > cpop_open_frame) { cpop = false; return; }
        if (cpop_drag == 1 && down) {
            cpop_sv_x = clamp01((mx - svx) / svsz);
            cpop_sv_y = clamp01((my - svy) / svsz);
            cpop_update();
        }
        else if (cpop_drag == 2 && down) {
            cpop_hue = clamp01((my - svy) / hueh);
            cpop_update();
        }
        else if (cpop_drag == 3 && down) {
            cpop_t->w = clamp01((mx - abx) / svsz);
        }
        if (!down)
            cpop_drag = 0;
    }

    static float anim = 0.f;

    inline void render() {
        ImGuiIO& io = ImGui::GetIO();
        float sw = io.DisplaySize.x;
        float sh = io.DisplaySize.y;
        float sc = sw / 1920.f;
        float dt = io.DeltaTime;

        if (visible) {
            float k = 12.f * dt;
            if (k > 1.f) k = 1.f;
            anim += (1.f - anim) * k;
            if (anim > 0.99f) anim = 1.f;
        }
        else {
            float k = 12.f * dt;
            if (k > 1.f) k = 1.f;
            anim -= anim * k;
            if (anim < 0.01f) anim = 0.f;
        }
        if (anim < 0.01f) {
            cpop = false;
            cpop_t = nullptr;
            return;
        }
        if (!visible) {
            cpop = false;
            cpop_t = nullptr;
        }

        float mw0 = 560.f * sc;
        float mh0 = 520.f * sc;
        float scale = anim;
        float mw = mw0 * scale;
        float mh = mh0 * scale;
        float cy0 = (sh - mh0) * 0.5f;
        float offy = sh + mh0 * 0.5f;
        float mx = (sw - mw) * 0.5f;
        float my = cy0 + (offy - cy0) * (1.f - scale);
        float round = 24.f * sc * scale;

        static bool prev = false;
        bool down = io.MouseDown[0] && visible;
        bool press = down && !prev;
        bool release = !down && prev;
        prev = down;

        static float scroll = 0.f;
        static bool touch_drag = false;
        static float down_x = 0.f, down_y = 0.f;
        static float last_my = 0.f;
        if (press) { down_x = io.MousePos.x; down_y = io.MousePos.y; last_my = io.MousePos.y; touch_drag = false; }
        if (down && !cpop) {
            float dy = io.MousePos.y - last_my;
            last_my = io.MousePos.y;
            if (touch_drag) {
                scroll -= dy;
            }
            else {
                float dx = io.MousePos.x - down_x;
                float ddy = io.MousePos.y - down_y;
                float ady = ddy < 0 ? -ddy : ddy;
                float adx = dx < 0 ? -dx : dx;
                if (ady > 12.f * sc && ady > adx)
                    touch_drag = true;
            }
        }

        static bool was = false;
        static bool suppress_tap = false;
        static int guard = 0;
        if (press) suppress_tap = false;
        if (visible && !was) { guard = 3; suppress_tap = true; }
        was = visible;
        if (guard > 0) guard--;
        bool tap = release && !touch_drag && !suppress_tap && guard == 0 && visible;
        bool mtap = tap && !cpop;

        ImDrawList* dl = ImGui::GetForegroundDrawList();
        dl->AddRectFilled(ImVec2(mx, my), ImVec2(mx + mw, my + mh), IM_COL32(18, 18, 22, 255), round);

        float hdr_h = 48.f * sc;
        float logo_sz = 28.f * sc;
        float ts = 16.f * sc;
        float ty = my + (hdr_h - ts) * 0.5f;
        float lx = mx + 12.f * sc;
        float ly = ty + (ts - logo_sz) * 0.5f;

        if (watermark::logoTex) {
            dl->AddImage(watermark::logoTex, ImVec2(lx, ly), ImVec2(lx + logo_sz, ly + logo_sz), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
        }

        if (watermark::espFont) {
            // ============================================================
            // ЗДЕСЬ ИЗМЕНЕНО НА @sintsex_cheats
            // ============================================================
            dl->AddText(watermark::espFont, ts, ImVec2(lx + logo_sz + 8.f * sc, ty), IM_COL32(255, 255, 255, 255), "@sintsex_cheats");
        }

        float sb_w = 128.f * sc;
        float sby = my + hdr_h + 10.f * sc;
        float tb_h = 28.f * sc;
        float tgap = 8.f * sc;
        const char* tabs[5] = { g_lang->visuals, g_lang->combat, g_lang->rage, g_lang->configs, g_lang->settings };
        for (int i = 0; i < 5; i++) {
            float ty = sby + i * (tb_h + tgap);
            if (tab == i) {
                dl->AddRectFilled(ImVec2(mx + 6.f * sc, ty), ImVec2(mx + sb_w - 6.f * sc, ty + tb_h), IM_COL32(255, 255, 255, 12), 8.f * sc);
            }
            if (watermark::espFont) {
                float ts = 15.f * sc;
                ImVec2 tz = watermark::espFont->CalcTextSizeA(ts, FLT_MAX, 0.f, tabs[i]);
                ImU32 col = tab == i ? IM_COL32(255, 222, 176, 255) : IM_COL32(205, 205, 205, 255);
                dl->AddText(watermark::espFont, ts, ImVec2(mx + 14.f * sc, ty + (tb_h - tz.y) * 0.5f), col, tabs[i]);
            }
            if (mtap && inside(io.MousePos.x, io.MousePos.y, mx + 6.f * sc, ty, mx + sb_w - 6.f * sc, ty + tb_h))
                set_tab(i);
        }

        dl->AddLine(ImVec2(mx + sb_w, my + hdr_h), ImVec2(mx + sb_w, my + mh), IM_COL32(70, 70, 75, 255), 1.f);

        float cpad = 14.f * sc;
        float cx = mx + sb_w + cpad;
        float cw = mx + mw - cpad - cx;
        float ctop = my + hdr_h + 8.f * sc;

        dl->PushClipRect(ImVec2(mx + sb_w, my + hdr_h), ImVec2(mx + mw, my + mh), true);

        int scount = sub_count();
        float isz = 24.f * sc;
        float igap = 8.f * sc;
        float ix = cx;
        float iy = ctop - scroll;
        float push = 14.f * sc;

        static float spread[4] = { 0.f, 0.f, 0.f, 0.f };
        for (int s = 0; s < 4; s++) {
            float target = (s < scount && s == sub) ? 1.f : 0.f;
            spread[s] += (target - spread[s]) * 0.16f;
        }

        float cur_x = ix;
        for (int s = 0; s < scount; s++) {
            float ax = cur_x;
            float aw = isz;
            ImVec2 lw = watermark::espFont ? watermark::espFont->CalcTextSizeA(15.f * sc, FLT_MAX, 0.f, sub_name(s)) : ImVec2(0, 15.f * sc);
            float pill_w = aw + lw.x + 18.f * sc;
            if (sub == s) {
                dl->AddRectFilled(ImVec2(ax - 2.f * sc, iy - 2.f * sc), ImVec2(ax + pill_w, iy + isz + 2.f * sc), IM_COL32(255, 255, 255, 12), 8.f * sc);
            }
            if (watermark::iconFont) {
                ImVec2 ts = watermark::iconFont->CalcTextSizeA(isz * 0.8f, FLT_MAX, 0.f, sub_icon(s));
                ImU32 col = sub == s ? IM_COL32(255, 222, 176, 255) : IM_COL32(150, 150, 155, 255);
                dl->AddText(watermark::iconFont, isz * 0.8f, ImVec2(ax + (isz - ts.x) * 0.5f, iy + (isz - ts.y) * 0.5f), col, sub_icon(s));
            }
            if (watermark::espFont) {
                float ly = iy + (isz - lw.y) * 0.5f;
                dl->AddText(watermark::espFont, 15.f * sc, ImVec2(ax + isz + 8.f * sc, ly), sub == s ? IM_COL32(255, 222, 176, 255) : IM_COL32(190, 190, 195, 255), sub_name(s));
            }
            if (mtap && inside(io.MousePos.x, io.MousePos.y, ax - 2.f * sc, iy - 2.f * sc, ax + pill_w, iy + isz + 2.f * sc))
                sub = s;
            cur_x += pill_w + igap + spread[s] * push;
        }

        float fty = ctop + (scount > 0 ? isz + 16.f * sc : 0.f);
        float cell_w = (cw - 10.f * sc) * 0.5f;
        float cell_h = 66.f * sc;
        float cgap = 10.f * sc;
        float set_h = 30.f * sc;
        float set_pad = 8.f * sc;

        static float tog_anim[12] = { 0.f };
        static float exp_a[12] = { 0.f };
        static int exp_key = -1;
        int ekey = tab * 10 + sub;
        if (ekey != exp_key) { exp_key = ekey; for (int k = 0; k < 12; k++) exp_a[k] = 0.f; scroll = 0.f; }
        static setting* s_drag = nullptr;
        if (!down)
            s_drag = nullptr;

        cell cs[12];
        int nc = build_cells(cs);
        float col_y[2] = { fty, fty };
        for (int i = 0; i < nc; i++) {
            int col = i % 2;
            float x = cx + col * (cell_w + cgap);
            float y = col_y[col] - scroll;
            cell& cl = cs[i];
            float tgt = cl.val ? (*cl.val ? 1.f : 0.f) : 1.f;
            exp_a[i] += (tgt - exp_a[i]) * 0.16f;

            float set_area = cl.nset * set_h + set_pad * 2.f;
            float block_h = cell_h + set_area * exp_a[i];

            dl->AddRectFilled(ImVec2(x, y), ImVec2(x + cell_w, y + block_h), IM_COL32(29, 29, 35, 255), 12.f * sc);

            if (watermark::espFont) {
                float ns = 16.f * sc;
                float ds = 12.f * sc;
                float tw = 38.f * sc;
                float tx0 = x + cell_w - 12.f * sc - tw;
                float ny = y + 10.f * sc;
                ImVec2 nw = watermark::espFont->CalcTextSizeA(ns, FLT_MAX, 0.f, cl.name);
                dl->PushClipRect(ImVec2(x + 12.f * sc, y + 8.f * sc), ImVec2(tx0 - 4.f * sc, y + cell_h - 6.f * sc), true);
                dl->AddText(watermark::espFont, ns, ImVec2(x + 12.f * sc, ny), IM_COL32(255, 255, 255, 255), cl.name);
                dl->PopClipRect();
                dl->PushClipRect(ImVec2(x + 12.f * sc, y + 8.f * sc), ImVec2(x + cell_w - 8.f * sc, y + cell_h - 6.f * sc), true);
                dl->AddText(watermark::espFont, ds, ImVec2(x + 12.f * sc, ny + nw.y + 4.f * sc), IM_COL32(150, 150, 155, 255), cl.desc, NULL, x + cell_w - 24.f * sc);
                dl->PopClipRect();
            }

            float tw = 38.f * sc;
            float th = 20.f * sc;
            float tx = x + cell_w - 12.f * sc - tw;
            float ty2 = y + 10.f * sc;
            if (cl.val) {
                tog_anim[i] += (tgt - tog_anim[i]) * 0.25f;
                tog(dl, tx, ty2, tw, th, tog_anim[i]);
            }
            if (mtap && cl.val && inside(io.MousePos.x, io.MousePos.y, x, y, x + cell_w, y + cell_h))
                *cl.val = !*cl.val;

            if (cl.nset > 0 && exp_a[i] > 0.01f) {
                float set_top = y + cell_h;
                float set_bot = y + block_h;
                float slide = (1.f - exp_a[i]) * set_area;
                dl->PushClipRect(ImVec2(x, set_top), ImVec2(x + cell_w, set_bot), true);
                for (int k = 0; k < cl.nset; k++) {
                    setting& st = cl.sets[k];
                    float sy = set_top + set_pad + k * set_h + slide;
                    dl->AddRectFilled(ImVec2(x + 8.f * sc, sy), ImVec2(x + cell_w - 8.f * sc, sy + set_h - 4.f * sc), IM_COL32(255, 255, 255, 8), 6.f * sc);
                    float mid = x + cell_w * 0.5f;
                    float right = x + cell_w - 14.f * sc;
                    if (watermark::espFont) {
                        ImVec2 nm = watermark::espFont->CalcTextSizeA(13.f * sc, FLT_MAX, 0.f, st.name);
                        dl->PushClipRect(ImVec2(x + 12.f * sc, sy - 2.f * sc), ImVec2(mid - 4.f * sc, sy + set_h), true);
                        dl->AddText(watermark::espFont, 13.f * sc, ImVec2(x + 14.f * sc, sy + (set_h - 4.f * sc - nm.y) * 0.5f), IM_COL32(220, 220, 220, 255), st.name);
                        dl->PopClipRect();
                    }
                    if (st.kind == SET_BOOL) {
                        float sw2 = 30.f * sc, sh2 = 16.f * sc;
                        float sx = right - sw2;
                        float sy2 = sy + (set_h - 4.f * sc - sh2) * 0.5f;
                        tog(dl, sx, sy2, sw2, sh2, *st.b ? 1.f : 0.f);
                        if (mtap && inside(io.MousePos.x, io.MousePos.y, sx, sy2, sx + sw2, sy2 + sh2))
                            *st.b = !*st.b;
                    }
                    else if (st.kind == SET_SLIDER) {
                        float sxx = mid;
                        float sw2 = right - mid;
                        float sh2 = 14.f * sc;
                        float sy2 = sy + (set_h - 4.f * sc - sh2) * 0.5f;
                        dl->AddRectFilled(ImVec2(sxx, sy2), ImVec2(sxx + sw2, sy2 + sh2), IM_COL32(70, 70, 76, 255), sh2 * 0.5f);
                        float t = (*st.f - st.lo) / (st.hi - st.lo);
                        if (t < 0) t = 0;
                        if (t > 1) t = 1;
                        float fw = sw2 * t;
                        if (fw > sh2)
                            dl->AddRectFilled(ImVec2(sxx, sy2), ImVec2(sxx + fw, sy2 + sh2), IM_COL32(255, 222, 176, 255), sh2 * 0.5f);
                        dl->AddCircleFilled(ImVec2(sxx + fw, sy2 + sh2 * 0.5f), sh2 * 0.5f + 3.f * sc, IM_COL32(255, 255, 255, 255));
                        bool over = inside(io.MousePos.x, io.MousePos.y, sxx - 4.f * sc, sy2 - 4.f * sc, sxx + sw2 + 4.f * sc, sy2 + sh2 + 4.f * sc);
                        if (press && over && !touch_drag && !cpop)
                            s_drag = &st;
                        if (s_drag == &st && down && !touch_drag) {
                            float v = st.lo + (io.MousePos.x - sxx) / sw2 * (st.hi - st.lo);
                            if (v < st.lo) v = st.lo;
                            if (v > st.hi) v = st.hi;
                            *st.f = v;
                        }
                    }
                    else if (st.kind == SET_TYPE) {
                        const char* cur = st.opt_names[*st.i];
                        if (watermark::espFont) {
                            ImVec2 nm = watermark::espFont->CalcTextSizeA(13.f * sc, FLT_MAX, 0.f, cur);
                            dl->AddText(watermark::espFont, 13.f * sc, ImVec2(right - nm.x, sy + (set_h - 4.f * sc - nm.y) * 0.5f), IM_COL32(255, 222, 176, 255), cur);
                        }
                        if (mtap && inside(io.MousePos.x, io.MousePos.y, mid - 4.f * sc, sy - 4.f * sc, right, sy + set_h)) {
                            *st.i = (*st.i + 1) % st.opts;
                            if (st.i == &s_lang)
                                g_lang = s_lang ? &en : &ru;
                        }
                    }
                    else if (st.kind == SET_COLOR) {
                        float cr = 8.f * sc;
                        float ccx = right - cr - 2.f * sc;
                        float ccy = sy + (set_h - 4.f * sc) * 0.5f;
                        ImU32 cc = IM_COL32((int)(st.c->x * 255), (int)(st.c->y * 255), (int)(st.c->z * 255), (int)(st.c->w * 255));
                        dl->AddCircleFilled(ImVec2(ccx, ccy), cr, cc, 24);
                        dl->AddCircle(ImVec2(ccx, ccy), cr, IM_COL32(0, 0, 0, 200), 24, 1.f);
                        if (mtap && inside(io.MousePos.x, io.MousePos.y, ccx - cr - 4.f * sc, ccy - cr - 4.f * sc, ccx + cr + 4.f * sc, ccy + cr + 4.f * sc))
                            cpop_open(st.c);
                    }
                    else if (st.kind == SET_COLOR2) {
                        float cr = 8.f * sc;
                        float gap = 6.f * sc;
                        float ccx2 = right - cr - 2.f * sc;
                        float ccx1 = ccx2 - cr * 2.f - gap;
                        float ccy = sy + (set_h - 4.f * sc) * 0.5f;
                        ImU32 c1 = IM_COL32((int)(st.c->x * 255), (int)(st.c->y * 255), (int)(st.c->z * 255), (int)(st.c->w * 255));
                        ImU32 c2 = IM_COL32((int)(st.c2->x * 255), (int)(st.c2->y * 255), (int)(st.c2->z * 255), (int)(st.c2->w * 255));
                        dl->AddCircleFilled(ImVec2(ccx1, ccy), cr, c1, 24);
                        dl->AddCircle(ImVec2(ccx1, ccy), cr, IM_COL32(0, 0, 0, 200), 24, 1.f);
                        dl->AddCircleFilled(ImVec2(ccx2, ccy), cr, c2, 24);
                        dl->AddCircle(ImVec2(ccx2, ccy), cr, IM_COL32(0, 0, 0, 200), 24, 1.f);
                        if (mtap && inside(io.MousePos.x, io.MousePos.y, ccx1 - cr - 4.f * sc, ccy - cr - 4.f * sc, ccx1 + cr + 4.f * sc, ccy + cr + 4.f * sc))
                            cpop_open(st.c);
                        if (mtap && inside(io.MousePos.x, io.MousePos.y, ccx2 - cr - 4.f * sc, ccy - cr - 4.f * sc, ccx2 + cr + 4.f * sc, ccy + cr + 4.f * sc))
                            cpop_open(st.c2);
                    }
                }
                dl->PopClipRect();
            }

            col_y[col] += block_h + cgap;
        }

        float content_bottom = col_y[0] > col_y[1] ? col_y[0] : col_y[1];
        content_bottom -= cgap;

        dl->PopClipRect();

        cpop_draw(dl, sc, io.MousePos.x, io.MousePos.y, mx, my, mw, mh, press, tap, down);

        float view_bottom = my + mh - 8.f * sc;
        float max_scroll = content_bottom - view_bottom;
        if (max_scroll < 0.f) max_scroll = 0.f;
        if (scroll > max_scroll) scroll = max_scroll;
        if (scroll < 0.f) scroll = 0.f;
    }

}