#include "yocto/math/alg/shapes.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/math/kernel/diag.hpp"

#include "yocto/exception.hpp"

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
        void fit_circle<real_t>:: solve( real_t &R, v2d<real_t> &C )
        {
            static const char fn[] = "fit_circle";
            
            R = C.x = C.y  = 0;
            if( !svd<real_t>::build(S, W, V) )
            {
                throw exception("%s(no SVD: bad points)", fn);
            }
            
            if( svd<real_t>:: truncate(W, numeric<real_t>::ftol ) > 0 )
            {
                throw exception("%s(singular distribution)",fn);
            }
            
            
            //==================================================================
            // solve
            //==================================================================
            svd<real_t>::solve(S, W, V, Q, A);
            
            //==================================================================
            // deduce parameters
            //==================================================================
            C.x = A[1]/2;
            C.y = A[2]/2;
            const real_t R2 = A[3] + C.norm2();
            R = R2 > 0 ? Sqrt(R2) : 0;
            
            reset();
            
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
            C.ldz();
            
            C.ld1();
            //C[2][2] = 0.5;
            
            if(true)
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
        void fit_conic<real_t>:: solve(conic_type t)
        {
            static const char fn[] = "fit_conic";
            
            C.ldz();
            switch(t)
            {
                case conic_generic:
                    C[1][1] = 1;
                    C[2][2] = 0.5;
                    C[3][3] = 1;
                    break;
                    
                case conic_ellipse:
                    C[2][2] = -1;
                    C[1][3] = 2;
                    C[3][1] = 2;
                    break;
            }
            
            //==================================================================
            // compute the system matrix
            //==================================================================
            //std::cerr << "Sqq = " << Sqq << std::endl;
            //std::cerr << "Sqz = " << Sqz << std::endl;
            //std::cerr << "Szz = " << Szz << std::endl;
            lu<real_t> LU(3);
            
            matrix<real_t> iSzz(Szz);
            //------------------------------------------------------------------
            // iSzz <- inv(Szz)
            //------------------------------------------------------------------
            if( !LU.build(iSzz) )
            {
                throw exception("%s(invalid points)",fn);
            }
            
            //------------------------------------------------------------------
            // beta <- transpose(Sqz)
            //------------------------------------------------------------------
            matrix<real_t> beta(Sqz,matrix_transpose);
            
            //------------------------------------------------------------------
            // beta <- inv(Szz)*transpose(Sqz)
            //------------------------------------------------------------------
            LU.solve(iSzz, beta);
            
            //------------------------------------------------------------------
            // M <- Sqq - Sqz * beta
            //------------------------------------------------------------------
            matrix<real_t> M(3,3);
            mkl::mul(M, Sqz, beta);
            for(size_t i=3;i>0;--i)
            {
                for(size_t j=3;j>0;--j)
                {
                    M[i][j] = Sqq[i][j] - M[i][j];
                }
            }
            
            //------------------------------------------------------------------
            // I <- inv(C)
            //------------------------------------------------------------------
            matrix<real_t> I(C);
            std::cerr << "C=" << I << std::endl;
            if( !LU.build(I) )
            {
                throw exception("%s(invalid constraint)", fn);
            }
            
            //------------------------------------------------------------------
            // M <- inv(C) * M
            //------------------------------------------------------------------
            LU.solve(I,M);
            //std::cerr.precision(15);
            
            //------------------------------------------------------------------
            // find eigenvalues
            //------------------------------------------------------------------
            std::cerr << "M=" << M << std::endl;
            vector<real_t> wr(3,0);
            vector<real_t> wi(3,0);
            size_t         nr=0;
            {
                matrix<real_t> MM = M;
                if( !diag<real_t>::eig(MM, wr, wi, nr) )
                {
                    throw exception("%s(can find eigenvalues)", fn);
                }
            }
            
            if( nr <=0 )
            {
                throw exception("%s(no real eigenvalue)", fn); // shouldn't happen in dim=3
            }
            
            //------------------------------------------------------------------
            // find eigenvectors
            //------------------------------------------------------------------
            matrix<real_t> evec(nr,3);
            diag<real_t>::eigv(evec, M, wr);
            
            //------------------------------------------------------------------
            // find acceptable eigenvectors
            //------------------------------------------------------------------
            size_t na = 0;
            size_t ia[3] = {0};
            real_t wa[3] = {0};
            for(size_t i=1; i <= 3; ++i)
            {
                const array<real_t> &v = evec[i];
                mkl::mul(wi,C,v);
                const real_t tmp = mkl::dot(wi,v);
                if(tmp>0)
                {
                    ia[na] = i;
                    wa[na] = tmp;
                    std::cerr << "#acceptable_" << ia[na] << ": " << v << "; #w=" << wa[na] << std::endl;
                    ++na;
                }
            }
            
            if( na <= 0 )
            {
                throw exception("%s(no acceptable eigenvalue)",fn);
            }
            
            //------------------------------------------------------------------
            // keep the eigenvector with the smallest eigenvalue
            //------------------------------------------------------------------
            const size_t  i0 = ia[0];
            array<real_t> &A = evec[i0];
            {
                const real_t fac = REAL(1.0)/Sqrt(wa[0]);
                for(size_t i=3;i>0;--i) A[i] *= fac;
            }
            std::cerr << "A=" << A << std::endl;
            
            //------------------------------------------------------------------
            // compute the second part
            //------------------------------------------------------------------
            array<real_t> &B = wi;
            mkl::mul(B,beta,A);
            for(size_t i=3;i>0;--i) B[i] = -B[i];
            std::cerr << "B=" << B << std::endl;
            
            //------------------------------------------------------------------
            // reduce the quadratic form
            //------------------------------------------------------------------
            
            
            reset();
        }
        
    }
    
}
