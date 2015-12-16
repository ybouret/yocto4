#ifndef YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED
#define YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED 1

#include "yocto/math/fit/glsf.hpp"

namespace yocto
{
    namespace math
    {

        struct _GLS
        {
            //__________________________________________________________________
            //
            //! template creation of a fitting function
            //__________________________________________________________________
            template <
            typename T,
            template <typename> class FN
            >
            static inline
            typename GLS<T>::Function Create()
            {
                const FN<T> fn;
                return typename GLS<T>::Function(fn);
            }

            //__________________________________________________________________
            //
            //! polynomial function
            //__________________________________________________________________
            template <typename T>
            class Polynomial
            {
            public:
                inline  Polynomial() throw() {}
                inline  Polynomial(const Polynomial &) throw() {}
                inline ~Polynomial() throw() {}

                inline  T operator()(const T X, const array<T> &aorg)
                {
                    const size_t n = aorg.size();
                    if(n>0)
                    {
                        T ans = aorg[n];
                        for(size_t i=n;i>1;)
                        {
                            ans *= X;
                            ans += aorg[--i];
                        }
                        return ans;
                    }
                    else
                    {
                        return 0;
                    }
                }

            private:
                YOCTO_DISABLE_ASSIGN(Polynomial);
            };


            //__________________________________________________________________
            //
            //! Gaussian functions
            //__________________________________________________________________
            template <typename T>
            class Gauss
            {
            public:
                inline Gauss() throw() {}
                inline ~Gauss() throw() {}
                inline Gauss(const Gauss &) throw() {}

                //! #coefficients = 3*gaussians: amplitude*exp( -(scaling*(x-shift))^2 )
                inline  T operator()(const T X, const array<T> &aorg)
                {
                    const size_t nc = aorg.size();
                    assert(nc>=3);
                    assert(0==nc%3);
                    const size_t n = nc/3;
                    size_t       i = 0;
                    T            g = 0;

                    for(size_t j=1;j<=n;++j)
                    {
                        const T amplitude = aorg[++i];
                        const T scaling   = aorg[++i];
                        const T shift     = aorg[++i];

                        const T arg       = scaling*(X-shift);
                        g += amplitude * Exp( - arg*arg );
                    }

                    return g;
                }
            };


            //__________________________________________________________________
            //
            //! Pade approximant
            //__________________________________________________________________
            template <typename T>
            class Pade
            {
            public:
                inline  Pade() throw() {}
                inline ~Pade() throw() {}
                inline  Pade(const Pade &) throw() {}

                inline T operator()(const T X, const array<T> &a)
                {
                    return 0;
                }

            private:
                YOCTO_DISABLE_ASSIGN(Pade);
            };

        };


    }

}

#endif

