
ADD_EXECUTABLE(findsymbol findsymbol.cpp)
TARGET_LINK_YOCTO(findsymbol)

IF(YOCTO_LINUX)
TARGET_LINK_LIBRARIES(findsymbol dl)
ENDIF()

INSTALL( TARGETS findsymbol RUNTIME DESTINATION bin )


