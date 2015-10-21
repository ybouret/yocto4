#ifndef YOCTO_MATH_MATRICES_INCLUDED
#define YOCTO_MATH_MATRICES_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{

    //! helper to manage a group of matrices
    template <
    typename T,
    template <class> class MATRIX>
    class tableau_of : public vector< MATRIX<T> >
    {
    public:
        typedef  MATRIX<T>           matrix_type;
        typedef  vector<matrix_type> vector_type;

        virtual ~tableau_of() throw()
        {

        }

        //! generic matrices
        explicit tableau_of(const size_t n,
                            const size_t nr,
                            const size_t nc) :
        vector_type(n,as_capacity)
        {
            for(size_t i=1;i<=n;++i)
            {
                this->template append<size_t,size_t>(nr,nc);
            }
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(tableau_of);
    };


}


#endif
