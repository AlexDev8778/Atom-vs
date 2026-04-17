# Script para compilar Raylib con GRAPHICS_API_OPENGL_21
# Necesario para GPUs antiguas (Intel HD 2000/3000) sin WGL_ARB_create_context_profile
Write-Host "=== Compilando Raylib 4.5 con OpenGL 2.1 ===" -ForegroundColor Cyan

$RAYLIB_VERSION = "4.5.0"
$DOWNLOAD_URL   = "https://github.com/raysan5/raylib/archive/refs/tags/$RAYLIB_VERSION.zip"
$ZIP_FILE       = "raylib-src.zip"
$EXTRACT_DIR    = "raylib-$RAYLIB_VERSION"
$SRC_DIR        = "$EXTRACT_DIR\src"

# 1. Descargar fuente
if (-not (Test-Path $ZIP_FILE)) {
    Write-Host "Descargando raylib $RAYLIB_VERSION..." -ForegroundColor Yellow
    Invoke-WebRequest -Uri $DOWNLOAD_URL -OutFile $ZIP_FILE -UseBasicParsing
} else {
    Write-Host "Fuente ya descargada." -ForegroundColor Green
}

# 2. Extraer
if (-not (Test-Path $EXTRACT_DIR)) {
    Write-Host "Extrayendo..." -ForegroundColor Yellow
    Expand-Archive -Path $ZIP_FILE -DestinationPath "." -Force
} else {
    Write-Host "Ya extraido." -ForegroundColor Green
}

# 3. Compilar con make (Makefile oficial incluye GLFW correctamente)
Write-Host "Compilando con make GRAPHICS_API_OPENGL_21 (puede tardar 2-3 min)..." -ForegroundColor Yellow
Push-Location $SRC_DIR

# Limpiar build anterior si existe
& make clean 2>&1 | Out-Null

# Compilar con OpenGL 2.1 para Windows Desktop
& make PLATFORM=PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_21 CC=gcc 2>&1

$buildOk = ($LASTEXITCODE -eq 0)
Pop-Location

if (-not $buildOk) {
    Write-Host "ERROR: make fallo" -ForegroundColor Red
    exit 1
}

# 4. Copiar a libs/raylib/
Write-Host "Copiando archivos..." -ForegroundColor Yellow
Copy-Item "$SRC_DIR\libraylib.a"  "libs\raylib\lib\libraylib.a"  -Force
Copy-Item "$SRC_DIR\raylib.h"     "libs\raylib\include\raylib.h" -Force
Copy-Item "$SRC_DIR\raymath.h"    "libs\raylib\include\raymath.h" -Force
Copy-Item "$SRC_DIR\rlgl.h"       "libs\raylib\include\rlgl.h"   -Force

Write-Host ""
Write-Host "=== Raylib OGL2.1 compilado exitosamente ===" -ForegroundColor Green
Write-Host "Ahora corri build.bat" -ForegroundColor Cyan
