#include "raylib.h"
#include "data/elements.h"
#include "physics/quantum.h"
#include "renderer/atom_renderer.h"
#include "renderer/ui_renderer.h"
#include <cstring>
#include <cmath>
#include <cstdio>

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

    float zoomExp    = 0.0f;
    float zoomFactor = 1.0f;

    // Lambda para recargar elemento preservando estado
    auto reloadCloud = [&](int idx) {
        bool  prevAnim = cloud.animationEnabled;
        float prevAngY = cloud.rotationAngle;
        float prevAngX = cloud.rotationAngleX;
        cloud = makeCloud(idx);
        cloud.animationEnabled = prevAnim;
        cloud.rotationAngle    = prevAngY;
        cloud.rotationAngleX   = prevAngX;
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Navegacion de elementos
        if (IsKeyPressed(KEY_DOWN) && selectedIndex < 117) reloadCloud(++selectedIndex);
        if (IsKeyPressed(KEY_UP)   && selectedIndex > 0)   reloadCloud(--selectedIndex);

        // Toggle animacion: Space
        if (IsKeyPressed(KEY_SPACE)) cloud.animationEnabled = !cloud.animationEnabled;

        updateAtomCloud(cloud, dt);

        // Zoom con rueda del mouse (escala logaritmica)
        float scroll = GetMouseWheelMove();
        if (scroll != 0.0f) {
            zoomExp += scroll * 0.15f;
            if (zoomExp < -2.0f) zoomExp = -2.0f;
            if (zoomExp >  6.0f) zoomExp =  6.0f;
            zoomFactor = powf(10.0f, zoomExp);
        }

        // Rotacion por arrastre del mouse
        float mx = (float)GetMouseX();
        float my = (float)GetMouseY();
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        bool overStrip   = (mx < 30.0f);
        bool overInfoBtn = (mx > sw - 50 && my > sh/2 - 30 && my < sh/2 + 30);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !overStrip && !overInfoBtn) {
            Vector2 delta = GetMouseDelta();
            cloud.rotationAngle  += delta.x * 0.5f;
            cloud.rotationAngleX += delta.y * 0.5f;
            if (cloud.rotationAngleX >  85.0f) cloud.rotationAngleX =  85.0f;
            if (cloud.rotationAngleX < -85.0f) cloud.rotationAngleX = -85.0f;
        }

        float atomCX = sw / 2.0f;
        float atomCY = sh / 2.0f;

        BeginDrawing();
        ClearBackground({5, 5, 15, 255});

        drawAtom(cloud, ELEMENTS[selectedIndex], atomCX, atomCY, zoomFactor);

        // UI
        bool stripChanged = drawCollapsedStrip(ui, selectedIndex);
        if (stripChanged) reloadCloud(selectedIndex);
        drawInfoButton(ui, dt);
        drawInfoOverlay(ELEMENTS[selectedIndex], cloud, ui);

        // Titulo
        const char* title = "ATOM-VS";
        int tw = MeasureText(title, 16);
        DrawText(title, sw / 2 - tw / 2, 10, 16, {40, 80, 160, 120});

        // HUD: zoom
        char zoomBuf[32];
        if (zoomFactor < 1000.0f)
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.1f", zoomFactor);
        else if (zoomFactor < 1e6f)
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.0fk", zoomFactor / 1000.0f);
        else
            snprintf(zoomBuf, sizeof(zoomBuf), "zoom x%.2fM", zoomFactor / 1e6f);
        DrawText(zoomBuf, sw - MeasureText(zoomBuf, 10) - 60, sh - 20, 10, {60, 100, 160, 160});

        // HUD: boton de animacion (clickeable, centro inferior)
        const char* animLabel = cloud.animationEnabled ? "[ ANIM: ON  ]" : "[ ANIM: OFF ]";
        Color animC = cloud.animationEnabled
                      ? Color{80, 220, 100, 220}
                      : Color{120, 120, 140, 160};
        int almw = MeasureText(animLabel, 12);
        int almX = sw / 2 - almw / 2;
        int almY = sh - 26;
        DrawRectangleRounded({(float)(almX-8), (float)(almY-5), (float)(almw+16), 22.0f},
                             0.4f, 4, {8, 8, 20, 180});
        DrawRectangleRoundedLines({(float)(almX-8), (float)(almY-5), (float)(almw+16), 22.0f},
                                  0.4f, 4, 1.0f, {40, 80, 120, 100});
        DrawText(animLabel, almX, almY, 12, animC);

        // Click en el boton de animacion
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            float mx2 = (float)GetMouseX();
            float my2 = (float)GetMouseY();
            if (mx2 >= almX-8 && mx2 <= almX+almw+8 && my2 >= almY-5 && my2 <= almY+17)
                cloud.animationEnabled = !cloud.animationEnabled;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
