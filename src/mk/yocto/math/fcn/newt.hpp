#ifndef YOCTO_MATH_FCN_NEWT_INCLUDED
#define YOCTO_MATH_FCN_NEWT_INCLUDED 1

#include "yocto/math/fcn/jacobian.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {

        
        template <typename T>
        class newt
        {
        public:
            typedef typename numeric<T>::vector_field             Field;
            typedef typename numeric<T>::function                 function1;
            typedef typename jacobian<T>::type                    Jacobian;
            explicit newt();
            virtual ~newt() throw();

            bool solve( Field &func, Jacobian &fjac, array<T> &x );


        private:
            size_t        nvar;
            Field        *hook;  //!< user's field
            array<T>     *pvar;  //!< user's variables
            vector<T>     F;     //!< function value
            matrix<T>     J;     //!< Jacobian/ SVD
            matrix<T>     V;     //!< for SVD
            vector<T>     w;     //!< for SVD
            vector<T>     gradf; //!< J'*F, gradient of objective function
            vector<T>     sigma; //!< full quasi-Newton's step
            vector<T>     xtry;  //!< for scanning
            matrix<T>     M;     //!< for backtracking
            vector<T>     rhs;   //!< for backtracking
            //function1     eval;  //!< call _eval, for Jacobian
            function1     scan;  //!< scan value for a fraction of step

            //T __eval(const T X);   //!< for Jacobian, uses ivar, ifcn, hook, pvar
            //void computeJ();

            T __scan(const T lam); //!< for step control


            YOCTO_DISABLE_COPY_AND_ASSIGN(newt);
        };

    }
}

#endif
