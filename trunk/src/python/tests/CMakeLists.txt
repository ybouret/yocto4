FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-python main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-python python)
TARGET_LINK_LIBRARIES(test-python ${PYTHON_LIBRARY})

