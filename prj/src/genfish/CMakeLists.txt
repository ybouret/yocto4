PROJECT(genfish)

ADD_EXECUTABLE(genfish genfish.cpp config.lua)
TARGET_LINK_YOCTO(genfish mk lua++ lua)

INSTALL( TARGETS genfish RUNTIME DESTINATION bin )

ADD_EXECUTABLE(genfish2 genfish2.cpp config.lua)
TARGET_LINK_YOCTO(genfish2 mk lua++ lua)

INSTALL( TARGETS genfish2 RUNTIME DESTINATION bin )

