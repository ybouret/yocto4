#ifndef YOCTO_SPLIT1D_INCLUDED
#define YOCTO_SPLIT1D_INCLUDED 1

#include "patch.hpp"

namespace yocto
{
    namespace parallel
    {

        //! split in one dimension
        /*
         base class: patch1D to keep original dimensions
         */
        class split1D : public patch1D
        {
        public:
            typedef patch1D patch_type;

            const size_t cores; //!< optimal #cores

            //! copy original patch and compute #cores=clamp(1,num_procs,width)
            explicit split1D(const size_t num_procs,const patch1D &p);

            //! copy  constructor
            explicit split1D(const split1D &other);

            //! default destructor
            virtual ~split1D() throw();

            //! compute the sub patch for 0<=rank<cores
            patch1D operator()(size_t rank) const throw();

        private:
            YOCTO_DISABLE_ASSIGN(split1D);
        };
    }
}

#endif
