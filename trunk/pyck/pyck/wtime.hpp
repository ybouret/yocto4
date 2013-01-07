#ifndef PYCK_WTIME_INCLUDED
#define PYCK_WTIME_INCLUDED 1

#include "./os.hpp"

class WallTime
{
public:
    explicit WallTime() throw();
    virtual ~WallTime() throw();
    void   start();
    double query() const;
    static uint64_t Ticks();
    static uint32_t Seed();
    
private:
    WallTime( const WallTime & );
    WallTime &operator=( const WallTime & );
    
#if defined(PYCK_BSD)
    int64_t data[2];
#endif
    
#if defined(PYCK_WIN)
    int64_t      quad;
    long double  freq;
#endif

};

#endif
