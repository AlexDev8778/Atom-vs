#pragma once
#include "raylib.h"
#include <vector>

// Un nucleon dentro del nucleo (protón o neutrón).
struct Nucleon {
    float x, y, z;  // posición en femtómetros (fm)
    bool isProton;  // true=protón (rojo), false=neutrón (azul)
};

// Un punto de la nube de probabilidad.
// Cada punto tiene posición 3D (proyectada a 2D al renderizar) y opacidad.
struct CloudPoint {
    float x, y, z;     // posición relativa al núcleo (normalizada)
    float opacity;      // densidad de probabilidad en ese punto (0..1)
    float size;         // radio del punto en pixels
};

// Una capa de la nube — agrupa los puntos de un suborbital dado.
// Cada suborbital tiene su distribución propia:
//   s -> esférica
//   p -> bilobular (2 lóbulos a lo largo de un eje)
//   d -> 4 lóbulos (clover-leaf)
//   f -> complejo, 8 lóbulos
struct OrbitalCloud {
    int    n;           // número cuántico principal (radio base)
    int    l;           // número cuántico azimutal (0=s, 1=p, 2=d, 3=f)
    int    electrons;   // electrones en este suborbital
    Color  color;       // color de esta capa
    std::vector<CloudPoint> points;
};

// Estado de animación de la nube completa.
struct AtomCloud {
    std::vector<OrbitalCloud> orbitals;
    std::vector<Nucleon>      nucleons;      // protones y neutrones
    float nuclearRadius;                     // radio nuclear en fm
    float rotationAngle;    // eje Y, controlado por mouse
    float rotationAngleX;   // eje X, controlado por mouse
    float pulsePhase;       // no usado actualmente
};

// Genera la nube de probabilidad Y los nucleones para un elemento dado.
// atomicMass se usa para calcular N = A - Z (número de neutrones).
AtomCloud generateAtomCloud(int atomicNumber, const int config[19], float atomicMass);

// Actualiza la animación. Llamar cada frame.
void updateAtomCloud(AtomCloud& cloud, float dt);
