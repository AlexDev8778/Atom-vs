# Atom-vs — Simulador Atómico

Simulador educativo 3D de átomos con nubes de probabilidad cuántica para los 118 elementos de la tabla periódica.

## Features

- **118 elementos** con configuración electrónica real por subniveles s/p/d/f (incluyendo excepciones: Cr, Cu, Pd, Au, etc.)
- **Nube de probabilidad** generada con Monte Carlo — cada tipo de orbital (s, p, d, f) tiene su distribución angular correcta
- **Animación de electrones**: todos los puntos de la nube oscilan alrededor de su posición base (proceso de Ornstein-Uhlenbeck — fuerza restauradora + jitter estocástico). Los electrones internos (1s, 2s) se mueven más rápido que los externos (6p, 7s), respetando las diferencias de energía
- **Vibración nuclear**: el núcleo oscila sinusoidalmente en una dirección aleatoria fija. Amplitud ∝ 1/√A (los átomos pesados vibran menos)
- **Núcleo realista**: protones (rojo) y neutrones (azul) empaquetados dentro de `R = 1.2 × A^(1/3)` femtómetros
- **Zoom continuo** con escala logarítmica — de la nube electrónica (~Å) al núcleo (~fm), 100.000x de diferencia
- **Rotación libre** por arrastre del mouse (ejes X e Y)
- Panel selector colapsable con hover-expand
- Panel de info toggleable con configuración electrónica por subnivel

## Stack

- **C++17** + **Raylib 4.5** (compilado con `GRAPHICS_API_OPENGL_21` para compatibilidad con GPUs antiguas)
- **Compilador:** g++ (MinGW-w64 / w64devkit)
- Renderizado 2D con proyección 3D propia (sin OpenGL 3D directo)

## Setup

### Requisitos

- [w64devkit](https://github.com/skeeto/w64devkit) o MinGW-w64 en el PATH
- Windows (probado en Windows 10/11)

### Compilar Raylib desde fuente (solo la primera vez)

El proyecto requiere Raylib compilado con OpenGL 2.1 para compatibilidad máxima:

```powershell
powershell -ExecutionPolicy Bypass -File build_raylib_ogl21.ps1
```

Esto descarga Raylib 4.5, lo compila con `GRAPHICS_API_OPENGL_21` y lo coloca en `libs/raylib/`.

### Compilar y ejecutar

```bat
build.bat
```

## Controles

| Acción | Control |
|--------|---------|
| Seleccionar elemento | Click en el strip izquierdo |
| Zoom in/out | Rueda del mouse |
| Rotar átomo | Click + arrastrar |
| Toggle info | Botón `i` (derecha) |
| Toggle animación | `Space` o click en `[ ANIM: ON/OFF ]` (centro inferior) |
| Navegar elementos | ↑ / ↓ |

## Escala física

| Nivel de zoom | Lo que se ve |
|---|---|
| x1 (default) | Nube electrónica completa |
| ~x1.000 | Nube y núcleo simultáneos |
| ~x50.000 | Nucleones individuales visibles |
| ~x100.000+ | Solo el núcleo (la nube está 100.000 radios afuera) |
