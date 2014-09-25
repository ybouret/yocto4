#ifndef YOCTO_FAME_INTEGRATE_INCLUDED
#define YOCTO_FAME_INTEGRATE_INCLUDED 1

#include "yocto/fame/array3d.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        
        template <typename T, typename U, typename FUNC, typename PARAM>
        inline U int1D(const array1D<T> &x, const array1D<U> &f, FUNC &F, const PARAM &param )
        {
            U ans(0);
            assert(x.same_layout_than(f));
            for(unit_t i=x.lower;i<x.upper;++i)
            {
                ans += (x[i+1]-x[i])*(F(f[i],param)+F(f[i+1],param));
            }
            return ans/2;
        }

        template <typename T, typename U, typename FUNC>
        inline U int1D(const array1D<T> &x, const array1D<U> &f, FUNC &F )
        {
            U ans(0);
            assert(x.same_layout_than(f));
            for(unit_t i=x.lower;i<x.upper;++i)
            {
                ans += (x[i+1]-x[i])*(F(f[i])+F(f[i+1]));
            }
            return ans/2;
        }
        
        template <typename T>
        inline T identity(T X)  throw()
        {
            return X;
        }
        
        template <typename T, typename U>
        inline U int1D(const array1D<T> &x, const array1D<U> &f)
        {
            return int1D(x,f,identity<U>);
        }
        
    }
    
}

#endif

