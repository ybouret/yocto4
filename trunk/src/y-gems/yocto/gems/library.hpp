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
            
            typedef typename atom<T>::properties    atm_properties;
            typedef typename residue<T>::properties res_properties;
            
            typedef set<word_t,typename atm_properties::ptr,key_hasher<word_t,hashing::sfh>,allocator>    atmlib;
            typedef set<word_t,typename res_properties::ptr,key_hasher<word_t,hashing::sfh>,allocator> reslib;

            atmlib atoms;
            reslib residues;
            
            res_properties & declare_residue( word_t type, const string &name );
            res_properties & declare_residue( word_t type, const char   *name );

            atm_properties & declare_atom(word_t type, const string &name, T mass );
            atm_properties & declare_atom(word_t type, const char   *name, T mass );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
    }
    
}


#endif
