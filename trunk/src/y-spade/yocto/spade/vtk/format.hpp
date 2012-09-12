#ifndef YOCTO_SPADE_VTK_FORMAT_INCLUDED
#define YOCTO_SPADE_VTK_FORMAT_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    namespace spade
    {
        //! format for basic types
        class vtk_format
        {
        public:
            explicit vtk_format();
            virtual ~vtk_format() throw();
            
            string real_fmt; //!< format for real, default is %g
            string unit_fmt; //!< format for unit, default is %ld
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_format);
        };
        
    }
}

#endif
