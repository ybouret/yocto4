#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"

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
                const species::pointer sp;
                const int              nu;
                actor(const species::pointer &a_sp, const int a_nu) throw();
                ~actor() throw();
                actor(const actor &a) throw();
                
                static int compare( const actor &lhs, const actor &rhs ) throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(actor);
            };
            
            //! default ctor
            explicit equilibrium(const string &id);
            
            //! default dtor
            virtual ~equilibrium() throw();
            
            const string name; //!< the name
            func_type    K;    //!< a functor, calling callK(), which calls the virtual getK()
            
            const string &key() const throw();
            typedef intr_ptr<string,equilibrium> pointer;
            typedef set<string,equilibrium>      database;
            
            void add( const species::pointer &sp, const int nu);
            void output( std::ostream &os ) const;
            friend inline std::ostream &operator<<(std::ostream &os, const equilibrium &eq) { eq.output(os); return os; }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double getK(double t) const = 0;
            double callK(double) const;
            vector<actor> actors;
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
