#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include "yocto/math/types.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::numeric<double>::function func_type;
        
        class equilibrium : public counted_object
        {
        public:
            
            // an actor
            class actor
            {
            public:
                YOCTO_MAKE_OBJECT
                const species::pointer sp;
                const int              nu;
                actor                 *prev;
                actor                 *next;
                
                actor(const species::pointer &a_sp, const int a_nu) throw();
                ~actor() throw();
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(actor);
            };
            
            //__________________________________________________________________
            //
            // Administrativia
            //__________________________________________________________________
            //! default ctor
            explicit equilibrium(const string &id);
            
            //! default dtor
            virtual ~equilibrium() throw();
            
            const string name; //!< the name
            func_type    K;    //!< a functor, calling callK(), which calls the virtual getK()
            
            const string &key() const throw();
            typedef intr_ptr<string,equilibrium> pointer;
            typedef set<string,pointer>          database;
            
            void add( const species::pointer &sp, const int nu);
            void output( std::ostream &os ) const;
            friend inline std::ostream &operator<<(std::ostream &os, const equilibrium &eq) { eq.output(os); return os; }
            
            //__________________________________________________________________
            //
            // Chemistry
            //__________________________________________________________________
            void initialize( array<ptrdiff_t> &Nu ) const throw();
            
            
            //! compute Gamma and K(t)
            double computeGamma( double t, const array<double> &C, double &Kt ) const;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double getK(double t) const = 0;
            double callK(double) const;
            typedef core::list_of_cpp<actor> actors;
            actors reac;
            actors prod;
        };
        
        class const_equilibrium : public equilibrium
        {
        public:
            explicit const_equilibrium(const string &id, const double Kvalue );
            virtual ~const_equilibrium() throw();
            
            const double value;
            
        private:
            virtual double getK(double) const;
            YOCTO_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
        };
        
        
    }
}


#endif
