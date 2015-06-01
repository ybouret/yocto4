
ADD_EXECUTABLE(y_tcp_relay tcp-relay.cpp)
TARGET_LINK_YOCTO(y_tcp_relay net)
#INSTALL( TARGETS otsu-voro RUNTIME DESTINATION bin )

