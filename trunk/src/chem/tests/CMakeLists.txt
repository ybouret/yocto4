FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chem main.cpp ${test_srcs}
sample.lua
acetic.lua
oxalic.lua
)
TARGET_LINK_YOCTO( test-chem chem mk lua++ lua)

ENABLE_TESTING()

ADD_TEST(Species     test-chemical species)
ADD_TEST(Library     test-chemical lib ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Equilibria  test-chemical eqs ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Acetic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/acetic.lua)
ADD_TEST(Oxalic      test-chemical dosage ${CMAKE_CURRENT_SOURCE_DIR}/oxalic.lua)

#ADD_TEST(Mix         test-chemical mix ${CMAKE_CURRENT_SOURCE_DIR}/mix.lua)
#ADD_TEST(Effectors   test-chemical effector ${CMAKE_CURRENT_SOURCE_DIR}/eff.lua)
#ADD_TEST(ODE         test-chemical ode ${CMAKE_CURRENT_SOURCE_DIR}/ode.lua)

