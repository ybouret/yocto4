#ifndef YOCTO_DATASPACE_INCLUDED
#define YOCTO_DATASPACE_INCLUDED 1

#include "yocto/spade/fields-setup.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename LAYOUT>
        class dataspace : public array_db, public LAYOUT
        {
        public:
            typedef LAYOUT                layout_type;
            typedef fields_setup<LAYOUT>  fields_list;
            
            virtual ~dataspace() throw() {}
            
            const LAYOUT outline; //!< this layout + ghosts
            
            explicit dataspace( const LAYOUT &L, const fields_list &F ) :
            array_db( F.size() + LAYOUT::DIMENSIONS ),
            LAYOUT(L),
            outline(L)
            {
                //--------------------------------------------------------------
                // setup ghosts
                //--------------------------------------------------------------
                
                //--------------------------------------------------------------
                // create fields from outline
                //--------------------------------------------------------------
                F.create( outline, *this );
            }
        
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(dataspace);
        };
        
    }
}

#endif
