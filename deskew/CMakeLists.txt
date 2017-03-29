cmake_minimum_required(VERSION 2.8)
project( IDpre )

file( GLOB TEST_SRCS "./src/*.cpp" "./test/*.cpp" )
file( GLOB LIB_SRCS "./src/*.cpp" )
file( GLOB JNI_SRCS "./src/*.cpp" "./jni/*.cpp" )

SET( OPENCV_Path "/home/hzqiaohan/.local" ) 
SET( OPENCV_INCLUDE "${OPENCV_Path}/include" "${OPENCV_Path}/include/opencv" "${OPENCV_Path}/include/opencv2" ) 
SET( OPENCV_LIB "${OPENCV_Path}/lib" ) 


INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE} "./include" )
LINK_DIRECTORIES( ${OPENCV_LIB} )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -D__STDC_CONSTANT_MACROS" )

add_executable( IDpre ${TEST_SRCS} )
target_link_libraries( IDpre -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system )
#add_library( deskew SHARED ${LIB_SRCS}  )
add_library( deskewjni SHARED ${JNI_SRCS}  )
