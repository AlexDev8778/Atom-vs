#pragma once
#include "raylib.h"
#include <vector>

struct Nucleon {
    float x, y, z;
    bool isProton;
};

// Punto de la nube — tiene posición animada (x,y,z) y posición base (ox,oy,oz).
// Cuando la animación está activa, oscila alrededor de la base con jitter estocástico.
struct CloudPoint {
    float x,  y,  z;    // posición actual (animada)
    float ox, oy, oz;   // posición base (original, generada por Monte Carlo)
    float opacity;
    float size;
    float speed;         // intensidad del jitter (mayor = más movimiento)
};

struct OrbitalCloud {
    int   n;
    int   l;
    int   electrons;
    Color color;
    std::vector<CloudPoint> points;
};

struct AtomCloud {
    std::vector<OrbitalCloud> orbitals;
    std::vector<Nucleon>      nucleons;
    float nuclearRadius;

    float rotationAngle;
    float rotationAngleX;
    float pulsePhase;

    bool  animationEnabled;

    // Vibración nuclear
    float vibPhase;
    float vibFreq;
    float vibAmplitude;
    float vibDirX, vibDirY, vibDirZ;
};

AtomCloud generateAtomCloud(int atomicNumber, const int config[19], float atomicMass);
void      updateAtomCloud(AtomCloud& cloud, float dt);
