#include "atom_renderer.h"
#include <cmath>
#include <algorithm>
#include <vector>

static constexpr float RMATH_PI = 3.14159265358979f;

// Escala física: el orbital 1s de H (n=1) tiene radio de Bohr = 52900 fm.
// En nuestra coordenada de display, ese radio = baseUnit * 1*1/4 = 7 px.
// Por lo tanto 1 fm = 7/52900 px en coordenadas de display.
// Los nucleones se almacenan en fm → multiplicamos por PX_PER_FM * zoom al proyectar.
static constexpr float PX_PER_FM = 7.0f / 52900.0f;

// Tamaño visual de un nucleón individual en fm.
// Radio real de un nucleón: ~0.85 fm.
static constexpr float NUCLEON_R_FM = 0.85f;

float calcNucleusRadius(float atomicMass) {
    return 8.0f * cbrtf(atomicMass);
}

static void drawGlowCircle(float x, float y, float radius, Color color, float glowScale) {
    Color outer = color; outer.a = 40;
    DrawCircleV({x, y}, radius * glowScale * 2.0f, outer);
    Color mid = color;   mid.a   = 100;
    DrawCircleV({x, y}, radius * glowScale * 1.4f, mid);
    DrawCircleV({x, y}, radius, color);
}

static void project3D(float x3, float y3, float z3,
                      float angleY, float angleX,
                      float cx, float cy,
                      float& outX, float& outY, float& outDepth) {
    float radY = angleY * RMATH_PI / 180.0f;
    float radX = angleX * RMATH_PI / 180.0f;

    float rx = x3 * cosf(radY) + z3 * sinf(radY);
    float ry = y3;
    float rz = -x3 * sinf(radY) + z3 * cosf(radY);

    float rx2 =  rx;
    float ry2 =  ry * cosf(radX) - rz * sinf(radX);
    float rz2 =  ry * sinf(radX) + rz * cosf(radX);

    float perspective = 1.0f / (1.0f + rz2 * 0.002f);
    outX     = cx + rx2 * perspective;
    outY     = cy + ry2 * perspective;
    outDepth = rz2;
}

void drawAtom(const AtomCloud& cloud, const Element& elem, float cx, float cy, float zoomFactor) {

    // ── NUBE ELECTRÓNICA ──────────────────────────────────────────────────────
    // A zoom alto los puntos estarán fuera de pantalla → Raylib los ignora.
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    for (const auto& orb : cloud.orbitals) {
        Color c = orb.color;
        for (const auto& pt : orb.points) {
            float px, py, depth;
            project3D(pt.x * zoomFactor, pt.y * zoomFactor, pt.z * zoomFactor,
                      cloud.rotationAngle, cloud.rotationAngleX,
                      cx, cy, px, py, depth);

            // Culling manual: si está muy lejos, no dibujar
            if (px < -500 || px > sw + 500 || py < -500 || py > sh + 500) continue;

            float depthFade = 1.0f - (depth * 0.001f);
            if (depthFade < 0.2f) depthFade = 0.2f;
            if (depthFade > 1.0f) depthFade = 1.0f;

            unsigned char alpha = (unsigned char)(pt.opacity * depthFade * 220.0f);
            Color dotColor = {c.r, c.g, c.b, alpha};
            Color glow1    = {c.r, c.g, c.b, (unsigned char)(alpha / 4)};
            Color glow2    = {c.r, c.g, c.b, (unsigned char)(alpha / 2)};

            float sz = pt.size * zoomFactor;
            if (sz < 1.0f) sz = 1.0f;
            if (sz > 20.0f) sz = 20.0f;

            DrawCircleV({px, py}, sz * 2.5f, glow1);
            DrawCircleV({px, py}, sz * 1.4f, glow2);
            DrawCircleV({px, py}, sz,         dotColor);
        }
    }

    // ── NÚCLEO: PROTONES Y NEUTRONES ──────────────────────────────────────────
    // Escala: posición en fm × PX_PER_FM × zoomFactor
    float nucScale  = PX_PER_FM * zoomFactor;
    float nucleonPx = NUCLEON_R_FM * nucScale; // radio visual de un nucleón

    // Colores: protón=rojo, neutrón=azul/gris
    static const Color C_PROTON  = {220,  60,  60, 255};
    static const Color C_NEUTRON = { 60,  90, 200, 255};

    // Ordenamos por depth (back-to-front) para z-ordering correcto.
    // Usamos un vector temp para no modificar el original.
    struct NucDraw { float px, py, depth; bool isProton; };
    static std::vector<NucDraw> nucDrawList; // static para no re-alocar cada frame
    nucDrawList.clear();

    for (const auto& nuc : cloud.nucleons) {
        float px, py, depth;
        project3D(nuc.x * nucScale, nuc.y * nucScale, nuc.z * nucScale,
                  cloud.rotationAngle, cloud.rotationAngleX,
                  cx, cy, px, py, depth);
        if (px < -500 || px > sw + 500 || py < -500 || py > sh + 500) continue;
        nucDrawList.push_back({px, py, depth, nuc.isProton});
    }

    // Ordenar back-to-front (mayor depth = más atrás)
    std::sort(nucDrawList.begin(), nucDrawList.end(),
              [](const NucDraw& a, const NucDraw& b){ return a.depth > b.depth; });

    if (nucleonPx >= 0.5f) {
        // Nucleones visibles — dibujar con glow
        for (const auto& nd : nucDrawList) {
            const Color& C = nd.isProton ? C_PROTON : C_NEUTRON;

            // Sombra/glow exterior
            Color glow = C; glow.a = 60;
            DrawCircleV({nd.px, nd.py}, nucleonPx * 1.8f, glow);

            // Cuerpo
            DrawCircleV({nd.px, nd.py}, nucleonPx, C);

            // Especular: punto blanco pequeño en la esquina superior izquierda
            if (nucleonPx > 4.0f) {
                Color spec = {255, 255, 255, 140};
                DrawCircleV({nd.px - nucleonPx * 0.3f, nd.py - nucleonPx * 0.3f},
                            nucleonPx * 0.25f, spec);
            }
        }
    } else {
        // Nucleones demasiado pequeños para dibujar individualmente —
        // mostramos el núcleo como un mini glow del color del elemento
        float coreR = cloud.nuclearRadius * nucScale;
        if (coreR < 1.5f) coreR = 1.5f;
        drawGlowCircle(cx, cy, coreR, elem.color, 2.0f);
    }
}
