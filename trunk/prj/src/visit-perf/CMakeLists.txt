
INCLUDE(${SDK}/share/yocto/visit.cmake)


ADD_EXECUTABLE(visit-perf visit-perf.cpp)

TARGET_LINK_VISIT(visit-perf)
TARGET_LINK_YOCTO(visit-perf spade)


#INSTALL( TARGETS b64enc b64dec RUNTIME DESTINATION bin )


