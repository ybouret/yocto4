#ifndef YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED
#define YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED 1

#include "yocto/math/fit/glsf.hpp"

namespace yocto
{
    namespace math
    {

        struct _GLS
        {
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
                        return 0;
                }

            private:
                YOCTO_DISABLE_ASSIGN(Polynomial);
            };


            template <typename T>
            class Gauss
            {
            public:
                inline Gauss() throw() {}
                inline ~Gauss() throw() {}
                inline Gauss(const Gauss &) throw() {}

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




        };


    }

}

#endif

