INCLUDE_DIRECTORIES(.. ../../main)

ADD_EXECUTABLE(gram2defs gram2defs.cpp)
TARGET_LINK_YOCTO(gram2defs lingua)

INSTALL( TARGETS gram2defs RUNTIME DESTINATION bin )
