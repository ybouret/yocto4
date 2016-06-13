
INCLUDE_DIRECTORIES(../../../main
../libtiff
${CMAKE_CURRENT_BINARY_DIR}/../libtiff
${TIFF_INCLUDES})

ADD_EXECUTABLE(test-tiff main.cpp)
TARGET_LINK_YOCTO(test-tiff tiff)
