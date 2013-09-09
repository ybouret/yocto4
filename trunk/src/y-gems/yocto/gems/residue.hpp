#ifndef YOCTO_GEMS_RESIDUE_INCLUDED
#define YOCTO_GEMS_RESIDUE_INCLUDED 1

#include "yocto/gems/atom.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class residue : public identifier, public atom<T>::group
        {
        public:
            typedef intrusive_ptr<word_t,residue> pointer;

            explicit residue( word_t u, word_t t) throw();
            virtual ~residue() throw();
            
            
            class properties : public gems::properties
            {
            public:
                typedef intrusive_ptr<word_t,properties> pointer;
                explicit properties(word_t t, const string &n);
                virtual ~properties() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
            };
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(residue);
        };
    }
}

#endif

