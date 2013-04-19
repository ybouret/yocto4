#ifndef YOCTO_MATH_SIG_SPIKE_FINDER_INCLUDED
#define YOCTO_MATH_SIG_SPIKE_FINDER_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/container/sequence.hpp"
#include "yocto/comparator.hpp"

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
            
            inline spike_info( const spike_info &s) throw() :
            idx(s.idx), pos(s.pos), val(s.val)
            {
            }
            
            inline spike_info &operator=( const spike_info &s) throw()
            {
                idx = s.idx;
                pos = s.pos;
                val = s.val;
                return *this;
            }
            
            static inline
            int compare_by_pos( const spike_info &lhs, const spike_info &rhs) throw()
            {
                return __compare(lhs.pos,rhs.pos);
            }
            
            static inline
            int compare_by_val( const spike_info &lhs, const spike_info &rhs) throw()
            {
                return __compare(lhs.val,rhs.val);
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

