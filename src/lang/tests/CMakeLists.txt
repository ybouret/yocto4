FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs}
	json.g
	simple.g
	calc.g
	lists.g
	eval.g
)
TARGET_LINK_YOCTO(test-lang lang)
 
