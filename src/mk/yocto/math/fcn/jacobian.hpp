#ifndef YOCTO_MATH_FCN_JACOBIAN_INCLUDED
#define YOCTO_MATH_FCN_JACOBIAN_INCLUDED 1

#include "yocto/math/fcn/drvs.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class jacobian : public derivative<T>
        {
        public:
            typedef functor<void,TL2(matrix<T>&,const array<T>&)> type;
            typedef typename numeric<T>::vector_field             field;
            typedef typename numeric<T>::function                 function1;

            explicit jacobian(const field &f) :
            func(f),
            ifcn(0),
            jvar(0),
            pvar(0),
            eval(this, & jacobian<T>::__eval),
            F(),
            scaling(1e-4)
            {}

            virtual ~jacobian() throw() {}

            inline void compute(matrix<T> &J, const array<T> &x)
            {
                assert(J.is_square());
                assert(J.cols==x.size());
                derivative<T> &drvs = *this;
                const size_t   nvar = x.size();
                F.make(nvar);
                pvar = &x;
                for(ifcn=nvar;ifcn>0;--ifcn)
                {
                    for(jvar=nvar;jvar>0;--jvar)
                    {
                        J[ifcn][jvar] = drvs(eval,x[jvar],scaling);
                    }
                }
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(jacobian);

            field           func;
            size_t          ifcn;
            size_t          jvar;
            const array<T> *pvar;
            function1       eval;
            vector<T>       F;

            inline T __eval( const T u )
            {
                assert(pvar);
                assert(jvar>0);
                assert(ifcn>0);
                assert(jvar<=pvar->size());
                assert(ifcn<=pvar->size());
                const array<T> &xorg = *pvar;
                T              &xval = (T&)xorg[jvar];
                const T         xsav =  xval;
                xval = u;
                func(F,xorg);
                xval = xsav;
                return F[ifcn];
            }

        public:
            T scaling;
        };
    }

}

#endif
