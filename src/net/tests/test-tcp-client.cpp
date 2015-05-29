#include "yocto/utest/run.hpp"
#include "yocto/net/socket/tcp.hpp"
#include "yocto/net/ipaddr.hpp"
#include "yocto/ios/icstream.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;


static 
void handle_tcp_client( const socket_address &ip)
{
	//net &NT = net::instance();
	//NT.resolve( ip, hostname );
	std::cerr << "-- tcp_client to " << ip << ":" << swap_be( ip.port ) << std::endl;
	try 
	{
		tcp_client cln( ip );
		ios::icstream input( ios::cstdin );
		(std::cerr << "> ").flush();
		string line;
		while( input.read_line(line) > 0 )
		{
			std::cerr << "-->'" << line << "'" << std::endl;
			line += '\n';
			
			//cln.put_all( line, done);
            const size_t ns = line.size();
            if(ns!=cln.send(line.c_str(),ns))
            {
                break;
            }
            line.clear();
			(std::cerr << "> ").flush();
		}
	}
	catch(...)
	{
		std::cerr << std::endl;
		std::cerr << "[I/O exception detected]" << std::endl;
		throw;
	}
}

YOCTO_UNIT_TEST_IMPL(tcp_client)
{
	if( argc < 3 )
	{
		throw exception("usage: %s hostname port [4|6]", argv[0]);
	}
	
	const string   hostname = argv[1];
	const uint16_t port     = uint16_t(strtol(argv[2],NULL,10));
	const uint16_t net_port = swap_be(port);
	int            version  = 4;
	if( argc > 3 )
	{
		version = strtol(argv[3], NULL, 10);
		if( version != 4 && version != 6 )
			throw exception("bad version %d", version);
	}
	
	if( version == 4 )
	{
		IPv4 addr( hostname, net_port );
		handle_tcp_client( addr );
	}
	else 
	{
		IPv6 addr( hostname, net_port );
		handle_tcp_client( addr );
	}
	
}
YOCTO_UNIT_TEST_DONE()

