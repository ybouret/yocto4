#ifndef YOCTO_CHEMICAL_EFFECTOR_INCLUDED
#define YOCTO_CHEMICAL_EFFECTOR_INCLUDED 1

#include "yocto/chemical/solution.hpp"
#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
#define YOCTO_CHEMICAL_EFFECTOR_ARGS \
array<double>      &dSdt,   \
const double        t,      \
const double        zeta,   \
const array<double> &S,     \
const array<double> &S_out, \
const collection    &lib
        
        class effector : public counted_object
        {
        public:
            virtual ~effector() throw();
            
            const string name; //!< identifier
            double       Vmax; //!< scaling factor, default is 1.0
            
            const string &key() const throw();
            typedef intr_ptr<string,effector> pointer;
            typedef set<string,pointer>       database;
            
            //! compute the rates WITHOUT the Vmax
            virtual void call( YOCTO_CHEMICAL_EFFECTOR_ARGS ) const = 0;
            
        protected:
            explicit effector( const string & );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        
        class effectors : public effector::database
        {
        public:
            explicit effectors() throw();
            virtual ~effectors() throw();
            
            void compute_rate( YOCTO_CHEMICAL_EFFECTOR_ARGS );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
            vector<double> tmp;
        };
        
    }
}

#endif
