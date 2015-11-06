FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chemical main.cpp ${test_srcs})

TARGET_LINK_YOCTO( test-chemical chemical mk lua++ lua)

