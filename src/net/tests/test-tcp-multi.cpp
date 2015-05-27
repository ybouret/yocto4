#include "yocto/utest/run.hpp"
#include "yocto/net/ipaddr.hpp"
#include "yocto/net/socket/set.hpp"
#include "yocto/net/socket/tcp.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/sequence/list.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

namespace
{
    typedef intr_ptr<sock_key_t,tcp_client> Client;
    typedef set<sock_key_t,Client>          Clients;



    static inline
    void  handle_tcp_multi_server( const socket_address &ipaddr )
    {
        // create the server
        tcp_server srv(ipaddr,2);
        srv.blocking(false);

        // use a socket set
        socket_set socks;

        // insert the server
        socks.insert(srv);

        Clients clients;

        memory::buffer_of<char,memory::global> iobuf(1024);
        list<sock_key_t> to_remove;

        // main loop
        bool  run = true;
        while(run)
        {
            delay  d  = 1.0;
            std::cerr << "-- checking " << srv.self() << ":" << int(swap_be(srv.self().port)) << std::endl;
            const size_t na = socks.check(d);
            std::cerr << "\t#activity=" << na << std::endl;

            if(na>0)
            {
                if(socks.is_ready(srv))
                {
                    Client cln( new tcp_client(srv) );
                    if(!clients.insert(cln))
                    {
                        throw exception("unable to insert client");
                    }
                    std::cerr << "\t new connection from " << cln->self() << " : " << int(swap_be(cln->self().port)) << std::endl;
                    socks.insert(*cln);
                }
                else
                {
                    //a client is recv something
                    to_remove.free();
                    for( Clients::iterator i=clients.begin();i!=clients.end();++i)
                    {
                        tcp_client  &cln = **i;
                        if(socks.is_ready(cln))
                        {
                            const size_t nr  = cln.recv(iobuf(),iobuf.length());
                            std::cerr << "\t#recv=" << nr << " from " << cln.self() << ":" << int(swap_be(cln.self().port)) << std::endl;
                            if(nr<=0)
                            {
                                std::cerr << "\tshould disconnect" << std::endl;
                                to_remove.push_back( cln.key() );
                            }
                        }
                    }
                    while( to_remove.size() )
                    {
                        const sock_key_t &k   = to_remove.back();
                        Client           &cln = * clients.search(k);
                        socks.remove(*cln);
                        clients.remove(k);
                        to_remove.pop_back();
                    }

                }
            }
        }


    }


}


YOCTO_UNIT_TEST_IMPL(tcp_multi)
{
    std::cerr << "sizeof(socket)      =" << sizeof(network::socket) << std::endl;
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

