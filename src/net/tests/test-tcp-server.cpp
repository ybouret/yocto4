#include "yocto/utest/run.hpp"
#include "yocto/net/ipaddr.hpp"
#include "yocto/net/socket/tcp.hpp"
#include "yocto/ios/iflux.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

static inline void handle_tcp_server( socket_address &ip )
{
    // start up !
    std::cerr << "-- tcp_server@" << ip << " port " << swap_be(ip.port) << std::endl;
    tcp_server srv( ip, 2 );
    memory::buffer_of<char,memory::global> iobuff( 128 );
    string     line;

    bool run = true;
    while( run )
    {
        // wait for client
        std::cerr << "-- listening..." << std::endl;
        tcp_client cln( srv );
        std::cerr << "-- accepting from " << cln.self() << ":" << int( swap_be(cln.self().port)) << std::endl;



        for(;;)
        {
            size_t nr = cln.recv(iobuff.rw(),iobuff.length());
            if(nr<=0)
            {
                break;
            }
            std::cerr.write(iobuff(),nr);

            if(3!=cln.send("OK\n",3))
            {
                std::cerr << "-- send failure..." << std::endl;
                break;
            }

            if( ! memcmp(".end", iobuff(), 4) )
            {
                break;
            }

            if( ! memcmp(".quit",iobuff(),5) )
            {
                run = false;
                break;
            }

        }

        
    }


}

YOCTO_UNIT_TEST_IMPL(tcp_server)
{
    if( argc < 2 )
    {
        throw exception("usage: %s port [4|6]", argv[0]);
    }

    const uint16_t port     = uint16_t(strtol(argv[1],NULL,10));
    const uint16_t net_port = swap_be(port);
    int            version  = 4;
    if( argc > 2 )
    {
        version = strtol(argv[2], NULL, 10);
        if( version != 4 && version != 6 )
            throw exception("bad version %d", version);
    }
    
    if( version == 4 )
    {
        IPv4 addr( socket_address_any, net_port );
        handle_tcp_server( addr );
    }
    else 
    {
        IPv6 addr( socket_address_any, net_port );
        handle_tcp_server( addr );
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
