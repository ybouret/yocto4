#include "yocto/utest/run.hpp"
#include "yocto/net/tcp/protocol.hpp"
#include "yocto/net/ipaddr.hpp"

using namespace yocto;
using namespace network;


namespace
{
    class YProtocol : public protocol
    {
    public:
        YProtocol(const socket_address &ipaddr) : protocol(ipaddr,4)
        {
        }



        virtual ~YProtocol() throw()
        {
        }

        virtual void onInit( connexion &conn )
        {
            const  char msg[] = "Welcome\n";
            size_t done = 0;
            conn.sendQ.put_all(msg,length_of(msg),done);
        }

        virtual void onRecv( connexion &conn )
        {
            std::cerr << conn.self() << ":" << swap_be(conn.self().port) << " : rbytes=" << conn.recvQ.bytes() << std::endl;
            conn.sendQ.merge(conn.recvQ);
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(YProtocol);
    };

}

static inline void handle_proto(const socket_address &ipaddr)
{
    YProtocol proto(ipaddr);

    proto.stand_by = 5;
    proto.execute();


}

YOCTO_UNIT_TEST_IMPL(proto)
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
        handle_proto( addr );
    }
    else
    {
        IPv6 addr( socket_address_any, net_port );
        handle_proto( addr );
    }
    
}
YOCTO_UNIT_TEST_DONE()
