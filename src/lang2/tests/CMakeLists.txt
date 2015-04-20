FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang2 main.cpp ${srcs})
TARGET_LINK_YOCTO(test-lang2 lang2)
 
