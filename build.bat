@echo off
setlocal

set SRC=src\main.cpp src\physics\quantum.cpp src\renderer\atom_renderer.cpp src\renderer\ui_renderer.cpp
set OUT=atom-vs.exe
set INC=-Ilibs\raylib\include
set LIB=-Llibs\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm

echo Compilando Atom-vs...
g++ -std=c++17 -O2 %SRC% -o %OUT% %INC% %LIB%

if %ERRORLEVEL% == 0 (
    echo.
    echo Compilacion exitosa: %OUT%
    echo Ejecutando...
    echo.
    %OUT%
) else (
    echo.
    echo ERROR: Fallo la compilacion.
    pause
)
