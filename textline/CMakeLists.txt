cmake_minimum_required(VERSION 2.8)
project( testOCR )

file( GLOB SRCS "./src/*.cpp" "./src/*.c" )
file( GLOB TEST "./test/*.cpp" )

SET( OPENCV_Path "/home/hzqiaohan/.local" ) 
SET( OPENCV_INCLUDE "${OPENCV_Path}/include" "${OPENCV_Path}/include/opencv" "${OPENCV_Path}/include/opencv2" ) 
SET( OPENCV_LIB "${OPENCV_Path}/lib" ) 

SET( OCR_PATH "/home/hzqiaohan/OCR/danzi_deep/deep_engine")
SET( OCR_INC "${OCR_PATH}/include")
SET( OCR_LIB "${OCR_PATH}/lib")

SET( CAFFE_PATH "/home/hzqiaohan/caffe/caffe-master")
SET( CAFFE_INC "${CAFFE_PATH}/include")
SET( CAFFE_LIB "${CAFFE_PATH}/build/lib")

INCLUDE_DIRECTORIES(${CAFFE_INC} ${OCR_INC} ${OPENCV_INCLUDE} "./include" )
LINK_DIRECTORIES( ${CAFFE_LIB} ${OCR_LIB} ${OPENCV_LIB} "/usr/local/cuda/lib64" )

#INCLUDE_DIRECTORIES( "/home/hzqiaohan/OCR/danzi/include" )
#LINK_DIRECTORIES( "/home/hzqiaohan/OCR/danzi/lib" )


SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -D__STDC_CONSTANT_MACROS" )

add_executable( testtextline ${SRCS} ${TEST})
#add_library(readtextline  SHARED ${SRCS}  )
target_link_libraries( testtextline -lglog -lcaffe -lprotobuf -lCharEngineCaffe -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system)
