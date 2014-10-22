
INCLUDE_DIRECTORIES(../../fame)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-visit main.cpp ${test_srcs})

TARGET_LINK_VISIT( test-visit )
TARGET_LINK_YOCTO( test-visit fame )

VISIT_CUSTOM_UI( test-visit loop.ui )

#INSTALL( TARGETS test-visit RUNTIME DESTINATION bin )

