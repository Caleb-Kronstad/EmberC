@echo off

echo === Setting up ===
dependencies\premake\Windows\premake5.exe --file=build.lua vs2022

echo === Setup complete ===

pause