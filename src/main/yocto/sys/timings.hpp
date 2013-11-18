#ifndef YOCTO_SYS_TIMINGS_INCLUDED
#define YOCTO_SYS_TIMINGS_INCLUDED 1

#include "yocto/sys/wtime.hpp"

namespace yocto
{
    
    class timings : public wtime
    {
    public:
        explicit timings();
        virtual ~timings() throw();
        
        size_t   cycle; //!< #cycles repeated
        double   run_t; //!< run time
        double   speed; //!< final speed
        
        inline void reset() { calibrate(); cycle = 0; speed=0; run_t=0; start=ticks();}
        inline bool probe( const double duration )
        {
            ++cycle;
            const uint64_t current  = ticks();
            run_t = (*this)(current-start);
            if( run_t > duration )
            {
                speed = double(cycle)/run_t;
                return false;
            }
            return true;
        }
        
        
        
    private:
        uint64_t start; //!< when timings started
        YOCTO_DISABLE_COPY_AND_ASSIGN(timings);
    };
    
#define YOCTO_TIMINGS(TMX,DURATION,CODE) \
do {                          \
TMX.reset();                  \
do { CODE; }                  \
while( TMX.probe(DURATION) ); \
} while(false)
    
    
}

#endif

