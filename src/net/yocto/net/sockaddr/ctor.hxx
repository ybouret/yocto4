#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

#define YOCTO_SOCKADDR_ADDR(X) ( (void*)(static_cast<uint8_t *>( (void *)&addr_[0] ) + (X)) )
#define YOCTO_SOCKADDR_HOST YOCTO_SOCKADDR_ADDR( fmt.host_offset )
#define YOCTO_SOCKADDR_PORT *(net16_t *)YOCTO_SOCKADDR_ADDR( fmt.port_offset )

#define YOCTO_SOCKADDR_CTOR() \
addr_(),                      \
host_( YOCTO_SOCKADDR_HOST ), \
port(  YOCTO_SOCKADDR_PORT )
