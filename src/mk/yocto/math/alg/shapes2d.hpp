#ifndef YOCTO_MK_ALG_SHAPES2D_INCLUDED
#define YOCTO_MK_ALG_SHAPES2D_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/diag.hpp"
#include "yocto/math/core/symdiag.hpp"
#include "yocto/math/point2d.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class FitShape2D
        {
        public:
            inline virtual ~FitShape2D() throw() {}

            virtual void reset() throw() = 0;
            virtual void append(const T x, const T y) = 0;
            inline  void append(const point2d<T> &p) { append(p.x,p.y); }

        protected:
            inline explicit FitShape2D() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitShape2D);
        };


        template <typename T>
        class FitCircle
        {
        public:
            inline explicit FitCircle() throw() :
            data()
            {
            }

            inline virtual ~FitCircle() throw() {}

            YOCTO_SEXTUPLE_DECL(Data,T,x,T,y,T,xx,T,yy,T,xy,T,z);
            YOCTO_SEXTUPLE_END();

            inline virtual void reset() throw() { data.free(); }
            inline void append(const T x, const T y)
            {
                const T    xx = x*x;
                const T    yy = y*y;
                const Data tmp(x,y,xx,yy,x*y,xx+yy);
                data.push_back(tmp);
            }

            vector<Data> data;

#define YOCTO_MK_FIT_CIRCLE(DEST,EXPR) do { \
for(size_t i=N;i>0;--i)                     \
{                                           \
const Data &value = data[i];                \
u[i] = EXPR;                                \
}                                           \
quicksort(u);                               \
T sum = 0;                                  \
for(size_t i=1;i<=N;++i) sum += u[i];       \
DEST = sum;                                 \
} while(false)

            inline void compute( point2d<T> &center, T &radius )
            {
                assert(data.size()>0);
                const size_t N = data.size();
                matrix<T>    S(3);
                vector<T>    Q(3);
                vector<T>    u(N);
                S[3][3] = N;
                YOCTO_MK_FIT_CIRCLE(S[1][3]=S[3][1], value.x );
                YOCTO_MK_FIT_CIRCLE(S[2][3]=S[3][2], value.y );
                YOCTO_MK_FIT_CIRCLE(S[1][1],         value.xx);
                YOCTO_MK_FIT_CIRCLE(S[2][2],         value.yy);
                YOCTO_MK_FIT_CIRCLE(S[1][2]=S[2][1], value.xy);
                YOCTO_MK_FIT_CIRCLE(Q[1],value.z*value.x);
                YOCTO_MK_FIT_CIRCLE(Q[2],value.z*value.y);
                YOCTO_MK_FIT_CIRCLE(Q[3],value.z        );

                if( !LU<T>::build(S) )
                {
                    throw imported::exception("FitCircle","singular distribution");
                }

                LU<T>::solve(S,Q);

                const T a = Q[1];
                const T b = Q[2];
                const T c = Q[3];
                center.x = T(0.5)*a;
                center.y = T(0.5)*b;
                radius  = Sqrt( max_of<T>(0,c+center.norm2()) );
            }
#undef YOCTO_MK_FIT_CIRCLE

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitCircle);
        };
    }
}

namespace yocto
{
    namespace math
    {

        enum FitConicType
        {
            FitConicGeneric,
            FitConicEllipse
        };

        template <typename T>
        class FitConic
        {
        public:

            YOCTO_PENTUPLE_DECL(Data,T,xx,T,xy,T,yy,T,x,T,y);
            YOCTO_PENTUPLE_END();

            inline virtual void reset() throw() { data.free(); }
            inline virtual void append(const T x,const T y)
            {
                const Data tmp(x*x,x*y,y*y,x,y);
                data.push_back(tmp);
            }

            inline explicit FitConic() throw() : data()
            {

            }

            inline virtual ~FitConic() throw()
            {
            }


            vector<Data> data;

#define YOCTO_MK_FIT_CONIC(DEST,EXPR) do {  \
for(size_t i=N;i>0;--i)                     \
{                                           \
const Data &value = data[i];                \
u[i] = EXPR;                                \
}                                           \
quicksort(u);                               \
T sum = 0;                                  \
for(size_t i=1;i<=N;++i) sum += u[i];       \
DEST = sum;                                 \
} while(false)
            virtual void compute(const FitConicType t, array<T> &param)
            {
                assert(param.size()>=6);
                assert(data.size()>0);
                static const char fn[] = "FitConic";
                //______________________________________________________________
                //
                // C=constraint matrix
                //______________________________________________________________
                matrix<T>   C(3,3);
                switch(t)
                {
                    case FitConicGeneric:
                        C[1][1] =   1;
                        C[2][2] = 0.5;
                        C[3][3] =   1;
                        break;

                    case FitConicEllipse:
                        C[2][2] = -1;
                        C[1][3] = 2;
                        C[3][1] = 2;
                        break;
                }

                //______________________________________________________________
                //
                // I=inv(C)
                //______________________________________________________________
                matrix<T> I(C,YOCTO_MATRIX_ENLARGE);
                if( !LU<T>::build(I) )
                {
                    throw imported::exception(fn,"singular constraints matrix");
                }


                const size_t N = data.size();
                matrix<T> Sqq(3), Sqz(3), Szz(3);
                vector<T> u(N);

                //______________________________________________________________
                //
                // build Sqq matrices , sum of Q*Q', Q = [xx xy yy]'
                //______________________________________________________________
                YOCTO_MK_FIT_CONIC(Sqq[1][1],          value.xx*value.xx);
                YOCTO_MK_FIT_CONIC(Sqq[1][2]=Sqq[2][1],value.xx*value.xy);
                YOCTO_MK_FIT_CONIC(Sqq[1][3]=Sqq[3][1],value.xx*value.yy);

                YOCTO_MK_FIT_CONIC(Sqq[2][2],          value.xy*value.xy);
                YOCTO_MK_FIT_CONIC(Sqq[2][3]=Sqq[3][2],value.xy*value.yy);

                YOCTO_MK_FIT_CONIC(Sqq[3][3],          value.yy*value.yy);

                //______________________________________________________________
                //
                // build Szz matrix, sum of Z*Z', Z=[x y 1]'
                //______________________________________________________________
                YOCTO_MK_FIT_CONIC(Szz[1][1],          value.xx);
                YOCTO_MK_FIT_CONIC(Szz[1][2]=Szz[2][1],value.xy);
                YOCTO_MK_FIT_CONIC(Szz[1][3]=Szz[3][1],value.x);

                YOCTO_MK_FIT_CONIC(Szz[2][2],value.yy);
                YOCTO_MK_FIT_CONIC(Szz[2][3]=Szz[3][2],value.y);

                Szz[3][3] = N;

                //______________________________________________________________
                //
                // build Sqz matrix, sum of Q*Z'
                //______________________________________________________________
                YOCTO_MK_FIT_CONIC(Sqz[1][1],value.xx*value.x);
                YOCTO_MK_FIT_CONIC(Sqz[1][2],value.xx*value.y);
                YOCTO_MK_FIT_CONIC(Sqz[1][3],value.xx);

                YOCTO_MK_FIT_CONIC(Sqz[2][1],value.xy*value.x);
                YOCTO_MK_FIT_CONIC(Sqz[2][2],value.xy*value.y);
                YOCTO_MK_FIT_CONIC(Sqz[2][3],value.xy);

                YOCTO_MK_FIT_CONIC(Sqz[3][1],value.yy*value.x);
                YOCTO_MK_FIT_CONIC(Sqz[3][2],value.yy*value.y);
                YOCTO_MK_FIT_CONIC(Sqz[3][3],value.yy);

                if( !LU<T>::build(Szz) )
                {
                    throw imported::exception(fn,"singular distribution");
                }

                //______________________________________________________________
                //
                // build matrix Beta = inv(Szz)*Sqz'
                //______________________________________________________________
                matrix<T> Beta(Sqz,YOCTO_MATRIX_TRANSPOSE); //! Beta = inv(Szz)*Sqz'
                LU<T>::solve(Szz,Beta);

                //______________________________________________________________
                //
                // build matrix J = Sqq - Sqz*Beta
                //______________________________________________________________
                matrix<T> J(3,3);
                tao::mmul(J,Sqz,Beta);
                for(size_t i=3;i>0;--i)
                {
                    for(size_t j=3;j>0;--j)
                    {
                        J[i][j] = Sqq[i][j] - J[i][j];
                    }
                }

                //______________________________________________________________
                //
                // build matrix M = inv(C)*(Sqq - Sqz*Beta)
                //______________________________________________________________
                matrix<T> M(3);
                tao::mmul(M, I, J);

                //______________________________________________________________
                //
                // find eigenvalues
                //______________________________________________________________
                vector<T> wr(3);
                vector<T> wi(3);
                size_t    nr=0;
                {
                    matrix<T> MM(M);
                    if( !diag<T>::eig(MM, wr, wi, nr) )
                    {
                        throw imported::exception(fn,"cannot find eigenvalues");
                    }
                }

                if(nr<=0 )
                {
                    throw imported::exception(fn,"no real eigenvalue"); // shouldn't happen in dim=3
                }
                std::cerr << "nr=" << nr << std::endl;
                
                //______________________________________________________________
                //
                // find eigenvectors
                //______________________________________________________________
                matrix<T> evec(nr,3);
                diag<T>::eigv(evec, M, wr);

                //______________________________________________________________
                //
                // find acceptable eigenvectors
                //______________________________________________________________
                size_t na    = 0;
                size_t ia[3] = {0};
                T      wa[3] = {0};
                for(size_t i=1; i <= nr; ++i)
                {
                    const array<T> &v = evec[i];
                    tao::mul(wi,C,v);
                    const T tmp = tao::dot(wi,v);
                    if(tmp>0)
                    {
                        ia[na] = i;
                        wa[na] = tmp;
                        ++na;
                    }
                }

                if( na <= 0 )
                {
                    throw imported::exception(fn,"no acceptable eigenvalue");
                }

                //______________________________________________________________
                //
                // keep the eigenvector with the smallest eigenvalue
                //______________________________________________________________
                assert(ia[0]>=1);
                assert(ia[0]<=3);
                const size_t  i0 = ia[0];
                array<T>     &A  = evec[i0];
                {
                    const T fac = T(1)/Sqrt(wa[0]);
                    for(size_t i=3;i>0;--i) A[i] *= fac;
                }

                //______________________________________________________________
                //
                // compute the second part
                //______________________________________________________________
                vector<T> &B = wi;
                tao::mul(B,Beta,A);
                for(size_t i=3;i>0;--i) B[i] = -B[i];

                param[1] = A[1];
                param[2] = A[2];
                param[3] = A[3];

                param[4] = B[1];
                param[5] = B[2];
                param[6] = B[3];


            }

#undef YOCTO_MK_FIT_CONIC

            static inline void Reduce(point2d<T>   &center,
                                      point2d<T>   &radius,
                                      matrix<T>    &rotation,
                                      array<T>     &param)
            {
                static const char  fn[] = "FitConic::Reduce";
                static const size_t nvar = 6;
                assert(param.size()>=nvar);
                assert(rotation.rows==2);
                assert(rotation.cols==2);
                matrix<T> S(2);
                const T &a = param[1];
                const T &b = param[2];
                const T &c = param[3];
                const T &d = param[4];
                const T &e = param[5];
                const T &f = param[6];

            BUILD_S:
                S[1][1] = a;
                S[2][2] = c;
                S[1][2] = S[2][1] = b/2;
                matrix<T> &Q = rotation;
                vector<T> lam(2);

                if( !symdiag<T>::build(S,lam,Q) )
                    throw imported::exception(fn,"invalid parameters");

                if( lam[1] <0 && lam[2] < 0)
                {
                    // change sign
                    for(size_t i=1;i<=nvar; ++i ) param[i] = -param[i];
                    goto BUILD_S;
                }

                if( lam[1]>0 && lam[2]>0)
                {
                    //__________________________________________________________
                    //
                    // ellipse
                    //
                    //__________________________________________________________

                    //__________________________________________________________
                    //
                    // greater axis => X
                    // lam[1] <= lam[2]
                    //__________________________________________________________
                    if(lam[1]>lam[2])
                    {
                        cswap(lam[1],lam[2]);
                        Q.swap_cols(1,2);
                    }
                    const T lamX = lam[1];
                    const T lamY = lam[2];

                    //__________________________________________________________
                    // ( D )                  ( d )
                    // (   ) = transpose(Q) * (   )
                    // ( E )                  ( e )
                    //__________________________________________________________
                    const T D   = (Q[1][1] * d + Q[2][1] * e);
                    const T E   = (Q[1][2] * d + Q[2][2] * e);

                    const T Dp  = D/lamX;
                    const T Ep  = E/lamY;
                    
                    center.x  = -T(0.5) * (Q[1][1] * Dp + Q[1][2] * Ep);
                    center.y  = -T(0.5) * (Q[2][1] * Dp + Q[2][2] * Ep);
                    const T rhs = T(0.25) * ( D*Dp + E*Ep) - f;
                    const T R2  = (rhs > 0) ? rhs : 0;
                    
                    radius.x  = Sqrt(R2/lamX);
                    radius.y =  Sqrt(R2/lamY);
                    
                    return;
                }
                
                throw imported::exception(fn,"can't reduce this kind of conic");
                
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitConic);
        };
        
        
    }
}

#endif
