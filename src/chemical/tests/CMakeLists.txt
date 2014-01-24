INCLUDE_DIRECTORIES(../../spade)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chemical main.cpp ${test_srcs} 
example.lua 
acetic.lua 
oxalic.lua 
diff.lua 
boot.lua 
boot-no-dof.lua
boot-ab.lua
boot-spec.lua
)
TARGET_LINK_YOCTO( test-chemical chemical mk lua++ lua)

ENABLE_TESTING()
ADD_TEST(Species     test-chemical species)
ADD_TEST(Collection  test-chemical collection)
ADD_TEST(Equilibrium test-chemical equilibrium)
ADD_TEST(Equilibria  test-chemical equilibria)
ADD_TEST(Lua         test-chemical lua    ${CMAKE_CURRENT_SOURCE_DIR}/example.lua)
ADD_TEST(Acetic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/acetic.lua)
ADD_TEST(Oxalic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/oxalic.lua)
ADD_TEST(DiffEq      test-chemical diff   ${CMAKE_CURRENT_SOURCE_DIR}/diff.lua)
ADD_TEST(Boot        test-chemical boot   ${CMAKE_CURRENT_SOURCE_DIR}/example.lua)
ADD_TEST(Boot1       test-chemical boot1)
ADD_TEST(Boot1.0     test-chemical boot1 0)
ADD_TEST(Boot2       test-chemical boot2)
ADD_TEST(Boot2.0     test-chemical boot2 0)
ADD_TEST(Mix         test-chemical mix)


