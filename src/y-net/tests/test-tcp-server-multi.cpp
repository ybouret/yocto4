#include "yocto/utest/run.hpp"
#include "yocto/net/ip-address.hpp"
#include "yocto/net/tcp-socket.hpp"
#include "yocto/ios/iflux.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/net/socket-set.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/counted.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;

namespace  {
	
	
	typedef socket_address ClientID;
	
	class Client : public counted, public tcp_client
	{
	public:
		
		inline Client( tcp_server &srv ) : tcp_client(srv), closing(false)
		{
		}
		
		virtual ~Client() throw() {}
		
		typedef intr_ptr<ClientID,Client> Ptr; 
		
		const ClientID & key() const throw() { return self(); }
		
		bool closing;
		
	private:
		
		YOCTO_DISABLE_COPY_AND_ASSIGN(Client);
	};
	
}

typedef set<ClientID,Client::Ptr> cln_set_t;

static inline void clean( string &txt ) throw()
{
	while( txt.size() > 0 )
	{
		const char C = txt[ txt.size() - 1];
		if( C == '\r' || C == '\n')
		{
			txt.trim(1);
			continue;
		}
		break;
	}
}

static inline void handle_tcp_server( socket_address &ip )
{
	// start up !
	std::cerr << "-- tcp_server@" << ip << " port " << swap_be(ip.port) << std::endl;
	tcp_server srv( ip, 2 );
	memory::buffer_of<char,memory::global> iobuff( 128 );
	//string     line;
	
	socket_set   the_set;      // socket set
	const delay  the_delay(2); // default wait delay
	cln_set_t               cln_set;
	list<network::socket*>  cln_out;

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
			cln_out.free();
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
					the_set.insert( *pCln );
				}
				catch(...)
				{
					(void) cln_set.remove( pCln->key() );
					throw;
				}
				const socket_address &ipaddr = pCln->self();
				std::cerr << ".. linked to " << ipaddr << ":" << swap_be(ipaddr.port) << std::endl;
			}
			else 
			{
				//-- client activity
				std::cerr << "-- client activity:" << std::endl;
				for( cln_set_t::iterator i = cln_set.begin(); i != cln_set.end(); ++i )
				{
					Client               &cln    = **i;
					const socket_address &ipaddr = cln.self();
					
					if( the_set.is_ready( cln  ) )
					{
						// check for incoming
						const size_t nr = cln.recv( iobuff.rw(), iobuff.length() );
						if( nr <= 0 )
						{
							//-- disconnecting
							network::socket *sock = &cln;
							cln_out.push_back( sock );
							std::cerr << "<" << ipaddr << ":" << swap_be(ipaddr.port) << "> [disconnecting]" << std::endl;
						}
						else 
						{
							string txt( (char*)iobuff.ro(),nr );
							clean(txt);
							std::cerr << "<" << ipaddr << ":" << swap_be(ipaddr.port) << ">'" << txt << "'" << std::endl;
						}
					}
				}
				
				while( cln_out.size() > 0 )
				{
					network::socket &sock = *cln_out.back();
					cln_set.remove( sock.self() );
					the_set.remove( sock        );
					cln_out.pop_back();
				}
				
				
				
			}
			
		}
		
		
		
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(tcp_server_multi)
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
