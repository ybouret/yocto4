
ADD_EXECUTABLE(ssh-rsa-pub pub.cpp)
TARGET_LINK_YOCTO(ssh-rsa-pub mpl)


#INSTALL( TARGETS b64enc b64dec RUNTIME DESTINATION bin )


