#ifndef YOCTO_CHEMICAL_EFFECTOR_INCLUDED
#define YOCTO_CHEMICAL_EFFECTOR_INCLUDED 1

#include "yocto/chemical/solution.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class effector : public object, public counted
        {
        public:
            const string name;
            double       factor; //!< scaling factor, default is 1
            
            virtual ~effector() throw();
            const string &key() const throw();
            
            typedef intrusive_ptr<string,effector> ptr;
            
            //! compute the rate
            virtual void call(solution &dSdt, double t, double zeta, const solution &S ) const = 0;
            
            
        protected:
            explicit effector( const string &id );

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        typedef set<string,effector::ptr> effectors_db;
        
        class effectors : public effectors_db
        {
        public:
            explicit effectors();
            virtual ~effectors() throw();
            
            void collect(solution &dSdt, double t, double zeta, const solution &S ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
        };
    }
    
}

#endif
