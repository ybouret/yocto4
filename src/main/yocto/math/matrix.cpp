#include "yocto/math/matrix.hpp"

namespace yocto
{
    namespace math
    {
        matrix_dims:: ~matrix_dims() throw()
        {
            (size_t &)rows  = 0;
            (size_t &)cols  = 0;
            (size_t &)items = 0;

        }

        matrix_dims:: matrix_dims() throw() :
        rows(0), cols(0), items(0)
        {
        }

    }

}
