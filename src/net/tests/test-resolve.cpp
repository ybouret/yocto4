#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/network.hpp"
#include "yocto/associative/key-hasher.hpp"

using namespace yocto;
using namespace network;

YOCTO_UNIT_TEST_IMPL(resolve)
{
	net &nt  = net::instance();
	const string name = argc > 1 ? argv[1] : "localhost";
	std::cerr << "resolving '" << name << "'" << std::endl;
	key_hasher<socket_address> sock_h;
	
	try {
		IPv4address addr4;
		nt.resolve( addr4, name);
		
		std::cerr << "addr4=" << addr4 << std::endl;
		std::cerr << "hash4=" << sock_h(addr4) << std::endl;
	}
	catch( const exception &e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.when() << std::endl;
	}
	catch (...) {
		std::cerr << "unhandled exception!" << std::endl;
	}
	
	try {
		IPv6address addr6;
		nt.resolve( addr6, name);
		std::cerr << "addr6=" << addr6 << std::endl;
	}
	catch( const exception &e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.when() << std::endl;
	}
	catch (...) {
		std::cerr << "unhandled exception!" << std::endl;
	}
	
	
	
}
YOCTO_UNIT_TEST_DONE()
