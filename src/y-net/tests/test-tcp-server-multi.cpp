#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/tcp-socket.hpp"
#include "yocto/ios/iflux.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/net/socket-set.hpp"
#include "yocto/associative/set.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

namespace  {
	
	
	typedef ptrdiff_t ClientID;
	
	class Client : public counted
	{
	public:
		
		inline Client( tcp_server &srv ) : sock_( srv ), id_( sock_.fd_value() )
		{
		}
		
		virtual ~Client() throw() {}
		
		typedef intrusive_ptr<ClientID,Client> Ptr; 
		
		tcp_client     & sock_ref()  throw() { return sock_; }
		const ClientID & key() const throw() { return id_; }
		
	private:
		tcp_client     sock_;
		const ClientID id_;
		
		YOCTO_DISABLE_COPY_AND_ASSIGN(Client);
	};
	
}

typedef set<ClientID,Client::Ptr> cln_set_t;

static inline void handle_tcp_server( socket_address &ip )
{
	// start up !
	std::cerr << "-- tcp_server@" << ip << " port " << swap_be(ip.port) << std::endl;
	tcp_server srv( ip, 2 );
	memory::buffer_of<char,memory::global> iobuff( 128 );
	//string     line;
	
	socket_set   the_set;      // socket set
	const delay  the_delay(2); // default wait delay
	
	cln_set_t cln_set;
	
	//-- insert the server
	the_set.insert( srv );
	
	bool run = true;
	while( run )
	{
		std::cerr << "-- waiting.." << std::endl;
		delay        d   = the_delay;
		const size_t nup = the_set.check(d);
		if(  nup > 0 )
		{
			std::cerr << "#activity=" << nup << std::endl;
			if( the_set.is_ready( srv ) )
			{
				//-- server activity
				std::cerr << "-- new connection!" << std::endl;
				Client::Ptr pCln( new Client(srv) );
				if( !cln_set.insert( pCln ) )
				{
					throw exception("Multiple ClientID!");
				}
				
				try
				{
					the_set.insert( pCln->sock_ref() );
				}
				catch(...)
				{
					(void) cln_set.remove( pCln->key() );
					throw;
				}
				const socket_address &ipaddr = pCln->sock_ref().self();
				std::cerr << ".. linked to " << ipaddr << std::endl;
			}
			else 
			{
				//-- client activity
				std::cerr << "-- client activity:" << std::endl;
				for( cln_set_t::iterator i = cln_set.begin(); i != cln_set.end(); ++i )
				{
					Client::Ptr          &pCln   = *i;
					io_socket            &iosock = pCln->sock_ref();
					network::socket      &ipsock = pCln->sock_ref();
					const socket_address &ipaddr = pCln->sock_ref().self();
					
					if( the_set.is_ready( ipsock ) )
					{
						std::cerr << "<" << ipaddr << ":" << swap_be(ipaddr.port) << ">:";
						const size_t nr =  iosock.recv( iobuff.rw(), iobuff.length() );
						if( nr > 0 )
						{
							const string txt( (char *)iobuff.ro(), nr );
							std::cerr << txt << std::endl;
						}
						else 
						{
							std::cerr << "...disconnecting" << std::endl;
							the_set.remove( ipsock );
							cln_set.remove( pCln->key() );
						}

					}
				}
			}

		}
		
		
		
#if 0
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
#endif
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(tcp_server_multi)
{
	if( argc < 2 )
	{
		throw exception("usage: %s port [4|6]", argv[0]);
	}
	
	const uint16_t port     = strtol(argv[1],NULL,10);
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
