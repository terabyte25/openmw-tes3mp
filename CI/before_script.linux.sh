#!/bin/sh

free -m
mkdir build
cd build

# Set up compilers
if [ ! -z "${MATRIX_CC}" ]; then
    eval "${MATRIX_CC}"
fi

export RAKNET_ROOT=~/CrabNet

export CODE_COVERAGE=0
if [ ! -z "${ANALYZE}" ]; then
    CODE_COVERAGE=1
fi

${ANALYZE}cmake .. \
    -DDESIRED_QT_VERSION=5 \
    -DBUILD_OPENMW_MP=ON \
    -DBUILD_BROWSER=ON \
    -DBUILD_MASTER=ON \
    -DBUILD_WITH_CODE_COVERAGE=${CODE_COVERAGE} \
    -DBUILD_BSATOOL=OFF \
    -DBUILD_ESMTOOL=OFF \
    -DBUILD_ESSIMPORTER=OFF \
    -DBUILD_LAUNCHER=OFF \
    -DBUILD_MWINIIMPORTER=OFF \
    -DBUILD_MYGUI_PLUGIN=OFF \
    -DBUILD_OPENCS=OFF \
    -DBUILD_WIZARD=OFF \
    -DBUILD_UNITTESTS=1 \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DBINDIR=/usr/games \
    -DCMAKE_BUILD_TYPE="None" \
    -DUSE_SYSTEM_TINYXML=TRUE \
    -DRakNet_LIBRARY_RELEASE=~/CrabNet/lib/libRakNetLibStatic.a \
    -DRakNet_LIBRARY_DEBUG=~/CrabNet/lib/libRakNetLibStatic.a
