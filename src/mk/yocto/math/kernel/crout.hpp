#ifndef YOCTO_MATH_KERNEL_CROUT_INCLUDED
#define YOCTO_MATH_KERNEL_CROUT_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        class crout : public vector< typename real_of<T>::type >
        {
        public:
            typedef typename real_of<T>::type real_type;
            typedef vector<real_type>         scal_type;
            
            explicit crout() throw();
            explicit crout(size_t);
            virtual ~crout() throw();
            
            void prepare(size_t n); // this->make(n,0)
            
            //! crout algorithm
            /**
             \param M a square matrix
             \warning this->size() >= M.rows
             */
            bool build( matrix<T> &M ) throw();
            
            
            //! solve by backsubstitution
            /**
             the matrix a must have been decomposed...
             */
            static void solve(const matrix<T> &a,
                              array<T>        &b) throw();
            
            //! solve the whole matrix
            static void solve(const matrix<T>     &M,
                              matrix<T>           &Q) throw();
            
            
            //! solve the inverse matrix
            /**
             memory must be allocated before
             */
            bool inverse( matrix<T> &a );
            
            
            //! Moore-Penrose pseudo inverse, P.cols >= P.rows
            /**
             memory must be allocated before
             */
            bool pseudo_inverse(matrix<T> &M, const matrix<T> &P);
            
            bool dneg; //!< a flag for determinant sign
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(crout);
        };
        
        
    }
}

#endif

