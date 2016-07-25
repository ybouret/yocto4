#ifndef YOCTO_MK_ALG_SHAPES2D_INCLUDED
#define YOCTO_MK_ALG_SHAPES2D_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/core/lu.hpp"

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

            inline bool compute(T &xcenter, T &ycenter, T &radius )
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
                    return false;
                }

                LU<T>::solve(S,Q);

                const T a = Q[1];
                const T b = Q[2];
                const T c = Q[3];
                xcenter = T(0.5)*a;
                ycenter = T(0.5)*b;
                radius  = Sqrt( max_of<T>(0,c + xcenter*xcenter + ycenter*ycenter) );
                return true;
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
            virtual bool compute()
            {
                assert(data.size()>0);
                const size_t N = data.size();
                matrix<T> Sqq(3), Sqz(3), Szz(3);
                vector<T> u(N);

                //______________________________________________________________
                //
                // build Sqq matrices , sum of Q*Q', Q = [xx xy yy]'
                //______________________________________________________________
                YOCTO_MK_FIT_CONIC(Sqq[1][1],value.xx*value.xx);
                YOCTO_MK_FIT_CONIC(Sqq[1][2]=Sqq[2][1],value.xx*value.yy);
                YOCTO_MK_FIT_CONIC(Sqq[1][3]=Sqq[3][1],value.xx*value.xy);

                YOCTO_MK_FIT_CONIC(Sqq[2][2],value.xy*value.xy);
                YOCTO_MK_FIT_CONIC(Sqq[2][3]=Sqq[3][2],value.xy*value.yy);

                YOCTO_MK_FIT_CONIC(Sqq[3][3],value.yy*value.yy);

                //______________________________________________________________
                //
                // build Szz matrix, sum of Z*Z', Z=[x y 1]'
                //______________________________________________________________
                YOCTO_MK_FIT_CONIC(Szz[1][1],value.xx);
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

                std::cerr << "Sqq=" << Sqq << std::endl;
                std::cerr << "Sqz=" << Sqz << std::endl;
                std::cerr << "Szz=" << Szz << std::endl;


                return true;
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(FitConic);
        };


    }
}

#endif
