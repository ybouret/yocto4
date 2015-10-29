#ifndef YOCTO_SPLIT2D_INCLUDED
#define YOCTO_SPLIT2D_INCLUDED 1

#include "patch.hpp"

namespace yocto
{
    namespace parallel
    {

        class split2D : public patch2D
        {
        public:
            const size_t cores;
            const size_t x_cores;
            const size_t y_cores;
            
            explicit split2D(const size_t num_procs, const patch2D &p);
            virtual ~split2D() throw();

            patch2D operator()(size_t rank) const throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(split2D);

            
        };
    }

}

#endif
