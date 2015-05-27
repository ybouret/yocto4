#include "yocto/utest/run.hpp"
#include "yocto/net/ipaddr.hpp"
#include "yocto/net/socket/set.hpp"
#include "yocto/net/socket/tcp.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

namespace
{
    static inline
    void  handle_tcp_multi_server( const socket_address &ipaddr )
    {
        // create the server
        socket_ptr srv ( new tcp_server(ipaddr,2) );

        // use a socket set
        socket_set socks;

        // insert the server
        socks.insert(srv);

        // main loop
        bool run = true;
        while(run)
        {
            delay  d  = 1.0;
            std::cerr << "-- checking..." << std::endl;
            size_t na = socks.check(d);

            if(na>0)
            {
                std::cerr << "#activity=" << na << std::endl;
                run = false;
            }
        }


    }


}


YOCTO_UNIT_TEST_IMPL(tcp_multi)
{
    std::cerr << "socket_set::max_size=" << socket_set::max_size << std::endl;

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
        handle_tcp_multi_server( addr );
    }
    else
    {
        IPv6 addr( socket_address_any, net_port );
        handle_tcp_multi_server( addr );
    }


}
YOCTO_UNIT_TEST_DONE()

