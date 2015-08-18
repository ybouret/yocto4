#ifndef YOCTO_CODE_MPI_SPLIT_INCLUDED
#define YOCTO_CODE_MPI_SPLIT_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace core
    {
        template <typename T>
        inline void mpi_split(size_t rank,
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

        void find_mpi_split2D(size_t      &px,
                              size_t      &py,
                              const size_t nproc,
                              const size_t Nx,
                              const size_t Ny) throw();

        template <typename T>
        class mpi_split2D
        {
        public:
            const size_t px; //!< #procs in x direction
            const size_t py; //!< #procs in y direction

            inline explicit mpi_split2D(const size_t size,
                                        const T      xlength,
                                        const T      ylength) throw() :
            px(0),
            py(0)
            {
                assert(size>0);
                find_mpi_split2D((size_t&)px, (size_t&)py, size, xlength, ylength);
                assert(px>0);
                assert(py>0);
                //std::cerr << "\t#x=" << xlength << ", #y=" << ylength << std::endl;
                //std::cerr << "\tpx=" << px <<", py=" << py << std::endl;
            }

            inline virtual ~mpi_split2D() throw() {}

            inline size_t get_xrank(const size_t rank) const throw()
            {
                return rank % px;
            }

            inline size_t get_yrank(const size_t rank) const throw()
            {
                return rank / px;
            }

            inline void operator()(size_t rank,
                                   T     &xoffset,
                                   T     &yoffset,
                                   T     &xlength,
                                   T     &ylength) const throw()
            {
                const size_t rx = get_xrank(rank); assert(rx<px);
                const size_t ry = get_yrank(rank); assert(ry<py);
                //std::cerr << "\trx=" << rx << ", ry=" << ry << std::endl;
                mpi_split(rx,px,xoffset,xlength);
                mpi_split(ry,py,yoffset,ylength);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mpi_split2D);

        };

        
    }
}

#endif
