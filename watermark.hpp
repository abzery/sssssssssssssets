#pragma once

#include "imgui.h"
#include "esp_font.h"
#include "fa_data.h"
#include "logo_png.h"
#include <GLES3/gl3.h>
#include <cstdio>

extern "C" {
    unsigned char* stbi_load_from_memory(unsigned char const* buffer, int len, int* x, int* y, int* comp, int req_comp);
    void stbi_image_free(void* retval_from_stbi_load);
}

namespace menu { extern bool visible; }

namespace watermark {

    inline ImFont* espFont = nullptr;
    inline ImFont* iconFont = nullptr;
    inline ImTextureID logoTex = nullptr;

    inline void init() {
        if (espFont)
            return;

        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = false;
        espFont = io.Fonts->AddFontFromMemoryTTF(esp, sizeof(esp), 18, &fc, io.Fonts->GetGlyphRangesCyrillic());

        static const ImWchar fa_ranges[] = { 0xF06E, 0xF06E, 0xF0AC, 0xF0AC, 0xF0E7, 0xF0E7, 0xF013, 0xF013, 0xF109, 0xF109, 0 };
        ImFontConfig fa_cfg;
        fa_cfg.PixelSnapH = true;
        iconFont = io.Fonts->AddFontFromMemoryTTF((void*)fa_ttf, fa_ttf_size, 20, &fa_cfg, fa_ranges);

        int w = 0, h = 0, ch = 0;
        unsigned char* px = stbi_load_from_memory(logo_png, (int)sizeof(logo_png), &w, &h, &ch, 4);
        if (px && w > 0 && h > 0) {
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
            glBindTexture(GL_TEXTURE_2D, 0);
            logoTex = (ImTextureID)(intptr_t)tex;
            stbi_image_free(px);
        }
    }

    inline void render() {
        if (!espFont)
            return;

        ImGuiIO& io = ImGui::GetIO();
        float sw = io.DisplaySize.x;

        float sc = sw / 1920.f;

        float icon_sz = 30.f * sc;
        float pad_x = 16.f * sc;
        float pad_y = 10.f * sc;
        float gap = 10.f * sc;
        float text_sz = 22.f * sc;
        float rounding = 26.f * sc;
        float sep_w = 1.5f * sc;
        float sep_h = 14.f * sc;
        float sep_gap = 10.f * sc;

        ImVec2 ts1 = espFont ? espFont->CalcTextSizeA(text_sz, FLT_MAX, 0.f, "@sintsex_cheats") : ImVec2(0, text_sz);  // ← ИЗМЕНЕНО!
        ImVec2 ts2 = espFont ? espFont->CalcTextSizeA(text_sz, FLT_MAX, 0.f, "internal") : ImVec2(0, text_sz);        // ← ИЗМЕНЕНО!
        ImVec2 ts3 = iconFont ? iconFont->CalcTextSizeA(text_sz, FLT_MAX, 0.f, "\uf109") : ImVec2(0, text_sz);

        float content_w = pad_x + icon_sz + gap + ts1.x + sep_gap + sep_w + sep_gap + ts3.x + gap + ts2.x + pad_x;
        float content_h = pad_y + (icon_sz > ts1.y ? icon_sz : ts1.y) + pad_y;

        float cx = sw * 0.5f;
        float cy = 100.f * sc;

        float rx = cx - content_w * 0.5f;
        float ry = cy - content_h * 0.5f;

        ImDrawList* dl = ImGui::GetForegroundDrawList();

        dl->AddRectFilled(
            ImVec2(rx, ry),
            ImVec2(rx + content_w, ry + content_h),
            IM_COL32(0, 0, 0, 230),
            rounding
        );

        float icon_y = ry + (content_h - icon_sz) * 0.5f;
        float icon_x = rx + pad_x;

        if (logoTex) {
            dl->AddImage(
                logoTex,
                ImVec2(icon_x, icon_y),
                ImVec2(icon_x + icon_sz, icon_y + icon_sz),
                ImVec2(0, 0), ImVec2(1, 1),
                IM_COL32(255, 255, 255, 255)
            );
        }

        float cur_x = icon_x + icon_sz + gap;
        float text_y = ry + (content_h - ts1.y) * 0.5f;

        if (espFont) {
            ImU32 txt = IM_COL32(255, 255, 255, 255);

            dl->AddText(espFont, text_sz, ImVec2(cur_x, text_y), txt, "@sintsex_cheats");  // ← ИЗМЕНЕНО!
            cur_x += ts1.x;

            float sep_cy = ry + content_h * 0.5f;
            cur_x += sep_gap;
            dl->AddLine(ImVec2(cur_x, sep_cy - sep_h * 0.5f), ImVec2(cur_x, sep_cy + sep_h * 0.5f), IM_COL32(80, 80, 80, 255), sep_w);
            cur_x += sep_w + sep_gap;

            if (iconFont) {
                dl->AddText(iconFont, text_sz, ImVec2(cur_x, ry + (content_h - ts3.y) * 0.5f), IM_COL32(255, 222, 176, 255), "\uf109");
                cur_x += ts3.x;
                cur_x += gap;
            }

            dl->AddText(espFont, text_sz, ImVec2(cur_x, text_y), txt, "internal");   // ← ИЗМЕНЕНО!
        }

        static bool prev_down = false;
        bool down = io.MouseDown[0];
        bool pressed = down && !prev_down;
        prev_down = down;
        if (pressed) {
            float mx = io.MousePos.x, my = io.MousePos.y;
            if (mx >= rx && mx <= rx + content_w && my >= ry && my <= ry + content_h)
                menu::visible = !menu::visible;
        }
    }

}