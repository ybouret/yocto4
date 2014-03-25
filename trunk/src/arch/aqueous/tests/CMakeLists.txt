INCLUDE_DIRECTORIES(../../spade)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-aqueous main.cpp ${test_srcs} config.lua)
TARGET_LINK_YOCTO( test-aqueous aqueous mk lua++ lua)

ENABLE_TESTING()
ADD_TEST(Species     test-aqueous species)
ADD_TEST(Library     test-aqueous lib)
ADD_TEST(ChemSys     test-aqueous chemsys)
ADD_TEST(Init        test-aqueous init)
ADD_TEST(Init2       test-aqueous init2)
ADD_TEST(Acetic      test-aqueous dosage ${CMAKE_CURRENT_SOURCE_DIR}/config.lua)
ADD_TEST(Phosphoric  test-aqueous dosage ${CMAKE_CURRENT_SOURCE_DIR}/h3po4.lua)
ADD_TEST(Lua         test-aqueous lua ${CMAKE_CURRENT_SOURCE_DIR}/config.lua)

#ADD_CUSTOM_TARGET(check-aqueous COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-aqueous)
