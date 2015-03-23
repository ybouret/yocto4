
ADD_EXECUTABLE(bitrev bitrev.cpp)
TARGET_LINK_YOCTO(bitrev)

ADD_EXECUTABLE(bitrev2 bitrev2.cpp)
TARGET_LINK_YOCTO(bitrev2)

ADD_EXECUTABLE(genbitrev genbitrev.cpp)
TARGET_LINK_YOCTO(genbitrev)


#INSTALL( TARGETS file2data RUNTIME DESTINATION bin )


