FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chem main.cpp ${test_srcs} 
sample.lua
sample2.lua
boot-spec.lua
)
TARGET_LINK_YOCTO( test-chem chem mk lua++ lua)

ENABLE_TESTING()

ADD_TEST(Species     test-chem species)
ADD_TEST(Collection  test-chem collection ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Solution    test-chem solution)
ADD_TEST(Equilibrium test-chem equilibrium)
ADD_TEST(Equilibria  test-chem equilibria)
