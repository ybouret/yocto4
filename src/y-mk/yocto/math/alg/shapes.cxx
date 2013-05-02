#include "yocto/math/alg/shapes.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

#include "yocto/math/kernel/svd.hpp"

namespace yocto
{
    namespace math
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        fit_shape<real_t>:: fit_shape() throw() {}
        
        template <>
        fit_shape<real_t>:: ~fit_shape() throw() {}
        
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        fit_circle<real_t>:: ~fit_circle() throw() {}
        
        template <>
        fit_circle<real_t>:: fit_circle() :
        S(3,3),
        Q(3,numeric<real_t>::zero),
        LU(3)
        {
            
        }
        
        template <>
        void fit_circle<real_t>:: reset() throw()
        {
            S.ldz();
            for(size_t i=1;i<=Q.size();++i) Q[i] = 0;
        }
        
        template <>
        void fit_circle<real_t>:: append( real_t x, real_t y) throw()
        {
            const real_t x2 = x*x;
            const real_t y2 = y*y;
            const real_t xy = x*y;
            const real_t z  = x2+y2;
            S[1][1] += x2;
            S[2][2] += y2;
           
            S[1][2] += xy;
            S[2][1]  = S[1][2];
            
            S[1][3] += x;
            S[3][1]  = S[1][3];
            
            S[2][3] += y;
            S[3][2]  = S[2][3];
            
            ++S[3][3];
            
            Q[1] += z * x;
            Q[2] += z * y;
            Q[3] += z;
            
        }
        
        template <>
        bool fit_circle<real_t>:: solve( real_t &R, v2d<real_t> &C ) throw()
        {
            bool ans = false;
            R = C.x = C.y  = 0;
            if( LU.build(S) )
            {
                ans = true;
                //==============================================================
                // solve
                //==============================================================
                LU.solve(S,Q);
                std::cerr << "A=" << Q << std::endl;
                //==============================================================
                // deduce parameters
                //==============================================================
                C.x = Q[1]/2;
                C.y = Q[2]/2;
                const real_t R2 = Q[3] + C.norm2();
                R = R2 > 0 ? Sqrt(R2) : 0;
            }
            
            reset();
            return ans;
        }
        
#if 0
        template <>
        ellipse<real_t>:: ~ellipse() throw() {}
        
        template <>
        ellipse<real_t>:: ellipse() :
        S(6,6),
        C(6,6)
        {
            C[1][3] = C[3][1] = 2;
            C[2][2] = -1;
        }
        
        template <>
        void ellipse<real_t>:: reset() throw()
        {
            S.ldz();
        }
        
        template <>
        void ellipse<real_t>::append( real_t x, real_t y) throw()
        {
            const real_t  z[6] = { x*x, x*y, y*y, x, y , 1};
            const real_t *Z=z-1;
            
            for(size_t i=1; i <=6; ++i )
            {
                for(size_t j=1; j<= 6; ++j)
                    S[i][j] += Z[i] * Z[j];
            }
        }
#endif
        
    }
    
}
