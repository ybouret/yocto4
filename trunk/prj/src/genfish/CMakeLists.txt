PROJECT(genfish)

ADD_EXECUTABLE(genfish2 genfish2.cpp config.lua)
TARGET_LINK_YOCTO(genfish2 mk lua++ lua)

INSTALL( TARGETS genfish2 RUNTIME DESTINATION bin )

