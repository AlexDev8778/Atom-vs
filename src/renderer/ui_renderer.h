#pragma once
#include "raylib.h"
#include "../data/elements.h"
#include "../physics/quantum.h"

// Estado de la UI — mantenido de frame a frame en main.cpp
struct UIState {
    int  hoveredIndex;     // índice del elemento bajo el cursor (-1 = ninguno)
    bool infoPanelOpen;    // si el panel de info está visible
    float infoButtonPulse; // fase de animación del botón de info (0..2PI)
    int  scrollOffset;     // scroll del strip izquierdo
};

// Dibuja el strip lateral izquierdo colapsado.
// Devuelve true si el elemento seleccionado cambió.
bool drawCollapsedStrip(UIState& ui, int& selectedIndex);

// Dibuja el botón de info (derecha, centro vertical).
// Devuelve true si fue clickeado (toggle).
bool drawInfoButton(UIState& ui, float dt);

// Dibuja el panel de info flotante (solo si ui.infoPanelOpen == true).
void drawInfoOverlay(const Element& elem, const AtomCloud& cloud, UIState& ui);
