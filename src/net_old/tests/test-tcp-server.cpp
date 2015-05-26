#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/tcp-socket.hpp"
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
		std::cerr << "-- accepting from " << cln.self() << std::endl;
		ios::iflux input( cln, iobuff );
		
		line.clear();
		while( input.read_line( line ) > 0 )
		{
			std::cerr << ">" << line << std::endl;
			if( line == ".end" )
				break;
			if( line == ".quit" )
			{
				run = false;
				break;
			}
			
			line.clear();
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
		IPv4address addr( socket_address_any, net_port );
		handle_tcp_server( addr );
	}
	else 
	{
		IPv6address addr( socket_address_any, net_port );
		handle_tcp_server( addr );
	}
	
	
	
}
YOCTO_UNIT_TEST_DONE()
