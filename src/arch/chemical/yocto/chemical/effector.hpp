#ifndef YOCTO_CHEMICAL_EFFECTOR_INCLUDED
#define YOCTO_CHEMICAL_EFFECTOR_INCLUDED 1

#include "yocto/chemical/solution.hpp"

namespace yocto
{
    namespace chemical
    {
        
        //! compute a global rate
        class effector : public object, public counted
        {
        public:
            const string name;   //!< unique identifier
            double       factor; //!< scaling factor, default is 1
            
            virtual ~effector() throw();
            const string &key() const throw(); //!< the name
            
            typedef intr_ptr<string,effector> ptr;
            
            //! compute the appropriate rates, WITHOUT factor
            virtual void call(solution &dSdt, double t, double zeta, const solution &S, const solution &S_out ) const = 0;
            
            
        protected:
            explicit effector( const string &id );

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        //! base class for effectors database
        typedef set<string,effector::ptr> effectors_db;
        
        //! database of effectos
        class effectors : public effectors_db
        {
        public:
            explicit effectors();
            virtual ~effectors() throw();
            
            effector       & operator[]( const string &id );
            const effector & operator[](const string &id) const;
            
            
            //! sum of all the rates times the effector's factor, ADDED to dSdt
            void append(solution &dSdt, double t, double zeta, const solution &S, const solution &S_out) const;
            
            friend std::ostream & operator<<( std::ostream &, const effectors &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
        };
    }
    
}

#endif
