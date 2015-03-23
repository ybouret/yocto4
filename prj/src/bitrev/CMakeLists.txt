
ADD_EXECUTABLE(bitrev bitrev.cpp)
TARGET_LINK_YOCTO(bitrev)

ADD_EXECUTABLE(bitrev2 bitrev2.cpp)
TARGET_LINK_YOCTO(bitrev2)

ADD_EXECUTABLE(genbitrev genbitrev.cpp)
TARGET_LINK_YOCTO(genbitrev)

ADD_EXECUTABLE(test-xbitrev test-xbitrev.cpp xbitrev.hpp xbitrev.cpp)
TARGET_LINK_YOCTO(test-xbitrev)

#INSTALL( TARGETS file2data RUNTIME DESTINATION bin )


