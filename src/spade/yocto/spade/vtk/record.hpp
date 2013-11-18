#ifndef YOCTO_SPADE_VTK_RECORD_INCLUDED
#define YOCTO_SPADE_VTK_RECORD_INCLUDED 1

#include "yocto/spade/vtk/format.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/type/spec.hpp"

namespace yocto
{
    namespace spade
    {
        //! write any data on fp with ftm
        typedef void (*vtk_callback)( ios::ostream &fp, const void *data, const vtk_format &fmt );
        
        class vtk_record
        {
        public:
            const type_spec     spec; //!< identifier
            const string        kind; //!< SCALARS | VECTORS
            const vtk_callback  proc; //!< print procedure
            
            const type_spec & key() const throw();
            vtk_record( const vtk_record & );
            vtk_record( const std::type_info &id, const char *k, vtk_callback cb );
            ~vtk_record() throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(vtk_record);            
        };
    }
}


#endif
