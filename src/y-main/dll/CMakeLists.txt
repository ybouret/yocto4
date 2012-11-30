ADD_LIBRARY(module MODULE module.cpp module.hpp)
ADD_EXECUTABLE(loader loader.cpp)
TARGET_LINK_YOCTO(loader)

