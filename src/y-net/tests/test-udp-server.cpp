#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/udp-socket.hpp"
#include "yocto/ios/iflux.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

static inline void handle_udp_server( socket_address &ip )
{
	// start up !
	std::cerr << "-- udp_server@" << ip << " port " << swap_be(ip.port) << std::endl;
	udp_server srv( ip );
	memory::buffer_of<char,memory::global> iobuff(128);
	
	bool run = true;
	while( run )
	{
		size_t done=0;
		memset( iobuff.rw(), 0, iobuff.length() );
		srv.get( iobuff.rw(), iobuff.length()-1, done);
		const string msg( iobuff(), done );
		std::cerr << "from " << srv.peer() << std::endl;
		std::cerr << "'" << msg << "'" << std::endl;
		if( ".quit" == msg ) run=false;
	}
	
}

YOCTO_UNIT_TEST_IMPL(udp_server)
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
		IPv4address addr( socket_address_any, net_port );
		handle_udp_server( addr );
	}
	else 
	{
		IPv6address addr( socket_address_any, net_port );
		handle_udp_server( addr );
	}
	
	
	
}
YOCTO_UNIT_TEST_DONE()
