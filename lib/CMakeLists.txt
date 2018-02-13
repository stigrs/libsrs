cmake_minimum_required(VERSION 3.4.3)
project(libsrs_lib CXX)

include_directories(${libsrs_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GSL_INCLUDE_DIR})
include_directories(${ARMADILLO_INCLUDE_DIRS})
link_directories(${libsrs_BINARY_DIR})

set(
    SRC_FILES
    datum.cpp
    input.cpp
    math_geometry.cpp
    math_integration.cpp
    math_statistics.cpp
)
add_library(
    srs 
    ${SRC_FILES}
)
if(WIN32)
    target_link_libraries(
        srs 
        mkl_intel_c.lib
        mkl_sequential.lib
        mkl_core.lib
    )
else()
    target_link_libraries(
        srs 
        ${ARMADILLO_LIBRARIES}
    ) 
endif()

install(
    TARGETS srs
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    CONFIGURATIONS Release
)
