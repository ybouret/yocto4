#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1


#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class equilibrium : public object, public counted
        {
        public:
            const string name;
            vslot        data;   //!< for derived classes            
            
            virtual ~equilibrium() throw();
            
            //! get a constant
            virtual double K( double t, double P, double T ) const throw() = 0;
            
            //! the name
            const string &key() const throw();
            
            
            typedef intrusive_ptr<string,equilibrium> ptr;
            typedef set<string,equilibrium::ptr>      db;
            
            //! species+coefficient
            class actor
            {
            public:
                const species::ptr spec;
                const int          coef;
                actor( const species::ptr &sp, const int stochio ) throw();
                ~actor() throw();
                actor(const actor &other ) throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(actor);
            };
            
            //! add a species
            void   add( const species::ptr &spec, const int coef );
            
            //! number of actors
            size_t count() const throw();
            
            //! output
            friend std::ostream & operator<<( std::ostream &, const equilibrium & );
            
            //! fill a matrix row
            void fill(array<ptrdiff_t> &nuR,
                      array<ptrdiff_t> &nuP) const throw();
            
        protected:
            vector<actor> actors; //!< 
            
            explicit equilibrium( const string &id );
            explicit equilibrium( const char   *id );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
        };
        
        
        class constant_equilibrium : public equilibrium
        {
        public:
            explicit constant_equilibrium( const string &id, const double Keq );
            explicit constant_equilibrium( const char   *id, const double Keq );
            virtual ~constant_equilibrium() throw();
            
            virtual double K( double t, double P, double T ) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(constant_equilibrium);
        };
        
    }
}


#endif
