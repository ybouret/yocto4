#ifndef YOCTO_SPLIT1D_INCLUDED
#define YOCTO_SPLIT1D_INCLUDED 1

#include "patch.hpp"

namespace yocto
{
    namespace parallel
    {

        class split1D : public patch1D
        {
        public:
            const size_t cores; //!< optimal #cores

            explicit split1D(const size_t num_procs,const patch1D &p);
            explicit split1D(const split1D &other);
            virtual ~split1D() throw();

            patch1D operator()(size_t rank) const;

        private:
            YOCTO_DISABLE_ASSIGN(split1D);
        };
    }
}

#endif
