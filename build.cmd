@echo off

IF "%1" == "" (
  cmake --list-presets
  exit /b 1
)

cmake --preset %1 || exit /b 1
cmake --build build/%1 --parallel || exit /b 1
