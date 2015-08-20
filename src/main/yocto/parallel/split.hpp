#ifndef YOCTO_PARALLEL_SPLIT_INCLUDED
#define YOCTO_PARALLEL_SPLIT_INCLUDED 1

#include "yocto/parallel/patch.hpp"

namespace yocto
{
    namespace parallel
    {
        struct split
        {
            template <typename T> inline
            static  void in1D(size_t rank,
                              size_t size,
                              T     &offset,
                              T     &length) throw()
            {
                assert(size>0);
                assert(rank<size);
                //assert(length>=0);
                T todo(length/size);
                for(size_t r=1;r<=rank;++r)
                {
                    length -= todo;
                    offset += todo;
                    todo    = length/(size-r);
                }
                length = todo;
            }

            static patch1D compute(size_t rank,size_t size,const patch1D &source);

            class in2D
            {
            public:
                const size_t size;
                const size_t xsize;
                const size_t ysize;
                //! compute the optimal xsize*ysize=size
                explicit in2D(const size_t nproc, const size_t Lx, const size_t Ly) throw();
                explicit in2D(const size_t nproc, const patch2D &p) throw();
                virtual ~in2D() throw();

                size_t get_xrank(const size_t rank) const throw();
                size_t get_yrank(const size_t rank) const throw();
                size_t get_rank(const size_t rx, const size_t ry) const throw();


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(in2D);
                void init(const size_t Lx, const size_t Ly) throw();
            };
        };
    }
}

#endif
