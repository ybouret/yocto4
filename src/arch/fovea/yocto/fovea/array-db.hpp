#ifndef YOCTO_FOVEA_ARRAY_DB_INCLUDED
#define YOCTO_FOVEA_ARRAY_DB_INCLUDED 1

#include "yocto/fovea/linear.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace fovea
    {
        
        typedef intr_ptr<string,linear_space> linear_ptr;
        typedef set<string,linear_ptr>        adb_type;
        
        class array_db : public adb_type
        {
        public:
            explicit array_db() throw();
            virtual ~array_db() throw();
            
            void store( linear_space *l );
            
            linear_space & operator[](const string &id );
            linear_space & operator[](const char   *id );
           
            const linear_space & operator[](const string &id ) const;
            const linear_space & operator[](const char   *id ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array_db);
        };
        
    }
}

#endif


