#include "yocto/math/alg/shapes.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/math/kernel/diag.hpp"

namespace yocto
{
    namespace math
    {
        
        typedef algebra<real_t> mkl;
        
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
        fit_conic<real_t>:: ~fit_conic() throw() {}
        
        template <>
        fit_conic<real_t>:: fit_conic() :
        Sqq(3,3),
        Sqz(3,3),
        Szz(3,3),
        C(3,3)
        {
            C.ld1();
            //C[2][2] = 0.5;
            
            if(false)
            {
                C.ldz();
                C[2][2] = -1;
                C[1][3] = 2;
                C[3][1] = 2;
            }
        }
        
        template <>
        void fit_conic<real_t>:: reset() throw()
        {
            Sqq.ldz();
            Sqz.ldz();
            Szz.ldz();
        }
        
        template <>
        void fit_conic<real_t>::append( real_t x, real_t y) throw()
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
        bool fit_conic<real_t>:: solve()
        {
#if 0
            std::cerr << "Sqq=" << Sqq << std::endl;
            std::cerr << "Sqz=" << Sqz << std::endl;
            std::cerr << "Szz=" << Szz << std::endl;
            std::cerr << "beta=inv(Szz)*(Sqz')" << std::endl;
            std::cerr << "S=Sqq-Sqz*beta" << std::endl;
            std::cerr << "C=" << C << std::endl;
            std::cerr << "M=inv(C)*S" << std::endl;
            std::cerr << "[gevec,geval]=eig(M)" << std::endl;
            std::cerr << "function [val] = F(A,B) val=A'*" << Sqq << "*A + 2 * A'*" << Sqz << "*B + B'*" << Szz << "*B; endfunction" << std::endl;
#endif
            
            //==================================================================
            // compute the system matrix
            //==================================================================
            std::cerr << "Sqq = " << Sqq << std::endl;
            std::cerr << "Sqz = " << Sqz << std::endl;
            std::cerr << "Szz = " << Szz << std::endl;
            lu<real_t> LU(3);
            if( !LU.build(Szz) )
                return false;
            
            matrix<real_t> beta(Sqz,matrix_transpose);
            std::cerr << "Sqzp= " << beta << std::endl;
            LU.solve(Szz, beta);
            std::cerr << "beta=" << beta << std::endl;
            matrix<real_t> M(3,3);
            mkl::mul(M, Sqz, beta);
            for(size_t i=3;i>0;--i)
            {
                for(size_t j=3;j>0;--j)
                {
                    M[i][j] = Sqq[i][j] - M[i][j];
                }
            }
            
            matrix<real_t> I(C);
            std::cerr << "C=" << I << std::endl;
            if( !LU.build(I) )
                return false;
            LU.solve(I,M);
            
            std::cerr << "M0=" << M << std::endl;
            
            //==================================================================
            // compute the eigenvalues
            //==================================================================
            vector<real_t> wr(3, numeric<real_t>::zero );
            vector<real_t> wi(3, numeric<real_t>::zero );
            size_t         nr = 0;
            {
                matrix<real_t> MM(M);
                diag<real_t>::HessenbergBalance(MM);
                std::cerr << "MB=" << MM << std::endl;
                diag<real_t>::HessenbergReduce(MM);
                std::cerr << "MR=" << MM << std::endl;
                
                if( !diag<real_t>::HessenbergQR(MM, wr, wi, nr) )
                    return false;
            }
            std::cerr << "nr=" << nr << std::endl;
            if( nr <=0 )
                return false; // shouldn't happen
            std::cerr << "wr=" << wr << std::endl;
            std::cerr << "wi=" << wi << std::endl;
            
            //==================================================================
            // compute the real possible eigenvectors
            //==================================================================
            matrix<real_t> ev(nr,3);
            diag<real_t>::eigv(ev, M, wr);
            std::cerr << "wr=" << wr << std::endl;
            std::cerr << "ev=" << ev << "'" << std::endl;
            
            //==================================================================
            // compute the acceptable eig
            //==================================================================
            size_t ok[3]  = { 0, 0, 0 };
            real_t vn[3]  = { 0, 0, 0 };
            size_t na = 0;
            vector<real_t> y(3, numeric<real_t>::zero );
            for(size_t i=1; i <= 3; ++i )
            {
                const array<real_t> &v = ev[i];
                mkl::mul(y,C,v);
                const real_t tmp = mkl::dot(v,y);
                if(tmp>0)
                {
                    ok[na] = i;
                    vn[na] = Sqrt(tmp);
                    std::cerr << "Accept : " << v << " / " << vn[na] << std::endl;
                    ++na;
                    
                }
            }
            
            return true;
        }
        
    }
    
}
