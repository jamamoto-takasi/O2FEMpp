SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )

SET(file_list
    SmallTetraeder
)

foreach(str ${file_list})
    ADD_EXECUTABLE(${str} ${str}.cpp)
    TARGET_LINK_LIBRARIES(${str} OOFEM)
    TARGET_LINK_LIBRARIES(${str} superlu)
    TARGET_LINK_LIBRARIES(${str} FoxGL)
    TARGET_LINK_LIBRARIES(${str} ${Boost_LIBRARIES})
    TARGET_LINK_LIBRARIES(${str} ${FOX_LIBRARY})
    TARGET_LINK_LIBRARIES(${str} ${VTK_LIBRARIES})
endforeach()

