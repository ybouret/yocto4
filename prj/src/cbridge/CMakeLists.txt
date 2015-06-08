
ADD_EXECUTABLE(cbridge cbridge.cpp)
TARGET_LINK_YOCTO(cbridge mk)
#INSTALL( TARGETS otsu-voro RUNTIME DESTINATION bin )

ADD_EXECUTABLE(cbridge2 cbridge2.cpp)
TARGET_LINK_YOCTO(cbridge2 mk)

