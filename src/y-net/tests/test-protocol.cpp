#include "yocto/utest/run.hpp"
#include "yocto/net/io/protocol.hpp"
#include "yocto/net/ip-address.hpp"
#include <cstdlib>

using namespace yocto;
using namespace network;

class my_link : public tcp_link
{
public:
    explicit my_link( const connexion &cnx ) throw() : tcp_link(cnx) {}
    virtual ~my_link() throw() {}
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(my_link);
};

typedef intr_ptr<socket_address,my_link> my_conn;


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
		handle_protocol( addr );
	}
	else
	{
		IPv6address addr( socket_address_any, net_port );
		handle_protocol( addr );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()

