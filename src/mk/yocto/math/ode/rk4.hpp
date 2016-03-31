#ifndef YOCTO_MATH_ODE_RK4_INCLUDED
#define YOCTO_MATH_ODE_RK4_INCLUDED 1

#include "yocto/math/ode/types.hpp"
#include "yocto/sequence/many-arrays.hpp"
#include "yocto/math/core/tao.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <typename T>
            class RK4 : public many_arrays<T,memory_type>
            {
            public:
                typedef many_arrays<T,memory_type>  arrays_type;
                typedef typename Field<T>::Array    Array;
                typedef typename Field<T>::Equation Equation;
                typedef typename Field<T>::Callback Callback;

                inline RK4() :
                arrays_type(6),
                Yt( this->next_array() ),
                k1( this->next_array() ),
                k2( this->next_array() ),
                k3( this->next_array() ),
                k4( this->next_array() ),
                YY( this->next_array() )
                {
                }

                inline ~RK4() throw()
                {
                }

                inline void operator()( Equation &F, Array &Y, const T X0, const T X1, Callback *cb=0 )
                {
                    const T h    = X1-X0;
                    const T half = T(0.5)*h;
                    const T scal = h/T(6);
                    const T Xmid = X0+half;
                    // k1 = F(X0,Y)
                    F(k1,X0,Y);

                    // k2 = F(X0+h/2,Y+h/2*k1)
                    tao::setprobe(Yt, Y, half, k1);
                    if(cb) (*cb)(Yt,Xmid);
                    F(k2,Xmid,Yt);

                    // k3 = F(X0+h/2,Y+h/2*k2)
                    tao::setprobe(Yt,Y,half,k2);
                    if(cb) (*cb)(Yt,Xmid);
                    F(k3,Xmid,Yt);

                    // k4 = F(X0+h=X1,Y+h*k3)
                    tao::setprobe(Yt, Y, h, k3);
                    if(cb) (*cb)(Yt,X1);
                    F(k4,X1,Yt);

#define YMK_RK4_UPDATE(I) const T _k2 = k2[I]; const T _k3 = k3[I]; Y[I] += scal * (k1[I]+(_k2+_k2)+(_k3+_k3)+k4[I])
                    YOCTO_LOOP_FUNC_(Yt.size(), YMK_RK4_UPDATE, 1);
#undef  YMK_RK4_UPDATE
                }

            private:
                Array &Yt;
                Array &k1;
                Array &k2;
                Array &k3;
                Array &k4;

                YOCTO_DISABLE_COPY_AND_ASSIGN(RK4);

            public:
                Array &YY; //!< may be used by user
            };

        }
    }
}

#endif
