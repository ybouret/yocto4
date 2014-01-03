INCLUDE_DIRECTORIES(../../spade)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chemical main.cpp ${test_srcs} example.lua acetic.lua oxalic.lua diff.lua)
TARGET_LINK_YOCTO( test-chemical chemical mk lua++ lua)

ENABLE_TESTING()
ADD_TEST(Species     test-chemical species)
ADD_TEST(Collection  test-chemical collection)
ADD_TEST(Equilibrium test-chemical equilibrium)
ADD_TEST(Equilibria  test-chemical equilibria)
ADD_TEST(Init        test-chemical init)
ADD_TEST(Init0       test-chemical init 0)
ADD_TEST(Init2       test-chemical init2 )
ADD_TEST(Init2.0     test-chemical init2 0)
ADD_TEST(Lua         test-chemical lua ${CMAKE_CURRENT_SOURCE_DIR}/example.lua)
ADD_TEST(Acetic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/acetic.lua)
ADD_TEST(Oxalic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/oxalic.lua)

