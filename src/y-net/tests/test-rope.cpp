#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/rope.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;


YOCTO_UNIT_TEST_IMPL(rope)
{
    const uint16_t    port     = 3141;
	const uint16_t    net_port = swap_be(port);
    const IPv4address srv_addr( socket_address_any, net_port );
    
    rope_server srv( srv_addr );
    
    srv.run();
    
    
    
}
YOCTO_UNIT_TEST_DONE()