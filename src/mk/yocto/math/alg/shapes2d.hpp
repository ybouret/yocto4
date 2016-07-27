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
            virtual void append(const T x, const T y, const T w=1) = 0;
            inline  void append(const point2d<T> &p,const T w=1) { append(p.x,p.y,w); }

        protected:
            inline explicit FitShape2D() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitShape2D);
        };


        template <typename T>
        class FitCircle : public FitShape2D<T>
        {
        public:
            inline explicit FitCircle() throw() :
            data()
            {
            }

            inline virtual ~FitCircle() throw() {}

            YOCTO_HEPTUPLE_DECL(YOCTO_TUPLE_TEMPLATE,Data,T,x,T,y,T,xx,T,yy,T,xy,T,z,T,w2);
            YOCTO_HEPTUPLE_END();

            inline virtual void reset() throw() { data.free(); }
            inline void append(const T x, const T y,const T w=1)
            {
                const T    xx = x*x;
                const T    yy = y*y;
                const Data tmp(x,y,xx,yy,x*y,xx+yy,w*w);
                data.push_back(tmp);
            }

            vector<Data> data;

#define YOCTO_MK_FIT_CIRCLE(DEST,EXPR) do { \
for(size_t i=N;i>0;--i)                     \
{                                           \
const Data &value = data[i];                \
const T     w2    = value.w2;               \
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
                YOCTO_MK_FIT_CIRCLE(S[3][3],         w2);
                YOCTO_MK_FIT_CIRCLE(S[1][3]=S[3][1], w2*value.x );
                YOCTO_MK_FIT_CIRCLE(S[2][3]=S[3][2], w2*value.y );
                YOCTO_MK_FIT_CIRCLE(S[1][1],         w2*value.xx);
                YOCTO_MK_FIT_CIRCLE(S[2][2],         w2*value.yy);
                YOCTO_MK_FIT_CIRCLE(S[1][2]=S[2][1], w2*value.xy);
                YOCTO_MK_FIT_CIRCLE(Q[1],w2*value.z*value.x);
                YOCTO_MK_FIT_CIRCLE(Q[2],w2*value.z*value.y);
                YOCTO_MK_FIT_CIRCLE(Q[3],w2*value.z        );

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
        class FitConic : public FitShape2D<T>
        {
        public:

            YOCTO_SEXTUPLE_DECL(YOCTO_TUPLE_TEMPLATE,Data,T,xx,T,xy,T,yy,T,x,T,y,T,w2);
            YOCTO_SEXTUPLE_END();

            inline virtual void reset() throw() { data.free(); }
            inline virtual void append(const T x,const T y,const T w=1)
            {
                const Data tmp(x*x,x*y,y*y,x,y,w*w);
                data.push_back(tmp);
            }

            inline explicit FitConic() throw() : data()
            {

            }

            inline virtual ~FitConic() throw()
            {
            }


            vector<Data> data;




            static inline T Eval(const array<T> &param, const T x, const T y) throw()
            {
                assert(param.size()>=6);
                return param[1] * x*x + param[2] * x*y + param[3] * y*y + param[4] * x + param[5] * y + param[6];
            }

            static inline void Reduce(point2d<T>     &center,
                                      point2d<T>     &radius,
                                      matrix<T>      &rotate,
                                      const array<T> &A)
            {
                static const char   fn[] = "FitConic::Reduce";
                assert(rotate.rows==2);
                assert(rotate.cols==2);
                assert(A.size()>=6);

                //______________________________________________________________
                //
                // prepare parameters
                //______________________________________________________________
                vector<double> param(6);
                for(size_t i=6;i>0;--i)
                {
                    param[i] = A[i];
                }
                const T &a = param[1];
                const T &b = param[2];
                const T &c = param[3];
                const T &d = param[4];
                const T &e = param[5];
                const T &f = param[6];

                matrix<T>  Q(2);
                matrix<T> &R = rotate;
                vector<T>  lam(2);

                //______________________________________________________________
                //
                // diagonalise quadratic part
                //______________________________________________________________
            BUILD_S:
                Q[1][1] = a;
                Q[2][2] = c;
                Q[1][2] = Q[2][1] = b/2;


                if( !symdiag<T>::build(Q,lam,R) )
                {
                    throw imported::exception(fn,"invalid parameters");
                }

                if( lam[1] <0 && lam[2] < 0)
                {
                    // change sign
                    for(size_t i=1;i<=6; ++i ) param[i] = -param[i];
                    goto BUILD_S;
                }

                
                const point2d<T> L(d,e);
                if( lam[1]>0 && lam[2]>0 )
                {

                    //__________________________________________________________
                    //
                    // smaller ev to 'X' axis => bigger radius
                    //__________________________________________________________
                    T lx = lam[1];
                    T ly = lam[2];
                    if(lx>ly)
                    {
                        cswap(lx,ly);
                        R.swap_cols(1,2);
                    }

                    //__________________________________________________________
                    //
                    // diag(lx,ly) = R'*Q*R
                    // inv(Q)      = R*diag(1/lx,1/ly)*R'
                    // center      = -0.5*inv(Q)*L = -0.5*R*diag(1/lx,1/ly)*R'*L
                    // L'*inv(Q)*L = (L'*R)*diag(1/lx,1/ly)*(R'*L)
                    //__________________________________________________________
                    point2d<T>  RtL;
                    tao::mul_trn(RtL,R,L);
                    const point2d<T> tmp(-RtL.x/(lx+lx),-RtL.y/(ly+ly));
                    tao::mul(center,R,tmp);

                    const T rhs = T(0.25) * (Square(RtL.x)/lx + Square(RtL.y)/ly)-f;
                    const T R2  = (rhs>0) ? rhs : 0;
                    radius.x = Sqrt(R2/lx);
                    radius.y = Sqrt(R2/ly);
                    return;
                }


                throw imported::exception(fn,"can't reduce this kind of conic");

            }

#define YOCTO_MK_FIT_CONIC(DEST,EXPR) do {  \
for(size_t i=N;i>0;--i)                     \
{                                           \
const Data &value = data[i];                \
const T     w2    = value.w2;               \
u[i] = EXPR;                                \
}                                           \
quicksort(u);                               \
T sum = 0;                                  \
for(size_t i=1;i<=N;++i) sum += u[i];       \
DEST = sum;                                 \
} while(false)

            void compute( const FitConicType t, array<T> &A)
            {
                assert(A.size()>=6);
                assert(data.size()>0);
                static const char fn[] = "FitConic::compute";
                //______________________________________________________________
                //
                // C=constraint matrix
                //______________________________________________________________
                matrix<T>   C(6,6);
                switch(t)
                {
                    case FitConicGeneric: // a^2+b^2+c^2=1
                        C[1][1] =   1;
                        C[2][2] =   1;
                        C[3][3] =   1;
                        break;

                    case FitConicEllipse: //! a*c-b^2/4=1
                        C[2][2] = -0.25;
                        C[1][3] = 0.5;
                        C[3][1] = 0.5;
                        break;
                }

                //______________________________________________________________
                //
                // S: matrix of moments
                //______________________________________________________________
                matrix<T> S(6);
                const size_t N = data.size();
                vector<T> u(N);

                // sums of Z = [xx xy yy x y 1]'
                YOCTO_MK_FIT_CONIC(S[1][1],        w2*value.xx*value.xx);
                YOCTO_MK_FIT_CONIC(S[1][2]=S[2][1],w2*value.xx*value.xy);
                YOCTO_MK_FIT_CONIC(S[1][3]=S[3][1],w2*value.xx*value.yy);
                YOCTO_MK_FIT_CONIC(S[1][4]=S[4][1],w2*value.xx*value.x );
                YOCTO_MK_FIT_CONIC(S[1][5]=S[5][1],w2*value.xx*value.y );
                YOCTO_MK_FIT_CONIC(S[1][6]=S[6][1],w2*value.xx         );

                YOCTO_MK_FIT_CONIC(S[2][2],        w2*value.xy*value.xy);
                YOCTO_MK_FIT_CONIC(S[2][3]=S[3][2],w2*value.xy*value.yy);
                YOCTO_MK_FIT_CONIC(S[2][4]=S[4][2],w2*value.xy*value.x );
                YOCTO_MK_FIT_CONIC(S[2][5]=S[5][2],w2*value.xy*value.y );
                YOCTO_MK_FIT_CONIC(S[2][6]=S[6][2],w2*value.xy         );


                YOCTO_MK_FIT_CONIC(S[3][3],        w2*value.yy*value.yy);
                YOCTO_MK_FIT_CONIC(S[3][4]=S[4][3],w2*value.yy*value.x );
                YOCTO_MK_FIT_CONIC(S[3][5]=S[5][3],w2*value.yy*value.y );
                YOCTO_MK_FIT_CONIC(S[3][6]=S[6][3],w2*value.yy         );

                YOCTO_MK_FIT_CONIC(S[4][4],        w2*value.x*value.x);
                YOCTO_MK_FIT_CONIC(S[4][5]=S[5][4],w2*value.x*value.y);
                YOCTO_MK_FIT_CONIC(S[4][6]=S[6][4],w2*value.x        );

                YOCTO_MK_FIT_CONIC(S[5][5],        w2*value.y*value.y);
                YOCTO_MK_FIT_CONIC(S[5][6]=S[6][5],w2*value.y        );

                YOCTO_MK_FIT_CONIC(S[6][6],w2);

                //______________________________________________________________
                //
                // M = inv(S)*C: matrix of the eigen problen
                //______________________________________________________________
                if(!LU<T>::build(S))
                {
                    throw imported::exception(fn,"singular distribution");
                }
                matrix<T> M(C,YOCTO_MATRIX_ENLARGE);
                LU<T>::solve(S,M);

                //______________________________________________________________
                //
                // Diagonalise...
                //______________________________________________________________
                vector<T> wr(6);
                vector<T> wi(6);
                size_t    nr=0;
                {
                    matrix<T> MM(M);
                    if( !diag<T>::eig(MM, wr, wi, nr) )
                    {
                        throw imported::exception(fn,"cannot find eigenvalues");
                    }
                }
                if(nr<=0)
                {
                    throw exception("no real eigenvalue found");
                }

                //______________________________________________________________
                //
                // find eigenvectors
                //______________________________________________________________
                matrix<T> evec(nr,6);
                diag<T>::eigv(evec,M, wr);
                
                //______________________________________________________________
                //
                // select the biggest real ev
                //______________________________________________________________
                const array<T> &U   = evec[nr];
                const T         UCU = tao::quadratic(C,U);
                if(UCU<=0)
                {
                    throw exception("invalid eigenvector");
                }
                const T scale = Sqrt(UCU);
                for(size_t i=6;i>0;--i) A[i] = U[i]/scale;
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitConic);
        };
        
        
    }
}

#endif
