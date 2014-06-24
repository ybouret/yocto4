FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chem main.cpp ${test_srcs} 
)
TARGET_LINK_YOCTO( test-chem chem mk)

ENABLE_TESTING()

ADD_TEST(Species    test-chem species)
ADD_TEST(Collection test-chem collection)
ADD_TEST(Solution   test-chem solution)
