#!/bin/bash

echo "=== Setting up ==="
dependencies/premake/Linux/premake5 --file=build.lua gmake2

echo "=== Setup complete ==="
