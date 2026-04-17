#pragma once
#include "raylib.h"
#include "../physics/quantum.h"
#include "../data/elements.h"

// Renderiza el átomo completo (núcleo + nube de probabilidad + overlay CRT).
// cx, cy = centro de la pantalla donde se dibuja el átomo
// radius = radio del núcleo en pixels (proporcional a A^1/3)
void drawAtom(const AtomCloud& cloud, const Element& elem, float cx, float cy, float zoomFactor);

// Calcula el radio del núcleo: R = R0 * A^(1/3), con R0 = 8px como base visual
float calcNucleusRadius(float atomicMass);
