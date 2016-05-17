#ifndef YOCTO_SPLIT2D_INCLUDED
#define YOCTO_SPLIT2D_INCLUDED 1

#include "patch.hpp"

namespace yocto
{
    namespace parallel
    {

        //! base class to compute 2D splitting
        /**
         base class: keep original patch for computations
         */
        class split2D : public patch2D
        {
        public:
            typedef patch2D patch_type;

            const size_t cores;    //!< between 1 and num_procs, cores=x_cores*y_cores
            const size_t x_cores;  //!< cores for width
            const size_t y_cores;  //!< cores for height
            
            explicit split2D(const size_t num_procs, const patch2D &p);
            virtual ~split2D() throw();

            patch2D operator()(size_t rank) const throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(split2D);

            
        };
    }

}

#endif
