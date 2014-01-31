PROJECT(cavit)

ADD_EXECUTABLE(cavit cavit.cpp config.lua)
TARGET_LINK_YOCTO(cavit gems mk lua++ lua)

#INSTALL( TARGETS file2data RUNTIME DESTINATION bin )


