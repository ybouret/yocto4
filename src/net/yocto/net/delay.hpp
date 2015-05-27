#ifndef YOCTO_NET_DELAY_INCLUDED
#define YOCTO_NET_DELAY_INCLUDED 1

#include "yocto/net/sockaddr/sys.hpp"

#if defined(YOCTO_BSD)
#include <sys/time.h>
#endif

namespace yocto {

    namespace network {

        class socket_set;
        
        //! wrapper for select(...) timeval
        class delay 
        {
        public:
            delay( const double seconds=-1.0) throw();
            ~delay() throw();
            delay( const delay &) throw();
            delay&operator=( const delay & other ) throw();

            double operator*() const throw();

        private:
            struct timeval  data_; //!< time from 0 to 1 day.
            struct timeval *wait_; //!< NULL: wait for ever else wait = data
            friend class socket_set;
        };
        
    }
    
}

#endif

