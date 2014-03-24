PROJECT(genrsa)

ADD_EXECUTABLE(parse-rsa parse-rsa.cpp)
TARGET_LINK_YOCTO(parse-rsa mpa)

INSTALL( TARGETS parse-rsa RUNTIME DESTINATION bin )
