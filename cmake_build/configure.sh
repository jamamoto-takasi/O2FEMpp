#!/bin/sh
reset
rm CMakeCache.txt
cmake .. \
-DCMAKE_C_COMPILER=gcc \
-DCMAKE_CXX_COMPILER=g++ \
-DCMAKE_Fortran_COMPILER=gfortran \
-DCMAKE_BUILD_TYPE=Release \
-DBOOST_ROOT="/home/hbui/kratos_rub/external_libraries/boost_1_55_0" \
-DFOX_DIR="/opt/fox-1.6.50" \

make install -j4
