PROJECT(otsu-voro)

ADD_EXECUTABLE(otsu-voro otsu-voro.cpp)
TARGET_LINK_YOCTO(otsu-voro mk)
INSTALL( TARGETS otsu-voro RUNTIME DESTINATION bin )

