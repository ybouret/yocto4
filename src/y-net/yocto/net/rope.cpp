#include "yocto/net/rope.hpp"

namespace yocto 
{
    namespace network
    {
        
        
        rope_link:: rope_link( const connexion &cnx ) :
        tcp_link( cnx ),
        request()
        {
        }
        
        rope_link:: ~rope_link() throw()
        {
            
        }
        
        rope_server:: ~rope_server() throw()
        {
            
        }
        
        rope_server:: rope_server( const socket_address &ip, const size_t bs  ) :
        server_protocol(ip,2,bs)
        {
            
        }
        
        
        void rope_server:: on_init(connexion &cnx)
        {
            std::cerr << "[ROPE INIT] " << cnx->key() << ":" << swap_be( cnx->key().port ) << std::endl;
            rope_client cln( new rope_link(cnx) );
            if( ! clients.insert( cln ) )
            {
                throw exception("ROPE: Unexpected multiple connexion");
            }
        }
        
        
        void rope_server:: on_quit(connexion &cnx) throw()
        {
            std::cerr << "[ROPE QUIT]" << cnx->key() << ":" << swap_be( cnx->key().port ) << std::endl;
            (void) clients.remove( cnx->key() );
        }
        
        
        void rope_server:: on_recv(connexion &cnx)
        {
            std::cerr << "[ROPE RECV]" << cnx->key() << ":" << swap_be( cnx->key().port ) << std::endl;
            rope_client *ppCln = clients.search( cnx->key() );
           
            if( !ppCln )
            {
                throw exception("ROPE: invalid recv connexion");
            }
            
            rope_link &cln = **ppCln;
            if( cnx->input.read_line( cln.request ) > 0 )
            {
                std::cerr << "\t '" << cln.request << "'" << std::endl;
                cln.request.clear();
            }
            
        }
        
    }
    
}
