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
            
            //! instruction for Gamma evalutaion
            class instr
            {
            public:
                const size_t i; //!< index of species in collection
                const size_t p; //!< its power from nu
                instr(const size_t indx, const size_t coef) throw();
                ~instr() throw();
                instr(const instr &) throw();
                inline friend std::ostream & operator<<( std::ostream &os, const instr &I)
                {
                    os << 'C' << I.i << '^' << I.p;
                    return os;
                }
                static int compare( const instr &, const instr &) throw();
            private:
                YOCTO_DISABLE_ASSIGN(instr);
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
            
            //! compile information
            void compile( array<double> &Nu, const collection &lib);

            //! wrapper for the virtual function
            double computeK(double) const;
            
            //! compute Gamma and initialize K
            double computeGamma(double t, const array<double> &C, double &KK);
            double updateGamma(const array<double> &C, const double KK) const throw();
            
        protected:
            explicit equilibrium( const string &id );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            vector<actor> actors;
            vector<instr> r_code; //!< instruction for reactants
            vector<instr> p_code; //!< instruction for products
            
            void update_delta() throw();
            
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
