#!/bin/bash

./misc/update_file_headers.py include/LibCarna/base/*.h
./misc/update_file_headers.py include/LibCarna/base/math/*.h
./misc/update_file_headers.py include/LibCarna/helpers/*.h
./misc/update_file_headers.py include/LibCarna/presets/*.h
./misc/update_file_headers.py include/LibCarna/LibCarna.h

./misc/update_file_headers.py misc/Version.h.in

./misc/update_file_headers.py src/base/*.cpp
./misc/update_file_headers.py src/base/math/*.cpp
./misc/update_file_headers.py src/helpers/*.cpp
./misc/update_file_headers.py src/presets/*.cpp
./misc/update_file_headers.py src/res/*.vert
./misc/update_file_headers.py src/res/*.frag
./misc/update_file_headers.py src/res/*.geom

./misc/update_file_headers.py test/Demo/src/main.cpp
./misc/update_file_headers.py test/IntegrationTests/*.cpp
./misc/update_file_headers.py test/IntegrationTests/*.h
./misc/update_file_headers.py test/ModuleTests/*.cpp
./misc/update_file_headers.py test/ModuleTests/*.h
./misc/update_file_headers.py test/Tools/*.cpp
./misc/update_file_headers.py test/Tools/*.h
./misc/update_file_headers.py test/UnitTests/*.cpp
./misc/update_file_headers.py test/UnitTests/*.h