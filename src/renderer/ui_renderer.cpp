#include "ui_renderer.h"
#include <cstring>
#include <cstdio>
#include <cmath>

// ─── Dimensiones del strip colapsado ─────────────────────────────────────────
static constexpr int   STRIP_W        = 22;   // ancho del strip en reposo
static constexpr int   EXPAND_W       = 210;  // ancho del item expandido al hover
static constexpr float ITEM_H_BASE    = 5.0f; // altura de cada elemento en reposo
static constexpr int   STRIP_BG_ALPHA = 178;  // 70 % opacidad (~255 * 0.70)

// ─── Colores de UI ────────────────────────────────────────────────────────────
static constexpr Color C_STRIP_BG   = {8,   8,  18, STRIP_BG_ALPHA};
static constexpr Color C_HOVER_BG   = {20,  40,  90, 220};
static constexpr Color C_SEL_BG     = {30,  70, 160, 230};
static constexpr Color C_TEXT       = {180, 210, 240, 255};
static constexpr Color C_DIM        = {80,  110, 150, 180};
static constexpr Color C_ACCENT     = {80,  200, 255, 255};
static constexpr Color C_BORDER     = {40,   80, 140,  90};

// ─── Nombres de suborbitales (para panel de info) ────────────────────────────
static const char* SUBORB_LABEL[SUBORB_COUNT] = {
    "1s","2s","2p","3s","3p","4s","3d","4p",
    "5s","4d","5p","6s","4f","5d","6p","7s","5f","6d","7p"
};
static const Color ORBC[4] = {
    {100, 200, 255, 255},
    {100, 255, 150, 255},
    {255, 180,  80, 255},
    {220,  80, 255, 255},
};

// ─────────────────────────────────────────────────────────────────────────────
// STRIP LATERAL IZQUIERDO
// ─────────────────────────────────────────────────────────────────────────────
bool drawCollapsedStrip(UIState& ui, int& selectedIndex) {
    int sh     = GetScreenHeight();
    bool changed = false;

    // Altura real por item: distribuir equitativamente en la pantalla
    float itemH = (float)sh / 118.0f;

    // Fondo del strip
    DrawRectangle(0, 0, STRIP_W, sh, C_STRIP_BG);

    Vector2 mouse = GetMousePosition();
    ui.hoveredIndex = -1;

    // Scroll con rueda si el mouse está sobre el strip expandido
    if (mouse.x < EXPAND_W) {
        ui.scrollOffset -= (int)GetMouseWheelMove() * 5;
        if (ui.scrollOffset < 0)   ui.scrollOffset = 0;
        if (ui.scrollOffset > 100) ui.scrollOffset = 100;
    }

    for (int i = 0; i < 118; i++) {
        float y = i * itemH;
        float yCenter = y + itemH * 0.5f;

        // ── Banda de color (categoría) en el borde ──────────────────────────
        Color band = ELEMENTS[i].color;
        band.a = (i == selectedIndex) ? 255 : 160;
        DrawRectangle(0, (int)y, 4, (int)itemH + 1, band);

        // ── Área de detección de hover ───────────────────────────────────────
        bool isHovered = (mouse.x >= 0 && mouse.x < EXPAND_W &&
                          mouse.y >= y && mouse.y < y + itemH);
        bool isSelected = (i == selectedIndex);

        if (isHovered) ui.hoveredIndex = i;

        // ── Símbolo pequeño en el strip (siempre visible) ────────────────────
        // Solo dibujamos texto si itemH >= 9px para que se pueda leer algo
        if (itemH >= 8.0f) {
            int fsz = (int)(itemH * 0.72f);
            if (fsz < 6)  fsz = 6;
            if (fsz > 11) fsz = 11;
            Color tc = isSelected ? WHITE : C_DIM;
            tc.a = isSelected ? 255 : 120;
            DrawText(ELEMENTS[i].symbol, 6, (int)(yCenter - fsz * 0.5f), fsz, tc);
        }

        // ── Item expandido al hover ───────────────────────────────────────────
        if (isHovered) {
            // Fondo expandido — semi-transparente
            Color bgExp = isSelected ? C_SEL_BG : C_HOVER_BG;
            DrawRectangle(0, (int)y, EXPAND_W, (int)itemH + 1, bgExp);

            // Borde izquierdo coloreado
            DrawRectangle(0, (int)y, 4, (int)itemH + 1, ELEMENTS[i].color);

            // Número atómico
            char zBuf[8];
            snprintf(zBuf, sizeof(zBuf), "%d", ELEMENTS[i].atomicNumber);
            DrawText(zBuf, 7, (int)(yCenter - 6), 10, C_DIM);

            // Símbolo grande
            int symFsz = (int)(itemH * 0.9f);
            if (symFsz < 10) symFsz = 10;
            if (symFsz > 18) symFsz = 18;
            DrawText(ELEMENTS[i].symbol, 32, (int)(yCenter - symFsz * 0.5f), symFsz, WHITE);

            // Nombre (si cabe)
            int nameFsz = 10;
            int nameX   = 32 + MeasureText(ELEMENTS[i].symbol, symFsz) + 6;
            if (nameX + MeasureText(ELEMENTS[i].name, nameFsz) < EXPAND_W - 4) {
                DrawText(ELEMENTS[i].name, nameX, (int)(yCenter - nameFsz * 0.5f),
                         nameFsz, C_TEXT);
            }

            // Categoría (si hay suficiente altura)
            if (itemH >= 20.0f) {
                DrawText(ELEMENTS[i].category, 32, (int)(y + itemH - 13), 9, C_DIM);
            }

            // Click = selección
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedIndex = i;
                changed = true;
            }
        } else if (isSelected) {
            // Item seleccionado pero no hovered: destacar en el strip
            DrawRectangle(0, (int)y, STRIP_W, (int)itemH + 1, {30, 70, 160, 130});
        }
    }

    // Borde derecho del strip
    DrawLine(STRIP_W, 0, STRIP_W, sh, C_BORDER);

    return changed;
}

// ─────────────────────────────────────────────────────────────────────────────
// BOTÓN DE INFO (flotante, centro-derecha)
// ─────────────────────────────────────────────────────────────────────────────
bool drawInfoButton(UIState& ui, float dt) {
    ui.infoButtonPulse += dt * 2.5f;
    if (ui.infoButtonPulse > 6.2831f) ui.infoButtonPulse -= 6.2831f;

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    float btnX = (float)(sw - 44);
    float btnY = (float)(sh / 2 - 22);
    float btnW = 40.0f;
    float btnH = 44.0f;

    Rectangle btnRect = {btnX, btnY, btnW, btnH};
    Vector2   mouse   = GetMousePosition();
    bool      hovered = CheckCollisionPointRec(mouse, btnRect);

    // Pulso de brillo
    float pulse = 0.5f + 0.5f * sinf(ui.infoButtonPulse);
    unsigned char glowA = ui.infoPanelOpen ? 220 : (unsigned char)(60 + pulse * 60);

    // Fondo del botón
    Color bgBtn = ui.infoPanelOpen
                  ? Color{30, 100, 200, 220}
                  : Color{15,  25,  50, 180};
    if (hovered) bgBtn.a = 240;

    DrawRectangleRounded(btnRect, 0.35f, 6, bgBtn);
    DrawRectangleRoundedLines(btnRect, 0.35f, 6, 1.0f,
                              {(unsigned char)(80 + (int)(pulse * 60)), 180, 255, glowA});

    // Icono "i"
    int fsz = 20;
    int tw  = MeasureText("i", fsz);
    DrawText("i",
             (int)(btnX + btnW / 2 - tw / 2),
             (int)(btnY + btnH / 2 - fsz / 2),
             fsz,
             hovered ? WHITE : C_ACCENT);

    // Etiqueta pequeña
    DrawText("INFO", (int)(btnX + 2), (int)(btnY + btnH - 13), 8, C_DIM);

    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        ui.infoPanelOpen = !ui.infoPanelOpen;
        return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// PANEL DE INFO (overlay flotante, toggleable)
// ─────────────────────────────────────────────────────────────────────────────
void drawInfoOverlay(const Element& elem, const AtomCloud& cloud, UIState& ui) {
    if (!ui.infoPanelOpen) return;

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    int panW = 230;
    int panH = 360;
    int panX = sw - panW - 55; // a la izquierda del botón
    int panY = sh / 2 - panH / 2;

    // Fondo del overlay — más transparente que en la versión anterior
    DrawRectangleRounded({(float)panX, (float)panY, (float)panW, (float)panH},
                         0.06f, 8, {8, 10, 24, 210});
    DrawRectangleRoundedLines({(float)panX, (float)panY, (float)panW, (float)panH},
                               0.06f, 8, 1.0f, {40, 80, 160, 120});

    int x = panX + 12;
    int y = panY + 12;
    const int PAD = 12;

    // ── Cabecera ──────────────────────────────────────────────────────────────
    // Símbolo grande con color de elemento
    DrawText(elem.symbol, x, y, 44, elem.color);
    int symW = MeasureText(elem.symbol, 44);

    // Z a la derecha del símbolo
    char zBuf[16];
    snprintf(zBuf, sizeof(zBuf), "Z = %d", elem.atomicNumber);
    DrawText(zBuf, x + symW + 8, y + 6, 14, C_ACCENT);

    y += 50;
    DrawText(elem.name, x, y, 13, WHITE);
    y += 16;
    DrawText(elem.category, x, y, 10, C_DIM);
    y += 14;

    // Masa atómica
    char massBuf[32];
    snprintf(massBuf, sizeof(massBuf), "Masa: %.3f u", elem.atomicMass);
    DrawText(massBuf, x, y, 10, C_TEXT);
    y += 14;

    // Capas
    char shellBuf[16];
    snprintf(shellBuf, sizeof(shellBuf), "Capas: %d", elem.shells);
    DrawText(shellBuf, x, y, 10, C_TEXT);
    y += 16;

    // Separador
    DrawLine(x, y, panX + panW - PAD, y, {40, 80, 140, 80});
    y += 8;

    // ── Configuración electrónica ─────────────────────────────────────────────
    DrawText("Config. electronica:", x, y, 10, C_ACCENT);
    y += 13;

    float cx = (float)x;
    float cy = (float)y;
    for (int i = 0; i < SUBORB_COUNT; i++) {
        if (elem.config[i] == 0) continue;
        char buf[12];
        snprintf(buf, sizeof(buf), "%s^%d", SUBORB_LABEL[i], elem.config[i]);
        int bw = MeasureText(buf, 10);
        if (cx + bw > panX + panW - PAD) {
            cx = (float)x;
            cy += 13.0f;
        }
        DrawText(buf, (int)cx, (int)cy, 10, ORBC[SUBORB_L[i]]);
        cx += bw + 5.0f;
    }
    y = (int)(cy + 18);

    // Separador
    DrawLine(x, y, panX + panW - PAD, y, {40, 80, 140, 80});
    y += 8;

    // ── Orbitales activos ─────────────────────────────────────────────────────
    static const char* ORBN[] = {"s","p","d","f"};
    DrawText("Orbitales:", x, y, 10, C_ACCENT);
    y += 13;

    for (const auto& orb : cloud.orbitals) {
        if (y > panY + panH - 16) break;
        char orbBuf[24];
        snprintf(orbBuf, sizeof(orbBuf), "%d%s  %de",
                 orb.n, ORBN[orb.l], orb.electrons);
        DrawText(orbBuf, x + 4, y, 10, ORBC[orb.l]);
        y += 12;
    }

    // FPS esquina inferior
    char fpsBuf[12];
    snprintf(fpsBuf, sizeof(fpsBuf), "FPS %d", GetFPS());
    DrawText(fpsBuf, panX + panW - 40, panY + panH - 14, 9, C_DIM);
}
