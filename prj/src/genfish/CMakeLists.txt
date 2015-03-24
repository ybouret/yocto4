PROJECT(genfish)

ADD_EXECUTABLE(genfish genfish.cpp config.lua)
TARGET_LINK_YOCTO(genfish mk lua++ lua)

INSTALL( TARGETS genfish RUNTIME DESTINATION bin )

