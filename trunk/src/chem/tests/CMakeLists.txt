FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chem main.cpp ${test_srcs}
sample.lua
acetic.lua
oxalic.lua
ammonium.lua
mix.lua
boot-ab.lua
ode.lua
)
TARGET_LINK_YOCTO( test-chem chem mk lua++ lua)

ENABLE_TESTING()

ADD_TEST(Species     test-chem species)
ADD_TEST(Library     test-chem lib      ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Equilibria  test-chem eqs      ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(Boot1       test-chem boot     ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua ini)
ADD_TEST(Boot2       test-chem boot     ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua ini2)
ADD_TEST(BootAB      test-chem boot     ${CMAKE_CURRENT_SOURCE_DIR}/boot-ab.lua)
ADD_TEST(Acetic      test-chem dosage   ${CMAKE_CURRENT_SOURCE_DIR}/acetic.lua)
ADD_TEST(Oxalic      test-chem dosage   ${CMAKE_CURRENT_SOURCE_DIR}/oxalic.lua)
ADD_TEST(Ammonium    test-chem dosage   ${CMAKE_CURRENT_SOURCE_DIR}/ammonium.lua)
ADD_TEST(Mix         test-chem mix      ${CMAKE_CURRENT_SOURCE_DIR}/mix.lua)
ADD_TEST(Effectors   test-chem eff      ${CMAKE_CURRENT_SOURCE_DIR}/sample.lua)
ADD_TEST(ODE         test-chem ode      ${CMAKE_CURRENT_SOURCE_DIR}/ode.lua)

