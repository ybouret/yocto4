FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chemical main.cpp ${test_srcs} 
sample.lua
sample2.lua
boot-spec.lua
co2.lua
mix.lua
eff.lua
ode.lua
fluo.lua
)
TARGET_LINK_YOCTO( test-chemical chemical mk lua++ lua)

ENABLE_TESTING()

ADD_TEST(Species     test-chemical species)
ADD_TEST(Collection  test-chemical collection ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Solution    test-chemical solution)
ADD_TEST(Equilibrium test-chemical equilibrium)
ADD_TEST(Equilibria  test-chemical equilibria)
ADD_TEST(Mix         test-chemical mix ${CMAKE_CURRENT_SOURCE_DIR}/mix.lua)
ADD_TEST(Effectors   test-chemical effector ${CMAKE_CURRENT_SOURCE_DIR}/eff.lua)
ADD_TEST(ODE         test-chemical ode ${CMAKE_CURRENT_SOURCE_DIR}/ode.lua)

