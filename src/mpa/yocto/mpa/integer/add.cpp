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
        
        integer integer::add(const  integer &lhs, const int64_t rhs)
        {
            const word2mpz w(rhs);
            return add(rhs,w.z);
        }
        
        integer operator+(const integer &lhs, const int64_t  rhs)
        {
            return integer::add(lhs,rhs);
        }
        
        integer operator+(const int64_t  lhs, const integer &rhs)
        {
            return integer::add(rhs,lhs);
        }
        
        
        integer & integer:: operator+=( const integer &rhs )
        {
            integer tmp = add(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator+=( const int64_t rhs )
        {
            integer tmp = add(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator++()
        {
            static const word2mpz one(1);
            integer tmp = add(*this,one.z);
            xch(tmp);
            return *this;
        }
        
        integer integer:: operator++ (int)
        {
            static const word2mpz one(1);
            const integer save(*this);
            integer tmp = add(*this,one.z);
            xch(tmp);
            return save;
        }
        
    }
}
