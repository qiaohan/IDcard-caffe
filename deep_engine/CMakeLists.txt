cmake_minimum_required(VERSION 2.8)
project( testOCR )

file( GLOB SRCS "./src/*.cpp" )
file( GLOB TEST "./test/*.cpp" )

SET( OPENCV_Path "/home/hzqiaohan/.local" ) 
SET( OPENCV_INC "${OPENCV_Path}/include" "${OPENCV_Path}/include/opencv" "${OPENCV_Path}/include/opencv2" ) 
SET( OPENCV_LIB "${OPENCV_Path}/lib" ) 

SET( CAFFE_PATH "/home/hzqiaohan/caffe/caffe-master")
SET( CAFFE_INC "${CAFFE_PATH}/include")
SET( CAFFE_LIB "${CAFFE_PATH}/build/lib")

INCLUDE_DIRECTORIES( ${OPENCV_INC} ${CAFFE_INC} "./include" )
LINK_DIRECTORIES( ${OPENCV_LIB} ${CAFFE_LIB} "/usr/local/cuda/lib64")

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -D__STDC_CONSTANT_MACROS" )

add_executable( testOCR ${SRCS} ${TEST})
add_library( CharEngineCaffe SHARED ${SRCS}  )
target_link_libraries( CharEngineCaffe -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system )
target_link_libraries( testOCR -lglog -lcaffe -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system )
