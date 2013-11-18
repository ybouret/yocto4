#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/residue.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class library
        {
        public:
            explicit library();
            virtual ~library() throw();
            
            typename atom<T>::properties::table    atoms;
            typename residue<T>::properties::table residues;
            
            typename residue<T>::properties & declare_residue( word_t type, const string &name );
            typename atom<T>::properties    & declare_atom( word_t type, const string &name, T mass);

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
    }
    
}


#endif
