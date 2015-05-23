FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs}
	../../../data/json.g
	../../../data/simple.g
	../../../data/calc.g
	../../../data/lists.g
	../../../data/eval.g
)
TARGET_LINK_YOCTO(test-lang lang)
 
