
SET(fx test-lua)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(${fx} main.cpp ${test_srcs})
TARGET_LINK_YOCTO(${fx} lua)
SET( HERE "${CMAKE_SOURCE_DIR}/third_party/lua/tests" )

ADD_CUSTOM_COMMAND(
	TARGET ${fx}
	POST_BUILD
	COMMAND ${fx} "${HERE}/hello.lua"
	COMMAND ${fx} "${HERE}/bisect.lua"
	COMMAND ${fx} "${HERE}/sieve.lua"
	COMMAND ${fx} "${HERE}/sort.lua"
	COMMENT "Auto Testing Lua"
)

