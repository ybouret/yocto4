#ifndef YOCTO_CHEMICAL_EFFECTOR_INCLUDED
#define YOCTO_CHEMICAL_EFFECTOR_INCLUDED 1

#include "yocto/chemical/solution.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class effector : public counted_object
        {
        public:
            virtual ~effector() throw();
            
            const string name; //!< identifier
            double       Vmax; //!< scaling factor, default is 1.0
            
            const string &key() const throw();
            typedef intr_ptr<string,effector> pointer;
            
            
            
        protected:
            explicit effector( const string & );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
    }
}

#endif
