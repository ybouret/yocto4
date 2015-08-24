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
            static  void compute1D(size_t rank,
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

            //! setting up 1D splitter
            class in1D
            {
            public:
                const size_t  cores;  //!< maximal #cores vs the requested nproc
                const coord1D offset;
                const coord1D length;

                //! compute 1<=size<=nproc, offset and length from patch
                explicit in1D(const size_t nproc, const patch1D &p) throw();
                virtual ~in1D() throw();

                patch1D operator()(const size_t rank) const throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(in1D);
            };
            
            //! 2D splitter
            class in2D
            {
            public:
                const size_t  cores;  //!< #cores
                const size_t  xsize;  //!< X-#cores
                const size_t  ysize;  //!< Y-#cores
                const coord2D offset; //!< global offsets
                const coord2D length; //!< global lengths
                const double  timing; //!< the comm timing

                //! compute the optimal xsize*ysize=size from nproc and patch
                explicit in2D(const size_t nproc, const patch2D &p) throw();
                virtual ~in2D() throw();

                size_t get_xrank(const size_t rank) const throw();
                size_t get_yrank(const size_t rank) const throw();
                size_t get_rank(const size_t rx, const size_t ry) const throw();


                patch2D operator()(const size_t rank) const throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(in2D);
                double init(const size_t Lx, const size_t Ly) throw();
            };
        };

        template <size_t>
        struct split_for;
        template <> struct split_for<1> { typedef split::in1D type; };
        template <> struct split_for<2> { typedef split::in2D type; };


    }
}

#endif
