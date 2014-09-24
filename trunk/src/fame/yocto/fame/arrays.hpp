#ifndef YOCTO_FAME_ARRAYS_INCLUDED
#define YOCTO_FAME_ARRAYS_INCLUDED 1

#include "yocto/fame/linear.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace fame
    {
        
        typedef intr_ptr<string,linear_space> linear_ptr;
        typedef set<string,linear_ptr>        arrays_type;
        
        class arrays : public arrays_type
        {
        public:
            explicit arrays() throw();
            explicit arrays(size_t n);
            virtual ~arrays() throw();
            
            void store( linear_space *l );
            
            linear_space & operator[](const string &id );
            linear_space & operator[](const char   *id );
            
            const linear_space & operator[](const string &id ) const;
            const linear_space & operator[](const char   *id ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(arrays);
        };
        
    }
}

#endif


