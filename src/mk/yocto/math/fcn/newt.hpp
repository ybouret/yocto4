#ifndef YOCTO_MATH_FCN_NEWT_INCLUDED
#define YOCTO_MATH_FCN_NEWT_INCLUDED 1

#include "yocto/math/fcn/drvs.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class newt
        {
        public:
            typedef typename numeric<T>::vector_field field;
            typedef typename numeric<T>::function     function1;
            explicit newt();
            virtual ~newt() throw();

            bool solve( field &Field, array<T> &x );


        private:
            size_t        nvar;
            size_t        ivar;  //!< for drvs
            size_t        ifcn;  //!< for drvs
            field        *hook;  //!< field
            array<T>     *pvar;  //!< user's var
            vector<T>     F;     //!< function value
            matrix<T>     J;     //!< Jacobian
            matrix<T>     U;     //!< for SVD
            matrix<T>     V;     //!< for SVD
            vector<T>     w;     //!< for SVD
            vector<T>     gradf; //!< J'*F, gradient of objective function
            vector<T>     sigma; //!< full quasi-Newton's step
            vector<T>     xtry;  //!< for scanning
            matrix<T>     M;     //!< for backtracking
            vector<T>     rhs;   //!< for backtracking
            function1     eval;  //!< call _eval, for Jacobian
            function1     scan;  //!< scan value for a fraction of step

            T __eval(const T X);   //!< for Jacobian, uses ivar, ifcn, hook, pvar
            void computeJ();

            T __scan(const T lam); //!< for step control


            YOCTO_DISABLE_COPY_AND_ASSIGN(newt);
            
            
        public:
            derivative<T> drvs;    //!< for derivatives
            T             scaling; //!< for derivatives, initially 1e-4
        };

    }
}

#endif
