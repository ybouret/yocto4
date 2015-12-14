#ifndef YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED
#define YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED 1

#include "yocto/math/fit/glsf.hpp"

namespace yocto
{
    namespace math
    {

        struct _GLS
        {

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


        };


    }

}

#endif

