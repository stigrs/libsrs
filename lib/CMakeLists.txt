cmake_minimum_required(VERSION 3.4.3)
project(libsrs_lib CXX)

include_directories(${libsrs_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GSL_INCLUDE_DIR})
link_directories(${libsrs_BINARY_DIR})
include_directories(${MKL_INCLUDE_DIR})
link_directories(${MKL_LIB_DIR})
if(APPLE)
    link_directories(${TBB_LIB_DIR})
endif()

set(
    SRC_FILES
    annealfunc.cpp
    datum.cpp
	euler.cpp
    geometry.cpp
    grid.cpp
    input.cpp
    integration.cpp
    linalg.cpp
	quaternion.cpp
    statistics.cpp
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
elseif(APPLE)
    target_link_libraries (
        ${program} 
        srs
        mkl_intel_lp64
        mkl_tbb_thread
        mkl_core
        tbb
        stdc++
        pthread
        m 
        ldl 
        test_catch
    )
else()
    target_link_libraries(
        srs 
        mkl_intel_lp64
        mkl_gnu_thread
        mkl_core
        gomp
        pthread
        m 
        dl 
    ) 
endif()

install(
    TARGETS srs
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
