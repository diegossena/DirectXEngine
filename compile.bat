CLS
@ECHO OFF
g++ src\main.cpp -o bin\engine -Isrc -lD3D11 -ld3dcompiler
cd bin
engine
cd ..