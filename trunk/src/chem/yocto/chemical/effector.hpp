#ifndef YOCTO_CHEMICAL_EFFECTOR_INCLUDED
#define YOCTO_CHEMICAL_EFFECTOR_INCLUDED 1

#include "yocto/chemical/solution.hpp"

namespace yocto
{
    namespace chemical
    {
        
#define YOCTO_CHEMICAL_EFFECTOR_ARGS solution &dSdt, const double t, const double zeta, const solution &S, const solution &S_out
        
        class effector : public counted_object
        {
        public:
            virtual ~effector() throw();
            
            const string name; //!< identifier
            double       Vmax; //!< scaling factor, default is 1.0
            
            const string &key() const throw();
            typedef intr_ptr<string,effector> pointer;
            typedef set<string,pointer>       database;
            
            //! compute the rate WITHOUT the Vmax
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
            
            void compute_rate( YOCTO_CHEMICAL_EFFECTOR_ARGS, solution &Stmp );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
        };
        
    }
}

#endif
