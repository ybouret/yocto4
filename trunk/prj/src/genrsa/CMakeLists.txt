PROJECT(genrsa)

ADD_EXECUTABLE(parse-rsa parse-rsa.cpp)
TARGET_LINK_YOCTO(parse-rsa pki mpk)

INSTALL( TARGETS parse-rsa RUNTIME DESTINATION bin )
