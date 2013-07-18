#ifndef YOCTO_CHEMICAL_INITIALIZER_INCLUDED
#define YOCTO_CHEMICAL_INITIALIZER_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef map<string,double> constraint_weights;
        
        class constraint : public constraint_weights
        {
        public:
            double   value;
            explicit constraint( const double v );
            virtual ~constraint() throw();
            
            typedef shared_ptr<constraint>   ptr;
            
            double & operator[]( const string & ); //!< created on the fly
            double & operator[]( const char   * );
            
            const double & operator[](const string &) const;
            const double & operator[](const char   *) const;
            
            friend std::ostream & operator<<(std::ostream &, const constraint &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(constraint);
        };
        
        typedef vector<constraint::ptr> constraints;

        class initializer : public constraints
        {
        public:
            explicit initializer() throw();
            virtual ~initializer() throw();
            
            //! append a new constraint, to be filled
            constraint & create( const double value );
            
            //! set the electroneutrality
            void electroneutrality( const collection &lib );
            
            void operator()( equilibria &cs, collection &lib, const double t );
            
            friend std::ostream & operator<<(std::ostream &, const initializer &);
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(initializer);
        };
        
        
    }
}

#endif
