#include "yocto/mpa/word2mpz.hpp"

namespace yocto
{
    namespace mpa
    {
        
        integer integer::add(const  integer &lhs, const  integer &rhs)
        {
            switch(lhs.s)
            {
                    //__________________________________________________________
                    // lhs
                case __negative:
                    //__________________________________________________________
                    switch (rhs.s) {
                        case __negative:
                        {
                            const natural sum = lhs.n + rhs.n;
                            return integer( __negative, sum );
                        }
                            
                        case __zero:
                            return lhs;
                            
                        case __positive:
                            switch( int2sign( natural::compare(lhs.n, rhs.n)))
                        {
                            case __negative:
                            {
                                const natural dif = rhs.n - lhs.n;
                                return integer(__positive,dif);
                            }
                            case __zero:
                                return integer();
                                
                            case __positive:
                            {
                                const natural dif = lhs.n - rhs.n;
                                return integer(__negative,dif);
                            }
                        }
                    }
                    
                    //__________________________________________________________
                    // lhs
                case __zero:
                    //__________________________________________________________
                    return rhs;
                    
                    //__________________________________________________________
                    // lhs
                case __positive:
                    //__________________________________________________________
                    switch (rhs.s) {
                        case __negative:
                            switch( int2sign( natural::compare(lhs.n, rhs.n)))
                        {
                            case __negative:
                            {
                                const natural dif = rhs.n - lhs.n;
                                return integer(__negative,dif);
                            }
                                
                            case __zero:
                                return integer();
                                
                            case __positive:
                            {
                                const natural dif = lhs.n - rhs.n;
                                return integer(__positive,dif);
                            }
                        }
                            
                        case __zero:
                            return lhs;
                            
                        case __positive:
                        {
                            const natural sum = lhs.n + rhs.n;
                            return integer( __positive, sum );
                        }
                    }
                    
            }
        }
        
        
        integer operator+(const integer &lhs, const integer &rhs)
        {
            return integer::add(lhs,rhs);
        }
        
    }
}
