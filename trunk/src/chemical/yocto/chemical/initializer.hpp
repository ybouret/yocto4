#ifndef YOCTO_CHEMICAL_INITIALIZER_INCLUDED
#define YOCTO_CHEMICAL_INITIALIZER_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace chemical
    {
        //! base type for constraint weights
        typedef map<string,double> constraint_weights;
        
        //! a constraint
        /**
         a constraint is a value plus a linear combination
         of the concentrations.
         */
        class constraint : public constraint_weights
        {
        public:
            const double   value;
            explicit constraint( const double v );
            virtual ~constraint() throw();
            
            typedef shared_ptr<constraint>   ptr;
            
            double & operator[]( const string & ); //!< weight created on the fly
            double & operator[]( const char   * ); //!< weight created on the fly
            
            
            const double & operator[](const string &) const; //!< get an existing weight
            const double & operator[](const char   *) const; //!< get an existion weight
            
            friend std::ostream & operator<<(std::ostream &, const constraint &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(constraint);
        };
        
        //! base class for a collection of constraints
        typedef vector<constraint::ptr> constraints;

        //! a set of constraints to init equilibria
        class initializer : public constraints
        {
        public:
            explicit initializer() throw();
            virtual ~initializer() throw();
            rand32_kiss ran; //!< initialized during construction
            
            //! append a new constraint, weights to be filled
            constraint & equals( const double value );
            
            //! helper: create a simple constraint of a species to a value
            void define( const string &id, const double value);
            
            //! wrapper
            void define( const char *id, const double value );
            
            //! set the electroneutrality
            /**
             Won't create a constraint if no species is charged.
             */
            void electroneutrality( const collection &lib );
            
            //! initialize at time t
            void operator()( equilibria &cs, collection &lib, const double t );
            
            friend std::ostream & operator<<(std::ostream &, const initializer &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(initializer);
            
        };
        
        
    }
}

#endif
