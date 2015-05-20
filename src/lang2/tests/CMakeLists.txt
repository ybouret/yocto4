FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang2 main.cpp ${srcs}
	../../../data/json.g
	../../../data/simple.g
	../../../data/calc.g
	../../../data/list.g
)
TARGET_LINK_YOCTO(test-lang2 lang2)
 
