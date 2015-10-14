#ifndef YOCTO_MATH_TRIDIAG_INCLUDED
#define YOCTO_MATH_TRIDIAG_INCLUDED 1

#include "yocto/sequence/many-arrays.hpp"
#include "yocto/xnumeric.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class tri_diag_base
        {
        public:
            typedef array<T>       array_type;
            typedef memory::global memory_type;
            virtual ~tri_diag_base() throw() {}

            virtual T operator()(const size_t i,const size_t j) const = 0;

            inline size_t size() const throw() { return arrays.size; }

        protected:
            many_arrays<T,memory_type> arrays;

        public:
            array_type &a;
            array_type &b;
            array_type &c;

        protected:
            explicit inline tri_diag_base(size_t na) :
            arrays(na),
            a( arrays.next_array() ),
            b( arrays.next_array() ),
            c( arrays.next_array() )
            {
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tri_diag_base);
        };

        template <typename T>
        class tri_diag : public tri_diag_base<T>
        {
        public:
            explicit tri_diag(size_t n) : tri_diag_base<T>(6)
            {
                this->arrays.allocate(n);
            }

            virtual ~tri_diag() throw()
            {
            }

            virtual T operator()(const size_t i,const size_t j) const
            {
                assert(i<=this->size());
                assert(j<=this->size());
                if(i<j)
                {
                    if(1==j-i)
                    {
                        return this->c[i];
                    }
                    else
                    {
                        return xnumeric<T>::zero();
                    }
                }
                else
                {
                     if(j<i)
                     {
                         if(1==i-j)
                         {
                             return this->a[i];
                         }
                         else
                         {
                             return xnumeric<T>::zero();
                         }
                     }
                    else
                    {
                        return this->b[i];
                    }
                }
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tri_diag);
        };

    }

}

#endif
