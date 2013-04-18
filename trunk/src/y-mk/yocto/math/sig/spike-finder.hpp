#ifndef YOCTO_MATH_SIG_SPIKE_FINDER_INCLUDED
#define YOCTO_MATH_SIG_SPIKE_FINDER_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/container/sequence.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class spike_info
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            size_t idx;
            type   pos;
            type   val;
            
            inline  spike_info() throw() :
            idx(0), pos(0), val(0)
            {
                
            }
            
            inline ~spike_info() throw() {}
            
            spike_info( const spike_info &s) throw() :
            idx(s.idx), pos(s.pos), val(s.val)
            {
            }
            
            spike_info &operator=( const spike_info &s) throw()
            {
                idx = s.idx;
                pos = s.pos;
                val = s.val;
                return *this;
            }
            
            
        };
        
        template <typename T>
        struct spike_finder
        {
            
            static
            size_t detect(sequence< spike_info<T> > &spikes,
                          const size_t               count,
                          const array<T> &x,
                          const array<T> &y );
            
        };
        
    }
}

#endif

