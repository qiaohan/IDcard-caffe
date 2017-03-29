cmake_minimum_required(VERSION 2.8)
project( IDcardRecognize )

file( GLOB LIB_SRCS "deskew/src/*.cpp" "detect/src/*.cpp" "deep_engine/src/*.cpp" "textline/src/*.cpp" "idcardrec/src/*.cpp" )
file( GLOB TEST_SRCS ${LIB_SRCS} "./test/*.cpp" )
file( GLOB JNI_SRCS  ${LIB_SRCS} "./jni/*.cpp" )

SET( OPENCV_Path "/home/hzqiaohan/.local" ) 
SET( OPENCV_INCLUDE "${OPENCV_Path}/include" "${OPENCV_Path}/include/opencv" "${OPENCV_Path}/include/opencv2" ) 
SET( OPENCV_LIB "${OPENCV_Path}/lib" ) 

SET( CAFFE_PATH "/home/hzqiaohan/caffe/caffe-master")
SET( CAFFE_INC "${CAFFE_PATH}/include")
SET( CAFFE_LIB "${CAFFE_PATH}/build/lib")

INCLUDE_DIRECTORIES(${CAFFE_INC} ${OPENCV_INCLUDE} "detect/include" "deskew/include" "deep_engine/include" "textline/include" "idcardrec/include" )
LINK_DIRECTORIES( ${CAFFE_LIB} ${OPENCV_LIB} "/usr/local/cuda/lib64" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -D__STDC_CONSTANT_MACROS" )

add_executable( IDrec ${TEST_SRCS} )
#target_link_libraries( IDrec -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system )
target_link_libraries( IDrec -lglog -lcaffe -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy -lboost_filesystem -lboost_system)
#add_library( deskew SHARED ${LIB_SRCS}  )
#add_library( deskewjni SHARED ${JNI_SRCS}  )
