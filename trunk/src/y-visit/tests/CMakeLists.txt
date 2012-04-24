
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-visit main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-visit visit)
TARGET_LINK_VISIT( test-visit )

SET(VISIT_UI_DIR "$ENV{HOME}/.visit/ui")
ADD_CUSTOM_COMMAND( TARGET test-visit 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory  "${VISIT_UI_DIR}"
        COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/loop.ui" "${VISIT_UI_DIR}/loop.ui"
        COMMENT "Copy loop.ui to ${VISIT_UI_DIR}"
        )

#ENABLE_TESTING()
#ADD_TEST(Condition test-visit condition)
#ADD_TEST(Barrier   test-visit barrier)
#ADD_TEST(Team      test-visit team)

#ADD_CUSTOM_TARGET(check-thr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-thr)
