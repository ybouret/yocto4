INCLUDE_DIRECTORIES(../yocto/png)
ADD_EXECUTABLE(pngtest pngtest.c)
TARGET_LINK_LIBRARIES(pngtest y-png y-z)

ENABLE_TESTING()

ADD_TEST(pngtest ./pngtest ${CMAKE_CURRENT_SOURCE_DIR}/pngtest.png)
IF(MSVC)

ELSE()
TARGET_LINK_LIBRARIES(pngtest m)
ENDIF()
