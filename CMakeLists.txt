cmake_minimum_required(VERSION 3.10)
project( stereo )
add_compile_options( -O3 )
set( CMAKE_BUILD_TYPE "Release" )
message( ${CMAKE_BUILD_TYPE} )

#Include OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#Include ZED lib
option(LINK_SHARED_ZED "Link with the ZED SDK shared executable" ON)
find_package( ZED 3 REQUIRED )
find_package( ${ZED_CUDA_VERSION} )

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})

link_directories(${ZED_LIBRARY_DIR})
link_directories(${CUDA_LIBRARY_DIRS})

#src
set( Gradient Gradient.cpp Gradient.hpp )
set( MatHelper MatHelper.hpp )
set( Includes OpencvIncludes.hpp ZEDIncludes.hpp )
set( Cost Cost.hpp Cost.cpp )

add_executable( stereo main.cpp ${Gradient} ${MatHelper} ${Cost} )

#LINK AGAINST OPENCV lib
target_link_libraries( stereo ${OpenCV_LIBS})

#LINK AGAINST ZED lib
if (LINK_SHARED_ZED)
    SET(ZED_LIBS ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})
else()
    SET(ZED_LIBS ${ZED_STATIC_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_LIBRARY})
endif()

TARGET_LINK_LIBRARIES( stereo ${ZED_LIBS})
