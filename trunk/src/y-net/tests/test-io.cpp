#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/tcp-socket.hpp"
#include "yocto/net/io/queue.hpp"
#include "yocto/hashing/sha1.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;


static inline void handle_server( socket_address &ip )
{
	
	io_cache      ioDB( 16 );
	io_queue      Q( ioDB );
	tcp_server    srv( ip, 2 );
	hashing::sha1 h;
	
	uint8_t       bin[ hashing::sha1:: __length ];
	const size_t  num = hashing::sha1:: __length;
	
	bool run = true;
	while( run )
	{
		// wait for client
		std::cerr << "-- listening..." << std::endl;
		tcp_client cln( srv );
		std::cerr << "-- accepting from " << cln.self() << std::endl;
		string line;
		while( Q.recv( cln ) )
		{
			while( Q.read_line( line ) > 0 )
			{
				std::cerr << "'" << line << "'" << std::endl;
				
				if( ".bye" == line )
				{
					goto DISCONNECT;
				}
				
				if( ".quit" == line )
				{
					run = false;
					goto DISCONNECT;
				}
				
				h.set();
				h(line);
				h.get(bin,sizeof(bin));
				
				if( true )
				{
					for( size_t i=0; i < num; ++i )
					{
						Q("%02X", bin[i]);
					}
					Q.write( '\n' );
					while( ! Q.sent( cln ) );
				}
				line.clear();
			}
		}
	DISCONNECT:
		Q.reset();
		std::cerr << "cache count=" << ioDB.count() << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(ioQ)
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
		handle_server( addr );
	}
	else 
	{
		IPv6address addr( socket_address_any, net_port );
		handle_server( addr );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()





