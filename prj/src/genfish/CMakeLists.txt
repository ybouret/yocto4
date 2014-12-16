PROJECT(genfish)

ADD_EXECUTABLE(genfish genfish.cpp)
TARGET_LINK_YOCTO(genfish mk lua++ lua)

INSTALL( TARGETS genfish RUNTIME DESTINATION bin )


