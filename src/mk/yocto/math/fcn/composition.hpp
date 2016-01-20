#ifndef YOCTO_MATH_FCN_COMPOSITION_INCLUDED
#define YOCTO_MATH_FCN_COMPOSITION_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {

        //! to wrap g(f(x))
        template <typename T>
        class composition
        {
        public:
            typedef typename numeric<T>::function function_type;
            typedef          T                   (*cproc_type)(T);

            function_type g;
            function_type f;
            inline explicit composition( const function_type &G, const function_type &F) :
            g(G),
            f(F)
            {

            }

            inline composition(const composition &other) :
            g(other.g),
            f(other.f)
            {
            }


            inline virtual ~composition() throw()
            {
            }

            inline T   compute(   const T x ) { return g(f(x)); }
            inline T   operator()(const T x ) { return g(f(x)); }

            static inline
            function_type build(const function_type &G, const function_type &F)
            {
                const composition gf(G,F);
                return function_type(gf);
            }

            static inline
            function_type build(cproc_type proc, const function_type &F )
            {
                const  function_type G(proc);
                composition          gf(G,F);
                return function_type(gf);
            }


        private:
            YOCTO_DISABLE_ASSIGN(composition);
        };

    }
}

#endif

