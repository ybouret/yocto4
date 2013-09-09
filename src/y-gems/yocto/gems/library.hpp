#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/residue.hpp"
#include "yocto/associative/set.hpp"

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
            
            
            typedef typename atom<T>::properties    atom_properties;
            typedef typename residue<T>::properties residue_properties;
            
            typedef set<word_t,typename atom_properties::pointer,key_hasher<word_t,hashing::sfh>,allocator>    atmlib;
            typedef set<word_t,typename residue_properties::pointer,key_hasher<word_t,hashing::sfh>,allocator> reslib;

            atmlib atoms;
            reslib residues;
            
            residue_properties & declare_residue( word_t type, const string &name );
            residue_properties & declare_residue( word_t type, const char   *name );

            atom_properties & declare_atom(word_t type, const string &name, T mass );
            atom_properties & declare_atom(word_t type, const char   *name, T mass );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
    }
    
}


#endif
