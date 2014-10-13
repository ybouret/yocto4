PROJECT(volfit)

ADD_EXECUTABLE(volfit volfit.cpp)
TARGET_LINK_YOCTO(volfit mk lua++ lua)

INSTALL( TARGETS volfit RUNTIME DESTINATION bin )


