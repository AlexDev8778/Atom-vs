#pragma once
#include "raylib.h"

// Estructura de un elemento de la tabla periódica.
// Usamos configuración electrónica REAL con subniveles s, p, d, f.
// La distribución sigue el orden de llenado de Aufbau + excepciones reales.
struct Element {
    int atomicNumber;
    const char* symbol;
    const char* name;
    float atomicMass;
    Color color;

    // Configuración electrónica por suborbital.
    // Índices: [1s, 2s, 2p, 3s, 3p, 4s, 3d, 4p, 5s, 4d, 5p, 6s, 4f, 5d, 6p, 7s, 5f, 6d, 7p]
    // Máximos:  [2,  2,  6,  2,  6,  2,  10, 6,  2,  10, 6,  2,  14, 10, 6,  2,  14, 10, 6 ]
    int config[19];

    // Número de capas principales ocupadas
    int shells;

    // Categoría para color de fondo en UI
    const char* category;
};

// Colores por categoría
static constexpr Color COL_ALKALI      = {255, 80,  80,  255}; // Metales alcalinos
static constexpr Color COL_ALKALEARTH  = {255, 160, 80,  255}; // Metales alcalinotérreos
static constexpr Color COL_TRANSITION  = {100, 180, 255, 255}; // Metales de transición
static constexpr Color COL_POSTTRANS   = {120, 220, 150, 255}; // Metales post-transición
static constexpr Color COL_METALLOID   = {200, 200, 100, 255}; // Metaloide
static constexpr Color COL_NONMETAL    = {80,  255, 180, 255}; // No metales
static constexpr Color COL_HALOGEN     = {220, 100, 255, 255}; // Halógeneos
static constexpr Color COL_NOBLEGAS    = {100, 220, 255, 255}; // Gases nobles
static constexpr Color COL_LANTHANIDE  = {255, 200, 100, 255}; // Lantánidos
static constexpr Color COL_ACTINIDE    = {255, 140, 180, 255}; // Actínidos

// Índices para el array config[] — para legibilidad en quantum.cpp
enum SubOrbital {
    S1=0, S2, P2, S3, P3, S4, D3, P4, S5, D4, P5, S6, F4, D5, P6, S7, F5, D6, P7,
    SUBORB_COUNT = 19
};

// Capacidad máxima de cada suborbital
static constexpr int SUBORB_MAX[SUBORB_COUNT] = {
    2,  // 1s
    2,  // 2s
    6,  // 2p
    2,  // 3s
    6,  // 3p
    2,  // 4s
    10, // 3d
    6,  // 4p
    2,  // 5s
    10, // 4d
    6,  // 5p
    2,  // 6s
    14, // 4f
    10, // 5d
    6,  // 6p
    2,  // 7s
    14, // 5f
    10, // 6d
    6,  // 7p
};

// Número cuántico principal de cada suborbital (para radio de nube)
static constexpr int SUBORB_N[SUBORB_COUNT] = {
    1, 2, 2, 3, 3, 4, 3, 4, 5, 4, 5, 6, 4, 5, 6, 7, 5, 6, 7
};

// Letra del suborbital (s, p, d, f) — índice 0=s, 1=p, 2=d, 3=f
static constexpr int SUBORB_L[SUBORB_COUNT] = {
    0, 0, 1, 0, 1, 0, 2, 1, 0, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1
};

// =============================================================================
// TABLA PERIÓDICA COMPLETA — 118 elementos
// config[] sigue el orden de SubOrbital enum arriba.
// Excepciones reales de Cr, Cu, Nb, Mo, Ru, Rh, Pd, Ag, La, Ce, Gd, Pt, Au,
// Ac, Th, Pa, U, Np, Cm están incluidas.
// =============================================================================
static constexpr Element ELEMENTS[118] = {
    // Z   sym    name              mass     color          1s 2s 2p 3s 3p 4s 3d 4p 5s 4d 5p 6s 4f 5d 6p 7s 5f 6d 7p  shells  cat
    {  1, "H",  "Hidrogeno",       1.008f,  COL_NONMETAL,  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, "Nonmetal"},
    {  2, "He", "Helio",           4.003f,  COL_NOBLEGAS,  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1, "Noble gas"},
    {  3, "Li", "Litio",           6.941f,  COL_ALKALI,    {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Alkali metal"},
    {  4, "Be", "Berilio",         9.012f,  COL_ALKALEARTH,{2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Alkaline earth"},
    {  5, "B",  "Boro",           10.811f,  COL_METALLOID, {2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Metalloid"},
    {  6, "C",  "Carbono",        12.011f,  COL_NONMETAL,  {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Nonmetal"},
    {  7, "N",  "Nitrogeno",      14.007f,  COL_NONMETAL,  {2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Nonmetal"},
    {  8, "O",  "Oxigeno",        15.999f,  COL_NONMETAL,  {2, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Nonmetal"},
    {  9, "F",  "Fluor",          18.998f,  COL_HALOGEN,   {2, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Halogen"},
    { 10, "Ne", "Neon",           20.180f,  COL_NOBLEGAS,  {2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2, "Noble gas"},
    { 11, "Na", "Sodio",          22.990f,  COL_ALKALI,    {2, 2, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Alkali metal"},
    { 12, "Mg", "Magnesio",       24.305f,  COL_ALKALEARTH,{2, 2, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Alkaline earth"},
    { 13, "Al", "Aluminio",       26.982f,  COL_POSTTRANS, {2, 2, 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Post-transition"},
    { 14, "Si", "Silicio",        28.086f,  COL_METALLOID, {2, 2, 6, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Metalloid"},
    { 15, "P",  "Fosforo",        30.974f,  COL_NONMETAL,  {2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Nonmetal"},
    { 16, "S",  "Azufre",         32.065f,  COL_NONMETAL,  {2, 2, 6, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Nonmetal"},
    { 17, "Cl", "Cloro",          35.453f,  COL_HALOGEN,   {2, 2, 6, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Halogen"},
    { 18, "Ar", "Argon",          39.948f,  COL_NOBLEGAS,  {2, 2, 6, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, "Noble gas"},
    { 19, "K",  "Potasio",        39.098f,  COL_ALKALI,    {2, 2, 6, 2, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Alkali metal"},
    { 20, "Ca", "Calcio",         40.078f,  COL_ALKALEARTH,{2, 2, 6, 2, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Alkaline earth"},
    { 21, "Sc", "Escandio",       44.956f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 22, "Ti", "Titanio",        47.867f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 23, "V",  "Vanadio",        50.942f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 24, "Cr", "Cromo",          51.996f,  COL_TRANSITION,{2, 2, 6, 2, 6, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"}, // excepcion
    { 25, "Mn", "Manganeso",      54.938f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 26, "Fe", "Hierro",         55.845f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 27, "Co", "Cobalto",        58.933f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 28, "Ni", "Niquel",         58.693f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 29, "Cu", "Cobre",          63.546f,  COL_TRANSITION,{2, 2, 6, 2, 6, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"}, // excepcion
    { 30, "Zn", "Zinc",           65.38f,   COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Transition metal"},
    { 31, "Ga", "Galio",          69.723f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Post-transition"},
    { 32, "Ge", "Germanio",       72.630f,  COL_METALLOID, {2, 2, 6, 2, 6, 2,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Metalloid"},
    { 33, "As", "Arsenico",       74.922f,  COL_METALLOID, {2, 2, 6, 2, 6, 2,10, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Metalloid"},
    { 34, "Se", "Selenio",        78.971f,  COL_NONMETAL,  {2, 2, 6, 2, 6, 2,10, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Nonmetal"},
    { 35, "Br", "Bromo",          79.904f,  COL_HALOGEN,   {2, 2, 6, 2, 6, 2,10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Halogen"},
    { 36, "Kr", "Kripton",        83.798f,  COL_NOBLEGAS,  {2, 2, 6, 2, 6, 2,10, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 4, "Noble gas"},
    { 37, "Rb", "Rubidio",        85.468f,  COL_ALKALI,    {2, 2, 6, 2, 6, 2,10, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Alkali metal"},
    { 38, "Sr", "Estroncio",      87.620f,  COL_ALKALEARTH,{2, 2, 6, 2, 6, 2,10, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Alkaline earth"},
    { 39, "Y",  "Itrio",          88.906f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"},
    { 40, "Zr", "Circonio",       91.224f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"},
    { 41, "Nb", "Niobio",         92.906f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion
    { 42, "Mo", "Molibdeno",      95.950f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion
    { 43, "Tc", "Tecnecio",       98.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"},
    { 44, "Ru", "Rutenio",       101.070f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion
    { 45, "Rh", "Rodio",         102.906f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion
    { 46, "Pd", "Paladio",       106.420f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion (no 5s!)
    { 47, "Ag", "Plata",         107.868f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"}, // excepcion
    { 48, "Cd", "Cadmio",        112.411f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Transition metal"},
    { 49, "In", "Indio",         114.818f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Post-transition"},
    { 50, "Sn", "Estanno",       118.710f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 2, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Post-transition"},
    { 51, "Sb", "Antimonio",     121.760f,  COL_METALLOID, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 3, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Metalloid"},
    { 52, "Te", "Telurio",       127.600f,  COL_METALLOID, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 4, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Metalloid"},
    { 53, "I",  "Yodo",          126.904f,  COL_HALOGEN,   {2, 2, 6, 2, 6, 2,10, 6, 2,10, 5, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Halogen"},
    { 54, "Xe", "Xenon",         131.293f,  COL_NOBLEGAS,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 0, 0, 0, 0, 0, 0, 0, 0}, 5, "Noble gas"},
    { 55, "Cs", "Cesio",         132.905f,  COL_ALKALI,    {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 1, 0, 0, 0, 0, 0, 0, 0}, 6, "Alkali metal"},
    { 56, "Ba", "Bario",         137.327f,  COL_ALKALEARTH,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 0, 0, 0, 0, 0, 0, 0}, 6, "Alkaline earth"},
    { 57, "La", "Lantano",       138.905f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 0, 1, 0, 0, 0, 0, 0}, 6, "Lanthanide"}, // excepcion (5d1, no 4f)
    { 58, "Ce", "Cerio",         140.116f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 1, 1, 0, 0, 0, 0, 0}, 6, "Lanthanide"}, // excepcion
    { 59, "Pr", "Praseodimio",   140.908f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 3, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 60, "Nd", "Neodimio",      144.242f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 4, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 61, "Pm", "Prometio",      145.000f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 5, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 62, "Sm", "Samario",       150.360f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 6, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 63, "Eu", "Europio",       151.964f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 7, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 64, "Gd", "Gadolinio",     157.250f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 7, 1, 0, 0, 0, 0, 0}, 6, "Lanthanide"}, // excepcion
    { 65, "Tb", "Terbio",        158.925f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2, 9, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 66, "Dy", "Disprosio",     162.500f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,10, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 67, "Ho", "Holmio",        164.930f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,11, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 68, "Er", "Erbio",         167.259f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,12, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 69, "Tm", "Tulio",         168.934f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,13, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 70, "Yb", "Iterbio",       173.045f,  COL_LANTHANIDE,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 0, 0, 0, 0, 0, 0}, 6, "Lanthanide"},
    { 71, "Lu", "Lutecio",       174.967f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 1, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 72, "Hf", "Hafnio",        178.490f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 2, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 73, "Ta", "Tantalo",       180.948f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 3, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 74, "W",  "Wolframio",     183.840f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 4, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 75, "Re", "Renio",         186.207f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 5, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 76, "Os", "Osmio",         190.230f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 6, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 77, "Ir", "Iridio",        192.217f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14, 7, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 78, "Pt", "Platino",       195.084f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 1,14, 9, 0, 0, 0, 0, 0}, 6, "Transition metal"}, // excepcion
    { 79, "Au", "Oro",           196.967f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 1,14,10, 0, 0, 0, 0, 0}, 6, "Transition metal"}, // excepcion
    { 80, "Hg", "Mercurio",      200.592f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 0, 0, 0, 0, 0}, 6, "Transition metal"},
    { 81, "Tl", "Talio",         204.383f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 1, 0, 0, 0, 0}, 6, "Post-transition"},
    { 82, "Pb", "Plomo",         207.200f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 2, 0, 0, 0, 0}, 6, "Post-transition"},
    { 83, "Bi", "Bismuto",       208.980f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 3, 0, 0, 0, 0}, 6, "Post-transition"},
    { 84, "Po", "Polonio",       209.000f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 4, 0, 0, 0, 0}, 6, "Post-transition"},
    { 85, "At", "Astato",        210.000f,  COL_HALOGEN,   {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 5, 0, 0, 0, 0}, 6, "Halogen"},
    { 86, "Rn", "Radon",         222.000f,  COL_NOBLEGAS,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 0, 0, 0, 0}, 6, "Noble gas"},
    { 87, "Fr", "Francio",       223.000f,  COL_ALKALI,    {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 1, 0, 0, 0}, 7, "Alkali metal"},
    { 88, "Ra", "Radio",         226.000f,  COL_ALKALEARTH,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 0, 0, 0}, 7, "Alkaline earth"},
    { 89, "Ac", "Actinio",       227.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 0, 1, 0}, 7, "Actinide"}, // excepcion
    { 90, "Th", "Torio",         232.038f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 0, 2, 0}, 7, "Actinide"}, // excepcion
    { 91, "Pa", "Protactinio",   231.036f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 2, 1, 0}, 7, "Actinide"}, // excepcion
    { 92, "U",  "Uranio",        238.029f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 3, 1, 0}, 7, "Actinide"}, // excepcion
    { 93, "Np", "Neptunio",      237.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 4, 1, 0}, 7, "Actinide"}, // excepcion
    { 94, "Pu", "Plutonio",      244.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 6, 0, 0}, 7, "Actinide"},
    { 95, "Am", "Americio",      243.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 7, 0, 0}, 7, "Actinide"},
    { 96, "Cm", "Curio",         247.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 7, 1, 0}, 7, "Actinide"}, // excepcion
    { 97, "Bk", "Berkelio",      247.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2, 9, 0, 0}, 7, "Actinide"},
    { 98, "Cf", "Californio",    251.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,10, 0, 0}, 7, "Actinide"},
    { 99, "Es", "Einsteinio",    252.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,11, 0, 0}, 7, "Actinide"},
    {100, "Fm", "Fermio",        257.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,12, 0, 0}, 7, "Actinide"},
    {101, "Md", "Mendelevio",    258.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,13, 0, 0}, 7, "Actinide"},
    {102, "No", "Nobelio",       259.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 0, 0}, 7, "Actinide"},
    {103, "Lr", "Lawrencio",     266.000f,  COL_ACTINIDE,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 0, 1}, 7, "Actinide"},
    {104, "Rf", "Rutherfordio",  267.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 2, 0}, 7, "Transition metal"},
    {105, "Db", "Dubnio",        268.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 3, 0}, 7, "Transition metal"},
    {106, "Sg", "Seaborgio",     269.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 4, 0}, 7, "Transition metal"},
    {107, "Bh", "Bohrio",        270.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 5, 0}, 7, "Transition metal"},
    {108, "Hs", "Hassio",        269.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 6, 0}, 7, "Transition metal"},
    {109, "Mt", "Meitnerio",     278.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 7, 0}, 7, "Transition metal"},
    {110, "Ds", "Darmstadtio",   281.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 8, 0}, 7, "Transition metal"},
    {111, "Rg", "Roentgenio",    282.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14, 9, 0}, 7, "Transition metal"},
    {112, "Cn", "Copernicio",    285.000f,  COL_TRANSITION,{2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 0}, 7, "Transition metal"},
    {113, "Nh", "Nihonio",       286.000f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 1}, 7, "Post-transition"},
    {114, "Fl", "Flerovio",      289.000f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 2}, 7, "Post-transition"},
    {115, "Mc", "Moscovio",      290.000f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 3}, 7, "Post-transition"},
    {116, "Lv", "Livermorio",    293.000f,  COL_POSTTRANS, {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 4}, 7, "Post-transition"},
    {117, "Ts", "Teneso",        294.000f,  COL_HALOGEN,   {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 5}, 7, "Halogen"},
    {118, "Og", "Oganesson",     294.000f,  COL_NOBLEGAS,  {2, 2, 6, 2, 6, 2,10, 6, 2,10, 6, 2,14,10, 6, 2,14,10, 6}, 7, "Noble gas"},
};
