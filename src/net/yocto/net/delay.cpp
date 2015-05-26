#include "yocto/net/delay.hpp"
#include "yocto/code/utils.hpp"
#include <cmath>

namespace yocto {

    namespace network {

        static const struct timeval delay_data_init = { 0, 0 };

        delay:: delay( const double seconds ) throw() :
        data_( delay_data_init ),
        wait_(NULL)
        {
            if( seconds >= 0 ) {
                wait_ = &data_;
                const double s     = clamp<double>(0,seconds,86400);
                double       nsec  = 0;
                double       nusec = std::modf(s,&nsec);
                nusec              = std::floor(1.0e6 * nusec);

                data_.tv_sec  = (unsigned int)nsec;
                data_.tv_usec = (unsigned int)nusec;

                while( data_.tv_usec>=1000000) {
                    /* +1 sec       */ ++data_.tv_sec;
                    /* - 1M micro   */   data_.tv_usec -= 1000000;
                }

                //std::cout << seconds << " seconds->(" << data_.tv_sec << "," << data_.tv_usec <<")" << std::endl;
            }
        }


        delay:: ~delay() throw() {
        }


        delay:: delay( const delay &other ) throw() :
        data_( other.data_ ),
        wait_( other.wait_ ? &data_ : NULL )
        {
            
        }
        
        delay &delay::operator=( const delay &other ) throw() {
            if( this != &other )
            {
                data_ = other.data_;
                wait_ = other.wait_ ? &data_ : NULL;
            }
            return *this;
        }
        
        
        double delay:: operator*() const throw() {
            return wait_ ? data_.tv_sec + 1.0e6 * data_.tv_usec : -1;
        }


    }

}
