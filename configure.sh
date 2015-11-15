#!/bin/sh
reset
#rm CMakeCache.txt
/opt/cmake-3.0.2/bin/cmake .. \
-DCMAKE_C_COMPILER=gcc \
-DCMAKE_CXX_COMPILER=g++ \
-DCMAKE_Fortran_COMPILER=gfortran \
-DCMAKE_BUILD_TYPE=Release \
-DBOOST_ROOT="/home/hbui/kratos_rub/external_libraries/boost_1_55_0" \
-DFOX_DIR="/home/hbui/opt/fox-1.6.50" \
-DVTK_DIR="/home/hbui/opt/vtk-6.2.0/lib/cmake/vtk-6.2" \

make install -j4

