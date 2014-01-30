PROJECT(cavit)

ADD_EXECUTABLE(cavit cavit.cpp)
TARGET_LINK_YOCTO(cavit gems mk)

#INSTALL( TARGETS file2data RUNTIME DESTINATION bin )


