#include "yocto/math/alg/shapes.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

#include "yocto/math/kernel/svd.hpp"

#include "yocto/exception.hpp"

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
        Q(3,numeric<real_t>::zero)
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
        void fit_circle<real_t>:: solve( real_t &R, v2d<real_t> &C ) const
        {
            static const char fn[] = "fit_circle";
            
            R = C.x = C.y  = 0;
            matrix<real_t> U(S);
            matrix<real_t> V(3,3);
            vector<real_t> W(3,numeric<real_t>::zero);
            if( !svd<real_t>::build(U, W, V) )
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
            vector<real_t> A(3,numeric<real_t>::zero);
            svd<real_t>::solve(U, W, V, Q, A);
            
            //==================================================================
            // deduce parameters
            //==================================================================
            C.x = A[1]/2;
            C.y = A[2]/2;
            const real_t R2 = A[3] + C.norm2();
            R = R2 > 0 ? Sqrt(R2) : 0;
        }
        
    }
    
}

#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/kernel/diag.hpp"


namespace yocto
{
    namespace math
    {
        typedef algebra<real_t> mkl;
        
        template <>
        fit_conic<real_t>:: ~fit_conic() throw() {}
        
        template <>
        fit_conic<real_t>:: fit_conic() :
        Sqq(3,3),
        Sqz(3,3),
        Szz(3,3)
        {
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
        void fit_conic<real_t>:: solve(conic_type t, array<real_t> &param) const
        {
            static const char fn[] = "fit_conic";
            
            assert(param.size()>=6);
            for(size_t i=6;i>0;--i) param[i] = 0;
            matrix<real_t> C(3,3);
            switch(t)
            {
                case conic_generic:
                    C[1][1] =   1;
                    C[2][2] = 0.5;
                    C[3][3] =   1;
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
            
            matrix<real_t> iSzz(Szz);
            //------------------------------------------------------------------
            // iSzz <- inv(Szz)
            //------------------------------------------------------------------
            if( !crout<real_t>::build(iSzz) )
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
            crout<real_t>::solve(iSzz, beta);
            
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
            //std::cerr << "C=" << I << std::endl;
            if( !crout<real_t>::build(I) )
            {
                throw exception("%s(invalid constraint)", fn);
            }
            
            //------------------------------------------------------------------
            // M <- inv(C) * M
            //------------------------------------------------------------------
            crout<real_t>::solve(I,M);
            //std::cerr.precision(15);
            
            //------------------------------------------------------------------
            // find eigenvalues
            //------------------------------------------------------------------
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
                    //std::cerr << "#acceptable_" << ia[na] << ": " << v << "; #w=" << wa[na] << std::endl;
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
            //std::cerr << "A=" << A << std::endl;
            
            //------------------------------------------------------------------
            // compute the second part
            //------------------------------------------------------------------
            array<real_t> &B = wi;
            mkl::mul(B,beta,A);
            for(size_t i=3;i>0;--i) B[i] = -B[i];
            //std::cerr << "B=" << B << std::endl;
            
            param[1] = A[1];
            param[2] = A[2];
            param[3] = A[3];
            
            param[4] = B[1];
            param[5] = B[2];
            param[6] = B[3];
            
        }
        
    }
    
}


#include "yocto/math/kernel/jacobi.hpp"

namespace yocto
{
    
    namespace math
    {
        
        
        template <>
        void fit_conic<real_t>:: reduce(v2d<real_t>   &center,
                                        v2d<real_t>   &radius,
                                        m2d<real_t>   &rotation,
                                        array<real_t> &param )
        {
            assert(param.size()>=6);
            matrix<real_t> S(2,2);
            const real_t &a = param[1];
            const real_t &b = param[2];
            const real_t &c = param[3];
            const real_t &d = param[4];
            const real_t &e = param[5];
            const real_t &f = param[6];
            
        BUILD_S:
            S[1][1] = a;
            S[2][2] = c;
            S[1][2] = S[2][1] = b/2;
            //std::cerr << "S=" << S << std::endl;
            matrix<real_t> Q(2,2);
            vector<real_t> lam(2,numeric<real_t>::zero);
            if( !jacobi<real_t>::build(S,lam,Q) )
                throw exception("fit_conic::reduce(invalid parameters)");
            if( lam[1] <0 && lam[2] < 0)
            {
                // change sign
                for(size_t i=1; i <=6; ++i ) param[i] = -param[i];
                goto BUILD_S;
            }
            
            if( lam[1]>0 && lam[2]>0)
            {
                //==============================================================
                // ellipse
                //==============================================================

                //--------------------------------------------------------------
                // greater axis => X
                // lam[1] <= lam[2]
                //--------------------------------------------------------------
                if(lam[1]>lam[2])
                {
                    cswap(lam[1],lam[2]);
                    Q.swap_cols(1, 2);
                }
                //std::cerr << "#found ellipse" << std::endl;
                //std::cerr << "lam=" << lam << std::endl;
                //std::cerr << "Q="   << Q   << std::endl;
                const real_t lamX = lam[1];
                const real_t lamY = lam[2];
                rotation.ex.x = Q[1][1];
                rotation.ex.y = Q[2][1];
                
                rotation.ey.x = Q[1][2];
                rotation.ey.y = Q[2][2];
                
                //--------------------------------------------------------------
                // ( D )                  ( d )
                // (   ) = transpose(Q) * (   )
                // ( E )                  ( e )
                //--------------------------------------------------------------
                const real_t D   = (Q[1][1] * d + Q[2][1] * e);
                const real_t E   = (Q[1][2] * d + Q[2][2] * e);
                
                const real_t Dp  = D/lamX;
                const real_t Ep  = E/lamY;
                
                center.x = -REAL(0.5) * (Q[1][1] * Dp + Q[1][2] * Ep);
                center.y  = -REAL(0.5) * (Q[2][1] * Dp + Q[2][2] * Ep);
                //std::cerr << "center=" << center << std::endl;
                const real_t rhs = REAL(0.25) * ( D*Dp + E*Ep) - f;
                //std::cerr << "rhs=" <<rhs << std::endl;
                const real_t R2  = rhs > 0 ? rhs : 0;
                
                radius.x  = Sqrt(R2/lamX);
                radius.y =  Sqrt(R2/lamY);
                
                //std::cerr << "radius="<< radius << std::endl;
                //std::cerr << "rot.ex=" << rotation.ex << std::endl;
                //std::cerr << "rot.ey=" << rotation.ey << std::endl;
                return;
            }
         
            throw exception("can't reduce conic!");
        }
        
        
    }
}
