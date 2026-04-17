#include "quantum.h"
#include "../data/elements.h"
#include <random>
#include <cmath>
#include <cstdlib>

// Constante QMATH_PI
static constexpr float QMATH_PI = 3.14159265358979f;

// Generador Mersenne Twister — reproducible con semilla fija por elemento.
// Esto garantiza que la nube sea SIEMPRE la misma para el mismo elemento.
static std::mt19937 rng(42);

// Genera puntos distribuidos esféricamente (orbital s).
// La función de onda del hidrógeno 1s es: psi ~ exp(-r/a0)
// La densidad de probabilidad es: |psi|^2 ~ exp(-2r/a0)
// Para n>1, el radio más probable escala con n².
static void generateS(OrbitalCloud& orb, int pointCount, float baseRadius) {
    std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
    // Distribución exponencial para el radio: p(r) ~ r² * exp(-2r/n²)
    // Usamos método de rechazo simplificado: muestreo de r en [0, 3*n²]
    float rMax = 3.0f * baseRadius;

    int generated = 0;
    int attempts  = 0;
    while (generated < pointCount && attempts < pointCount * 20) {
        attempts++;
        float r     = uniformDist(rng) * rMax;
        float prob  = (r * r) * expf(-2.0f * r / baseRadius);
        float pMax  = (baseRadius * baseRadius) * expf(-2.0f); // máximo en r=n²
        if (uniformDist(rng) > prob / pMax) continue;

        // Punto aceptado — convertir a coordenadas esféricas aleatorias
        float theta = acosf(1.0f - 2.0f * uniformDist(rng));
        float phi   = 2.0f * QMATH_PI * uniformDist(rng);

        CloudPoint p;
        p.x = r * sinf(theta) * cosf(phi);
        p.y = r * sinf(theta) * sinf(phi);
        p.z = r * cosf(theta);
        p.opacity = (prob / pMax) * 0.8f + 0.2f;
        p.size    = 2.5f;
        p.ox = p.x; p.oy = p.y; p.oz = p.z; p.speed = 0.0f;
        orb.points.push_back(p);
        generated++;
    }
}

// Genera puntos para orbital p (2 lóbulos a lo largo de un eje).
// La función angular para p_z: Y ~ cos(theta)
// Densidad: |Y|^2 ~ cos²(theta)
static void generateP(OrbitalCloud& orb, int pointCount, float baseRadius, int axis) {
    std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
    float rMax = 3.5f * baseRadius;

    int generated = 0;
    int attempts  = 0;
    while (generated < pointCount && attempts < pointCount * 20) {
        attempts++;
        float r     = uniformDist(rng) * rMax;
        float radialProb = (r * r) * expf(-2.0f * r / baseRadius);
        float rMax2 = (baseRadius * baseRadius) * expf(-2.0f);

        float theta = acosf(1.0f - 2.0f * uniformDist(rng));
        float phi   = 2.0f * QMATH_PI * uniformDist(rng);

        // Factor angular: cos²θ para eje Z, sin²θ*cos²φ para X, sin²θ*sin²φ para Y
        float angularProb = 0.0f;
        if      (axis == 0) angularProb = sinf(theta) * sinf(theta) * cosf(phi) * cosf(phi);
        else if (axis == 1) angularProb = sinf(theta) * sinf(theta) * sinf(phi) * sinf(phi);
        else                angularProb = cosf(theta) * cosf(theta);

        float totalProb = (radialProb / rMax2) * angularProb;
        if (uniformDist(rng) > totalProb) continue;

        CloudPoint p;
        p.x = r * sinf(theta) * cosf(phi);
        p.y = r * sinf(theta) * sinf(phi);
        p.z = r * cosf(theta);
        p.opacity = totalProb * 0.9f + 0.1f;
        p.size    = 2.0f;
        p.ox = p.x; p.oy = p.y; p.oz = p.z; p.speed = 0.0f;
        orb.points.push_back(p);
        generated++;
    }
}

// Genera puntos para orbital d (4 lóbulos, clover-leaf).
// Usamos d_z² como aproximación: Y ~ (3cos²θ - 1)
static void generateD(OrbitalCloud& orb, int pointCount, float baseRadius, int variant) {
    std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
    float rMax = 4.0f * baseRadius;

    int generated = 0;
    int attempts  = 0;
    while (generated < pointCount && attempts < pointCount * 25) {
        attempts++;
        float r    = uniformDist(rng) * rMax;
        float radP = (r * r) * expf(-2.0f * r / baseRadius);
        float rMax2= (baseRadius * baseRadius) * expf(-2.0f);

        float theta = acosf(1.0f - 2.0f * uniformDist(rng));
        float phi   = 2.0f * QMATH_PI * uniformDist(rng);

        float cosT = cosf(theta);
        float sinT = sinf(theta);
        float angP = 0.0f;

        switch (variant % 5) {
            case 0: { float v = 3*cosT*cosT - 1; angP = v*v * 0.25f; break; }
            case 1: angP = cosT*cosT * sinT*sinT * cosf(phi)*cosf(phi); break;
            case 2: angP = cosT*cosT * sinT*sinT * sinf(phi)*sinf(phi); break;
            case 3: angP = sinT*sinT*sinT*sinT * cosf(2*phi)*cosf(2*phi); break;
            case 4: angP = sinT*sinT*sinT*sinT * sinf(2*phi)*sinf(2*phi); break;
        }

        float totalProb = (radP / rMax2) * angP;
        if (uniformDist(rng) > totalProb + 0.02f) continue;

        CloudPoint p;
        p.x = r * sinT * cosf(phi);
        p.y = r * sinT * sinf(phi);
        p.z = r * cosT;
        p.opacity = totalProb * 0.85f + 0.15f;
        p.size    = 1.8f;
        p.ox = p.x; p.oy = p.y; p.oz = p.z; p.speed = 0.0f;
        orb.points.push_back(p);
        generated++;
    }
}

// Genera puntos para orbital f (8 lóbulos, complejísimo).
// Aproximación: f_z³ = Y ~ cosθ * (5cos²θ - 3cosθ)
static void generateF(OrbitalCloud& orb, int pointCount, float baseRadius, int variant) {
    std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
    float rMax = 5.0f * baseRadius;

    int generated = 0;
    int attempts  = 0;
    while (generated < pointCount && attempts < pointCount * 30) {
        attempts++;
        float r    = uniformDist(rng) * rMax;
        float radP = (r * r) * expf(-2.0f * r / baseRadius);
        float rMax2= (baseRadius * baseRadius) * expf(-2.0f);

        float theta = acosf(1.0f - 2.0f * uniformDist(rng));
        float phi   = 2.0f * QMATH_PI * uniformDist(rng);

        float cosT = cosf(theta);
        float sinT = sinf(theta);
        float angP = 0.0f;

        // Variantes de orbitales f simplificadas
        switch (variant % 7) {
            case 0: { float v = cosT*(5*cosT*cosT - 3); angP = v*v * 0.1f; break; }
            case 1: angP = sinT*sinT * (5*cosT*cosT - 1) * (5*cosT*cosT - 1) * cosf(phi)*cosf(phi) * 0.05f; break;
            case 2: angP = sinT*sinT * (5*cosT*cosT - 1) * (5*cosT*cosT - 1) * sinf(phi)*sinf(phi) * 0.05f; break;
            case 3: angP = sinT*sinT*sinT*sinT * cosT*cosT * cosf(2*phi)*cosf(2*phi) * 0.4f; break;
            case 4: angP = sinT*sinT*sinT*sinT * cosT*cosT * sinf(2*phi)*sinf(2*phi) * 0.4f; break;
            case 5: angP = sinT*sinT*sinT*sinT*sinT*sinT * cosf(3*phi)*cosf(3*phi) * 0.6f; break;
            case 6: angP = sinT*sinT*sinT*sinT*sinT*sinT * sinf(3*phi)*sinf(3*phi) * 0.6f; break;
        }

        float totalProb = (radP / rMax2) * angP + 0.01f;
        if (uniformDist(rng) > totalProb) continue;

        CloudPoint p;
        p.x = r * sinT * cosf(phi);
        p.y = r * sinT * sinf(phi);
        p.z = r * cosT;
        p.opacity = (totalProb - 0.01f) * 0.8f + 0.2f;
        p.size    = 1.5f;
        p.ox = p.x; p.oy = p.y; p.oz = p.z; p.speed = 0.0f;
        orb.points.push_back(p);
        generated++;
    }
}

// Paleta de colores para suborbitales — se cicla si hay más de los definidos
static const Color ORBITAL_COLORS[] = {
    {100, 200, 255, 200},  // s — azul suave
    {100, 255, 150, 200},  // p — verde brillante
    {255, 180,  80, 200},  // d — naranja cálido
    {220,  80, 255, 200},  // f — violeta
};

AtomCloud generateAtomCloud(int atomicNumber, const int config[19], float atomicMass) {
    rng.seed(42 + atomicNumber * 7);

    AtomCloud cloud;
    cloud.rotationAngle  = 0.0f;
    cloud.rotationAngleX = 0.0f;
    cloud.pulsePhase     = 0.0f;
    cloud.animationEnabled = false;

    // ── NUBE ELECTRÓNICA ────────────────────────────────────
    float baseUnit = 28.0f;
    int dVariant = 0;
    int fVariant = 0;

    for (int i = 0; i < SUBORB_COUNT; i++) {
        if (config[i] == 0) continue;
        int n         = SUBORB_N[i];
        int l         = SUBORB_L[i];
        int electrons = config[i];
        float radius  = baseUnit * (float)(n * n) / 4.0f;

        OrbitalCloud orb;
        orb.n         = n;
        orb.l         = l;
        orb.electrons = electrons;
        orb.color     = ORBITAL_COLORS[l % 4];

        int pts = 30 + (electrons * 8);
        if (pts > 150) pts = 150;

        switch (l) {
            case 0: generateS(orb, pts, radius); break;
            case 1:
                for (int ax = 0; ax < 3 && orb.points.size() < (size_t)pts; ax++)
                    generateP(orb, pts / 3, radius, ax);
                break;
            case 2: generateD(orb, pts, radius, dVariant++); break;
            case 3: generateF(orb, pts, radius, fVariant++); break;
        }

        // Asignar speed a todos los puntos recién generados.
        // Electrones internos (n bajo) se mueven más rápido.
        float speed = 80.0f / ((float)n * n);
        for (auto& p : orb.points) {
            if (p.speed == 0.0f) p.speed = speed; // solo los recién creados
        }

        cloud.orbitals.push_back(orb);
    }

    // ── NÚC LEO: PROTONES Y NEUTRONES ─────────────────────────
    int Z = atomicNumber;
    int A = (int)(atomicMass + 0.5f);       // redondeo al entero más cercano
    int N = A - Z;
    if (N < 0) N = 0;

    // Radio nuclear real: R = 1.2 * A^(1/3)  [fm]
    float R_nuc = 1.2f * cbrtf((float)A);
    cloud.nuclearRadius = R_nuc;

    // RNG propio para los nucleones (semilla diferente)
    std::mt19937 nucRng(atomicNumber * 31 + 17);
    std::uniform_real_distribution<float> u(-1.0f, 1.0f);

    cloud.nucleons.reserve(A);
    int placedP = 0, placedN = 0;

    // Empaquetamos nucleones uno a uno dentro de la esfera R_nuc.
    // Alternamos protón/neutrón por proporción para mezclarlos uniformemente.
    int maxAttempts = A * 200;
    for (int attempt = 0; attempt < maxAttempts && (placedP < Z || placedN < N); attempt++) {
        float x = u(nucRng) * R_nuc;
        float y = u(nucRng) * R_nuc;
        float z = u(nucRng) * R_nuc;
        if (x*x + y*y + z*z > R_nuc * R_nuc) continue; // fuera de la esfera

        // ¿Próximo a colocar es protón o neutrón?
        bool placeProton = (placedP * (N + Z) <= Z * (placedP + placedN));
        if (placeProton && placedP < Z) {
            cloud.nucleons.push_back({x, y, z, true});
            placedP++;
        } else if (!placeProton && placedN < N) {
            cloud.nucleons.push_back({x, y, z, false});
            placedN++;
        }
    }
    // Completar los que faltaron
    while (placedP < Z) { cloud.nucleons.push_back({0,0,0,true});  placedP++; }
    while (placedN < N) { cloud.nucleons.push_back({0,0,0,false}); placedN++; }

    // ── VIBRACIÓN NUCLEAR ────────────────────────────────────
    cloud.vibPhase     = 0.0f;
    cloud.vibFreq      = 2.0f * QMATH_PI * (2.0f + (atomicNumber % 7) * 0.4f); // 2..4.8 Hz
    cloud.vibAmplitude = 4.5f / sqrtf((float)A);  // núcleos pesados vibran menos
    if (cloud.vibAmplitude > 6.0f) cloud.vibAmplitude = 6.0f;

    // Dirección aleatoria fija para la vibración
    float vx = u(nucRng), vy = u(nucRng), vz = u(nucRng);
    float vlen = sqrtf(vx*vx + vy*vy + vz*vz);
    if (vlen < 0.001f) vlen = 1.0f;
    cloud.vibDirX = vx / vlen;
    cloud.vibDirY = vy / vlen;
    cloud.vibDirZ = vz / vlen;

    return cloud;
}

void updateAtomCloud(AtomCloud& cloud, float dt) {
    if (!cloud.animationEnabled) return;

    // Vibración del núcleo
    cloud.vibPhase += cloud.vibFreq * dt;
    if (cloud.vibPhase > 2.0f * QMATH_PI) cloud.vibPhase -= 2.0f * QMATH_PI;

    // Todos los puntos de la nube usan la misma lógica:
    //   xfit restauradora hacia la posición base (fuerza de muelle suave)
    //   + kick aleatorio proporcional a speed del orbital
    static constexpr float RESTORE_K = 3.5f; // rigidez del muelle

    for (auto& orb : cloud.orbitals) {
        for (auto& p : orb.points) {
            // Fuerza restauradora hacia la posición base
            p.x += (p.ox - p.x) * RESTORE_K * dt;
            p.y += (p.oy - p.y) * RESTORE_K * dt;
            p.z += (p.oz - p.z) * RESTORE_K * dt;

            // Kick aleatorio (jitter estocástico)
            float jit = p.speed * dt;
            p.x += ((rand() % 1000) / 500.0f - 1.0f) * jit;
            p.y += ((rand() % 1000) / 500.0f - 1.0f) * jit;
            p.z += ((rand() % 1000) / 500.0f - 1.0f) * jit;
        }
    }
}
