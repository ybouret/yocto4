#ifndef YOCTO_NET_INCLUDED
#define YOCTO_NET_INCLUDED 1

#include "yocto/net/sockaddr/sys.hpp"
#include "yocto/net/types.hpp"

#include "yocto/threading/singleton.hpp"
#include "yocto/string.hpp"
namespace yocto
{
    namespace network
    {

        //! forward declaration
        class socket_address;


#if defined(YOCTO_BSD)
        typedef int socket_t;
#endif

#if defined(YOCTO_WIN)
        typedef SOCKET socket_t;
#endif



        //! the only net object, used for platform specific initialization
        class net : public singleton<net>
        {
        public:

            //! get host name.
            string get_host_name() const;

            //! resolving
            /**
             * \param ip the resolved address as a result
             * \param id the host name to resolve
             * It uses the network database.
             */
            void resolve( socket_address &ip, const string &id ) const;
            
            
            static const uint16_t reserved_port;   //!< port < reserved_port: for system
            static const uint16_t first_user_port; //!< port >= first_user_port: for user
            static const uint16_t final_user_port = 65535;
            static const uint16_t user_port_width; //!< final_user_port-first_user_port+1;
            
            
        private:
            explicit net();
            virtual ~net() throw();
            friend class singleton<net>;
            static const threading::longevity life_time = object::provider::life_time - 2;
            static const char name[];
        };


    }


}


#endif

