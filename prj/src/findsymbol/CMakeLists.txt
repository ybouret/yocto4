
ADD_EXECUTABLE(findsymbol findsymbol.cpp)
TARGET_LINK_YOCTO(findsymbol)

INSTALL( TARGETS findsymbol RUNTIME DESTINATION bin )


