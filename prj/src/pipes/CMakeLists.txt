PROJECT(pipes)

ADD_EXECUTABLE(pipes pipes.cpp)
TARGET_LINK_YOCTO(pipes)

ADD_EXECUTABLE(launch launch.cpp)
TARGET_LINK_YOCTO(launch)

