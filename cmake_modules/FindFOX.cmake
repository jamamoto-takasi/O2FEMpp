# Locate gdal
# This module defines
# FOX_LIBRARY
# FOX_FOUND, if false, do not try to link to gdal 
# FOX_INCLUDE_DIR, where to find the headers
#
# $FOX_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FOX_DIR
#
# Created by Robert Osfield. 

FIND_PATH(FOX_INCLUDE_DIR fx.h
    ${FOX_DIR}/include/fox-1.6
    ${FOX_DIR}/fox-1.6
)

MACRO(FIND_FOX_LIBRARY MYLIBRARY MYLIBRARYNAME)

    FIND_LIBRARY(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        ${FOX_DIR}/lib
        ${FOX_DIR}
    )

ENDMACRO(FIND_FOX_LIBRARY LIBRARY LIBRARYNAME)

FIND_FOX_LIBRARY(FOX_LIBRARY FOX-1.6)

SET(FOX_FOUND "NO")
IF(FOX_LIBRARY AND FOX_INCLUDE_DIR)
    SET(FOX_FOUND "YES")
ENDIF(FOX_LIBRARY AND FOX_INCLUDE_DIR)

