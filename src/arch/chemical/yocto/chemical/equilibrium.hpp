#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1


#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand32.hpp"

namespace yocto
{
    namespace chemical
    {
        
        //! an equilibrium
        class equilibrium : public object, public counted
        {
        public:
            const string name;   //!< unique identifier ("water", "acid",...)
            vslot        data;   //!< for derived classes            
            
            virtual ~equilibrium() throw();
            
            //! get a constant at a given time
            virtual double getK( double t ) const = 0;
            
            //! the name
            const string &key() const throw();
            
            
            typedef intr_ptr<string,equilibrium> ptr; //!< smart pointer
            typedef set<string,equilibrium::ptr> db;  //!< database
            
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
            
            //! add a species with its stochiometric coefficient
            /**
             the species pointer should come from a library.
             */
            void   add( const species::ptr &spec, const int coef );
            
            //! number of actors
            size_t count() const throw();
            
            //! output
            friend std::ostream & operator<<( std::ostream &, const equilibrium & );
            
            //! fill a matrix row
            void fill(array<ptrdiff_t> &nuR,
                      array<ptrdiff_t> &nuP) const throw();

            //! compute the scaled concentration C
            void scale(double t) const;

            //! append scaled concentration
            /**
             the scaling must be done before
             */
            void append( array<double> &C, urand32 &ran ) const throw();
            
            
            //! check current validity
            /**
             - at least one species
             - charge conservation
             */
            bool is_valid() const throw();
            
        protected:
            explicit equilibrium( const string &id );
            explicit equilibrium( const char   *id );
            double   computeK(double) const;
            
        private:
            vector<actor> actors; //!< sorted vector of actors
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            
        public:
            const   int      Nu;  //!< sum of algebraic coefficients (Delta_r nu)
            const   unsigned NuR; //!< sum of algebraic reactives coefficients (Delta_r nu_r)
            const   unsigned NuP; //!< sum of algebraic products  coefficients (Delta_r nu_p)
            mutable double   C0;  //!< used to compute typical C scaling if Nu!=0, otherwise: 1
            mutable math::numeric<double>::function K;
            
        };
        
        
        class constant_equilibrium : public equilibrium
        {
        public:
            explicit constant_equilibrium( const string &id, const double Keq );
            explicit constant_equilibrium( const char   *id, const double Keq );
            virtual ~constant_equilibrium() throw();
            
            virtual double getK( double t ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(constant_equilibrium);
        };
        
    }
}


#endif
