FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang2 main.cpp ${srcs}
	../../../data/json.g
	../../../data/simple.g
	../../../data/calc.g
	../../../data/lists.g
)
TARGET_LINK_YOCTO(test-lang2 lang2)
 
