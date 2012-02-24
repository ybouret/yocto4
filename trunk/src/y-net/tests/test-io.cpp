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
		handle_server( addr );
	}
	else 
	{
		IPv6address addr( socket_address_any, net_port );
		handle_server( addr );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/net/io/protocol.hpp"

class my_link : public tcp_link
{
public:
    explicit my_link( const connexion &cnx ) throw() : tcp_link(cnx) {}
    virtual ~my_link() throw() {}
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(my_link);
};

typedef intrusive_ptr<socket_address,my_link> my_conn;


class my_proto : public server_protocol
{
public:
    typedef set<socket_address,my_conn> cnx_set;
    
    cnx_set clients;
    
    
	explicit my_proto( socket_address &ip ) : 
    server_protocol( ip, 2, 16 ),
    clients()
	{
		
		waiting = delay(5.0);
	}
	
    virtual void on_init( connexion &cnx )
    {
        const my_conn conn( new my_link( cnx ) );
        std::cerr << "MyConn: [+]" << conn->key() << std::endl;
        clients.insert( conn );
    }
    
    virtual void on_quit( connexion &cnx ) throw()
    {
        std::cerr << "MyConn: [-]" << cnx->key() << std::endl;
        (void) clients.remove( cnx->key() );
    }
    
	virtual void on_recv( connexion &cnx )
	{
		protocol::on_recv(cnx);
		char C;
		std::cerr << "cnx.input_cache=" << cnx->input_cache() << std::endl;
		while( cnx->input.query(C) )
		{
			
			C = make_visible(C);
			cnx->output.write(C);
			if( C == '0' )
			{
				cnx->output.write('\n');
				cnx->close();
			}
			
			if( C == '1' )
				cnx->proto.stop();
		}
	}
	
	virtual ~my_proto() throw() {}
	
	
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(my_proto);
};


static inline void handle_protocol( socket_address &ip )
{
	my_proto proto( ip );
	proto.run();
}

YOCTO_UNIT_TEST_IMPL(protocol)
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
		handle_protocol( addr );
	}
	else 
	{
		IPv6address addr( socket_address_any, net_port );
		handle_protocol( addr );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()




