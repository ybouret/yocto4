#include "yocto/mpa/word2mpz.hpp"
#include "yocto/error.hpp"

namespace yocto
{
    namespace mpa
    {
        int integer:: compare(const integer &lhs, const integer &rhs ) throw()
        {
            switch(lhs.s)
            {
                case __negative:
                {
                    switch(rhs.s)
                    {
                        case __negative:
                            return -natural::compare(lhs.n, rhs.n);
                            
                        case __zero:
                        case __positive:
                            return -1;
                    }
                }
                    
                case __zero:
                {
                    switch(rhs.s)
                    {
                        case __negative:
                            return -1;
                            
                        case __zero:
                            return 0;
                            
                        case __positive:
                            return 1;
                    }
                }
                    
                case __positive:
                {
                    switch(rhs.s)
                    {
                        case __negative:
                        case __zero:
                            return 1;
                            
                        case __positive:
                            return natural::compare(lhs.n, rhs.n);
                    }
                }
                    
            }
            critical_error(error_invalid_data,"mpa.integer.compare: unexpected sign");
            return __zero;
        }
        
        int integer:: compare( const integer &lhs, const int64_t rhs ) throw()
        {
            const word2mpz w(rhs);
            return compare(lhs,w.z);
        }
        
        int integer::compare( const int64_t lhs, const integer &rhs ) throw()
        {
            const word2mpz w(lhs);
            return compare(w.z,rhs);
        }
        
    }
}
