#ifndef YOCTO_GEMS_RESIDUE_INCLUDED
#define YOCTO_GEMS_RESIDUE_INCLUDED 1

#include "yocto/gems/atom.hpp"


namespace yocto
{
    namespace gems
    {
        
        //! a residue identifies a group of atoms
        template <typename T>
        class residue : public identifier, public atom<T>::group
        {
        public:
            typedef  intr_ptr<word_t,residue> pointer;

            explicit residue( word_t u, word_t t ) throw();
            virtual ~residue() throw();
            
            class properties : public gems::properties
            {
            public:
                explicit properties( word_t t, const string &n);
                virtual ~properties() throw();
                
                typedef nosy_ptr<word_t,string,properties>  pointer;
                typedef gems::properties::table<properties> table;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
            };
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(residue);
        };
    }
}

#endif

