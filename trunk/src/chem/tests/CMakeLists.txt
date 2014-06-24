FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chem main.cpp ${test_srcs} 
)
TARGET_LINK_YOCTO( test-chem chem mk)

ENABLE_TESTING()

ADD_TEST(Species test-chem species)
