#ifndef YOCTO_MATH_FCN_NEWT_INCLUDED
#define YOCTO_MATH_FCN_NEWT_INCLUDED 1

#include "yocto/math/fcn/jacobian.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/many-arrays.hpp"

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
            typedef many_arrays<T,memory::global> arr_mgr;
            size_t        nvar;
            Field        *hook;  //!< user's field
            array<T>     *pvar;  //!< user's variables

            matrix<T>     J;     //!< Jacobian/ SVD
            matrix<T>     V;     //!< for SVD
            arr_mgr       arr;   //!< for little vectors
            array<T> &    w;     //!< for SVD
            array<T> &    F;     //!< function value
            array<T> &    gradf; //!< J'*F, gradient of objective function
            array<T> &    sigma; //!< full quasi-Newton's step
            array<T> &    xtry;  //!< for scanning

            matrix<T>     M;     //!< for backtracking, size=2x2
            vector<T>     rhs;   //!< for backtracking, size=2
            function1     scan;  //!< scan value for a fraction of step

            
            T __scan(const T lam); //!< for step control


            YOCTO_DISABLE_COPY_AND_ASSIGN(newt);
        };

    }
}

#endif
