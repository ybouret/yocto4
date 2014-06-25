#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::numeric<double>::function function_type;
        
        class equilibrium : public counted_object
        {
        public:
            typedef intr_ptr<string,equilibrium> pointer;
            typedef set<string,pointer>          database;
            
            virtual ~equilibrium() throw();
            
            const string  name; //!< key for database
            vslot         data; //!< for specific data
            function_type K;    //!< a functor

            const string & key() const throw();
            
            //! one actor
            class actor
            {
            public:
                actor( const species::pointer &spec, const int coef) throw();
                actor(const actor &other) throw();
                
                ~actor() throw();
                const species::pointer sp;
                const int              nu;
                
                static int compare( const actor &lhs, const actor &rhs) throw();
                               
            private:
                YOCTO_DISABLE_ASSIGN(actor);
            };
            
            //! check validity, best effort
            /**
             - charge conservation...
             */
            bool is_valid() const throw();
            
            //! output at time=0
            void output( std::ostream & ) const;
            
            inline friend std::ostream &operator<<(std::ostream &os,const equilibrium &eq)
            {
                eq.output(os);
                return os;
            }
            
            //! add an actor
            void add( const species::pointer &spec, const int coef );
            
            //! get the constant at a given time
            virtual double getK( double t ) const = 0;
            
            //! store info
            void fill( array<double> &Nu, array<ptrdiff_t> &NuR, array<ptrdiff_t> &NuP) const throw();
            
        protected:
            explicit equilibrium( const string &id );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            vector<actor> actors;
            void update_delta() throw();
            double computeK(double) const;
            
        public:
            const int     DeltaNu;
        };
        
        
        class const_equilibrium : public equilibrium
        {
        public:
            explicit const_equilibrium( const string &id, const double K );
            virtual ~const_equilibrium() throw();
            
            virtual double getK( double ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
            
        };
    }
}

#endif
