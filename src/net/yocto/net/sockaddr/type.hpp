//! \file socket-address-type.hpp

#ifndef YOCTO_SOCKADDR_TYPE_INCLUDED
#define YOCTO_SOCKADDR_TYPE_INCLUDED 1

#include "yocto/net/sockaddr/base.hpp"


#include <cstring>

namespace yocto {

    namespace network {


        //! a generic socket address
        template <
        const socket_address_format *F,
        typename                     T
        >
        class _socket_address : public socket_address
        {
        public:
            virtual ~_socket_address() throw() {}

            //! predefined address
            explicit _socket_address( socket_address_type type = socket_address_none, net16_t net_port = 0 ) throw() :
            socket_address( get_format(), type, net_port ),
            host( *static_cast<T *>(host_) )
            {
            }

            //! copy constructor
            _socket_address( const _socket_address &other ) throw() :
            socket_address( other ),
            host( *static_cast<T *>(host_) )
            {
            }

            //! assign operator
            _socket_address & operator=( const _socket_address &other ) throw()
            {
                assert( length() == other.length() );
                if( this != &other )
                {
                    memcpy( addr_, other.addr_, length() );
                }
                return *this;
            }

            T &host;

            //! constructor with name resolution
            inline _socket_address( const string &net_name, net16_t net_port = 0 ) :
            socket_address( get_format(), net_name, net_port ),
            host( *static_cast<T *>(host_) )
            {
            }
            
        private:
            static inline const socket_address_format & get_format() throw()
            {
                //assert(F!=NULL);
                return *F;
            }
        };
        
        
    }
    
}


#endif

