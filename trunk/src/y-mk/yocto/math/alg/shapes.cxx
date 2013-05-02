#include "yocto/math/alg/shapes.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"

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
        W(3,numeric<real_t>::zero),
        V(3,3),
        A(3,numeric<real_t>::zero)
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
            if( svd<real_t>::build(S, W, V) )
            {
                if(!svd<real_t>:: truncate(W, numeric<real_t>::ftol ) )
                {
                    ans = true;
                    //==============================================================
                    // solve
                    //==============================================================
                    //std::cerr << "W=" << W << std::endl;
                    svd<real_t>::solve(S, W, V, Q, A);
                    //std::cerr << "A=" << A << std::endl;
                    
                    //==============================================================
                    // deduce parameters
                    //==============================================================
                    C.x = A[1]/2;
                    C.y = A[2]/2;
                    const real_t R2 = A[3] + C.norm2();
                    R = R2 > 0 ? Sqrt(R2) : 0;
                }
            }
            
            reset();
            return ans;
        }
        
        template <>
        fit_ellipse<real_t>:: ~fit_ellipse() throw() {}
        
        template <>
        fit_ellipse<real_t>:: fit_ellipse() :
        Sqq(3,3),
        Sqz(3,3),
        Szz(3,3),
        C(3,3)
        {
            C.ld1();
            C.ldz();
            C[2][2] = -1;
            C[1][3] = 2;
            C[3][1] = 2;
        }
        
        template <>
        void fit_ellipse<real_t>:: reset() throw()
        {
            Sqq.ldz();
            Sqz.ldz();
            Szz.ldz();
        }
        
        template <>
        void fit_ellipse<real_t>::append( real_t x, real_t y) throw()
        {
            const real_t  q[3] = { x*x, x*y, y*y };
            const real_t  z[3] = {   x,   y,   1 };
            const real_t *Z=z-1;
            const real_t *Q=q-1;
            
            for(size_t i=1; i <=3; ++i )
            {
                for(size_t j=1; j<= 3; ++j)
                {
                    Sqq[i][j] += Q[i] * Q[j];
                    Sqz[i][j] += Q[i] * Z[j];
                    Szz[i][j] += Z[i] * Z[j];
                }
            }
        }
        
        template <>
        bool fit_ellipse<real_t>:: solve()
        {
            std::cerr << "Sqq=" << Sqq << std::endl;
            std::cerr << "Sqz=" << Sqz << std::endl;
            std::cerr << "Szz=" << Szz << std::endl;
            std::cerr << "beta=-inv(Szz)*(Sqz')" << std::endl;
            std::cerr << "S=Sqq+Sqz*beta" << std::endl;
            std::cerr << "C=" << C << std::endl;
            std::cerr << "M=inv(C)*S" << std::endl;
            std::cerr << "[gevec,geval]=eig(M)" << std::endl;
            std::cerr << "function [val] = F(A,B) val=A'*" << Sqq << "*A + 2 * A'*" << Sqz << "*B + B'*" << Szz << "*B; endfunction" << std::endl;
            return true;
        }
        
    }
    
}
