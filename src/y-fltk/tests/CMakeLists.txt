
FILE(GLOB test_srcs test-*.cpp)
FLUID_UIC(matte)
ADD_EXECUTABLE(test-fltk main.cpp ${test_srcs} ${matte_SOURCES})
TARGET_LINK_YOCTO( test-fltk fltk)
TARGET_LINK_FLTK( test-fltk )

