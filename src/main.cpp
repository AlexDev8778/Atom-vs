#include "raylib.h"
#include "data/elements.h"
#include "physics/quantum.h"
#include "renderer/atom_renderer.h"
#include "renderer/ui_renderer.h"
#include <cstring>
#include <cmath>
#include <cstdio>

// Genera el cloud pasando también atomicMass para los nucleones
static AtomCloud makeCloud(int idx) {
    return generateAtomCloud(
        ELEMENTS[idx].atomicNumber,
        ELEMENTS[idx].config,
        ELEMENTS[idx].atomicMass
    );
}

int main(void) {
    const int SCREEN_W = 1000;
    const int SCREEN_H = 700;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_W, SCREEN_H, "Atom-vs — Simulador Atomico");
    SetTargetFPS(60);

    int selectedIndex = 0;
    AtomCloud cloud = makeCloud(selectedIndex);

    UIState ui = {-1, false, 0.0f, 0};

    // ── ZOOM ──────────────────────────────────────────────────────────────────
    // Usamos escala logarítmica: zoomExp en [-2, 6].
    // zoomFactor = 10^zoomExp.
    // zoomExp=0 → zoom=1    (vista de la nube electrónica)
    // zoomExp=5 → zoom=1e5  (núcleo visible con nucleones individuales)
    float zoomExp    = 0.0f;
    float zoomFactor = 1.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ── Navegación de elementos ─────────────────────────────────────────
        if (IsKeyPressed(KEY_DOWN) && selectedIndex < 117) {
            selectedIndex++;
            cloud = makeCloud(selectedIndex);
        }
        if (IsKeyPressed(KEY_UP) && selectedIndex > 0) {
            selectedIndex--;
            cloud = makeCloud(selectedIndex);
        }

        updateAtomCloud(cloud, dt);

        // ── Zoom con rueda del mouse ─────────────────────────────────────────
        float scroll = GetMouseWheelMove();
        if (scroll != 0.0f) {
            zoomExp += scroll * 0.15f;
            if (zoomExp < -2.0f) zoomExp = -2.0f;   // zoom mínimo 0.01x
            if (zoomExp >  6.0f) zoomExp =  6.0f;   // zoom máximo 1,000,000x
            zoomFactor = powf(10.0f, zoomExp);
        }

        // ── Rotación por arrastre (click sostenido fuera del strip/botón) ────
        float mx = (float)GetMouseX();
        float my = (float)GetMouseY();
        bool overStrip   = (mx < 30.0f);
        bool overInfoBtn = (mx > GetScreenWidth() - 50 &&
                            my > GetScreenHeight()/2 - 30 &&
                            my < GetScreenHeight()/2 + 30);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !overStrip && !overInfoBtn) {
            Vector2 delta = GetMouseDelta();
            cloud.rotationAngle  += delta.x * 0.5f;
            cloud.rotationAngleX += delta.y * 0.5f;
            if (cloud.rotationAngleX >  85.0f) cloud.rotationAngleX =  85.0f;
            if (cloud.rotationAngleX < -85.0f) cloud.rotationAngleX = -85.0f;
        }

        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        float atomCX = sw / 2.0f;
        float atomCY = sh / 2.0f;

        BeginDrawing();
        ClearBackground({5, 5, 15, 255});

        drawAtom(cloud, ELEMENTS[selectedIndex], atomCX, atomCY, zoomFactor);

        // UI sobre el átomo
        bool stripChanged = drawCollapsedStrip(ui, selectedIndex);
        if (stripChanged) {
            cloud = makeCloud(selectedIndex);
        }
        drawInfoButton(ui, dt);
        drawInfoOverlay(ELEMENTS[selectedIndex], cloud, ui);

        // Título sutil
        const char* title = "ATOM-VS";
        int tw = MeasureText(title, 16);
        DrawText(title, sw / 2 - tw / 2, 10, 16, {40, 80, 160, 120});

        // Indicador de zoom (HUD mínimo)
        char zoomBuf[32];
        if (zoomFactor < 1000.0f)
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.1f", zoomFactor);
        else if (zoomFactor < 1e6f)
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.0fk", zoomFactor / 1000.0f);
        else
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.2fM", zoomFactor / 1e6f);
        DrawText(zoomBuf, sw - MeasureText(zoomBuf, 10) - 60, sh - 20, 10, {60, 100, 160, 160});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
