ADD_EXECUTABLE(genbitrev genbitrev.cpp)
TARGET_LINK_YOCTO(genbitrev)

ADD_EXECUTABLE(test-xbitrev test-xbitrev.cpp xbitrev.hpp xbitrev.cpp)
TARGET_LINK_YOCTO(test-xbitrev)

#INSTALL( TARGETS file2data RUNTIME DESTINATION bin )


