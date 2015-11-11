CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

PROJECT(oofem-cpp)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

FIND_PACKAGE(Boost COMPONENTS python REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

FIND_PACKAGE(FOX REQUIRED)
INCLUDE_DIRECTORIES(${FOX_INCLUDE_DIR})
ADD_DEFINITIONS(-DHAVE_GLU_H=1 -DHAVE_GL_H=1)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
SET(VTK_LIBRARIES vtkRendering vtkHybrid)

###########################################################

SET( FOX_GL_SOURCES     
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/fx_gl_shapes.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/gl_view_window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/vtk_view_window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/vtkfox/FXVTKCanvas.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/vtkfox/unix.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/viewer/vtkfox/vtkFXRenderWindowInteractor.cpp
)
ADD_LIBRARY(FoxGL SHARED ${FOX_GL_SOURCES})

###########################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/solver/SuperLU_4.3)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/solver/SuperLU_4.3)

###########################################################

SET(OOFEM_SOURCES
    dof.cpp
    node.cpp
    truss_element.cpp
    point_force_3d.cpp
    structure.cpp
    python_interface.cpp
#    visualizer.cpp
    solver/superlu_solver.cpp
)
ADD_LIBRARY(OOFEM SHARED ${OOFEM_SOURCES})
TARGET_LINK_LIBRARIES(OOFEM ${Boost_PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(OOFEM ${PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(OOFEM superlu)
SET_TARGET_PROPERTIES(OOFEM PROPERTIES PREFIX "")

###########################################################

# ADD_SUBDIRECTORY(examples)
ADD_SUBDIRECTORY(tests)

INSTALL(TARGETS OOFEM FoxGL DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/libs)

