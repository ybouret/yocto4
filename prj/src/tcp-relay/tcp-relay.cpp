#include "yocto/net/tcp/server-protocol.hpp"
#include "yocto/program.hpp"
#include "yocto/net/ipaddr.hpp"


using namespace yocto;
using namespace network;

class RelayProtocol : public server_protocol
{
public:
    const socket_address client_addr;
    connexion *source;
    connexion *target;
    
    explicit RelayProtocol(const socket_address &usr_addr, const socket_address &server_addr) :
    server_protocol(server_addr,1,128),
    client_addr(usr_addr),
    source(0),
    target(0)
    {
        
    }
    
    virtual ~RelayProtocol() throw()
    {
    }
    
    virtual void onInit( connexion &conn )
    {
        if(source)
        {
            if(target)
            {
                target->close(); // refuse new connection
                return;
            }
            else
            {
                // second connexion: target
                target = &conn;
                std::cerr << "relay.connected to " << conn.self() << ":" << swap_be(conn.self().port) << std::endl;
            }
        }
        else
        {
            //first connexion
            std::cerr << "relay.connexion from " << conn.self() << ":" << swap_be(conn.self().port) << std::endl;
            source = &conn;
            link_to(client_addr);
        }
    }
    
    virtual void onQuit( connexion &conn ) throw()
    {
        this->suspend();
    }
    
    virtual void onRecv( connexion &conn )
    {
        if(&conn==source)
        {
            target->sendQ.merge(source->recvQ);
        }
        else
        {
            assert(&conn==target);
            source->sendQ.merge(target->sendQ);
        }
        
    }

private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(RelayProtocol);
};

#include "yocto/string/conv.hpp"
#include "yocto/fs/vfs.hpp"

YOCTO_PROGRAM_START()
{
    if(argc<3)
    {
        throw exception("usage: %s client port server_port", vfs::get_base_name(argv[0]));
    }
    const string   client_name = argv[1];
    const uint16_t client_port = strconv::to<size_t>(argv[2],"client port");
    const uint16_t server_port = strconv::to<size_t>(argv[3],"server port");
    const IPv4     client_addr( client_name, swap_be(client_port));
    const IPv4     server_addr( socket_address_any, swap_be(server_port) );

    RelayProtocol  relay(client_addr,server_addr);
    relay.execute();
}
YOCTO_PROGRAM_END()
