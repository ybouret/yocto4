#ifndef PYCK_PAIR_INCLUDED
#define PYCK_PAIR_INCLUDED 1

#include "./os.hpp"

template <typename T,typename U>
class Pair
{
public:
    T first;
    U second;
    
    inline  Pair() : first(), second() {}
    inline ~Pair() throw() {}
    inline  Pair( const T v1, const U v2 ) : first(v1), second(v2) {}
    inline  Pair( const Pair &p ) : first(p.first), second(p.second) {}
    inline  Pair & operator=( const Pair &p )
    {
        first  = p.first;
        second = p.second;
        return *this;
    }
    
};

#endif
