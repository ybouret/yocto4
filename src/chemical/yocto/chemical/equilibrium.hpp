#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::numeric<double>::function func_type;
        typedef math::derivative<double>        drvs_type;
        
        class equilibrium : public counted_object
        {
        public:
            typedef intr_ptr<string,equilibrium> pointer;
            typedef set<string,pointer>          database;
            
            virtual ~equilibrium() throw();
            
            const string  name; //!< key for database
            vslot         data; //!< for specific data
            func_type     K;    //!< a functor

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
            
            class d_instr : public counted_object
            {
            public:
                explicit d_instr(const size_t i,const size_t nu,const size_t nmax);
                virtual ~d_instr() throw();
                
                typedef arc_ptr<d_instr> pointer;
                
                const size_t  indx; //!< the index of derivated species
                const double  coef; //!< its former coefficient
                vector<instr> code; //!< the remaining instruction code
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(d_instr);
            };
            
            typedef d_instr::pointer j_instr;
            
            
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
            
            //! compile information, return Delta_r Nu_products
            int compile( array<double> &Nu, const collection &lib);

            //! wrapper for the virtual function
            double computeK(double) const;
            
            //! compute Gamma and initialize K
            double computeGamma(double t, const array<double> &C, double &KK);
            
            //! compute Gamma from a pre-computed constant
            double updateGamma(const array<double> &C, const double KK) const throw();
            
            //! compute Gamma and Phi while initializing K
            double computeGammaAndPhi( array<double> &Phi, double t, const array<double> &C, double &KK);
            
            //! compute dGamma/dt and Phi
            double computeGammaPrimeAndPhi( array<double> &Phi, double t, const array<double> &C, drvs_type &drvs, double h );
            
            //! compute Phi only
            void updatePhi( array<double> &Phi, const array<double> &C, const double KK) const throw();
            
            //! compute Gamma and Phi whith a pre-computed constant
            double updateGammaAndPhi( array<double> &Phi, const array<double> &C, const double KK) const throw();
            
            //! compute a scaling concentration
            double computeScalingConcentration(const double KK) const throw();
            
        protected:
            explicit equilibrium( const string &id );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            vector<actor>   actors;
            vector<instr>   r_code;  //!< instruction  for reactants
            vector<instr>   p_code;  //!< instruction  for products
            vector<j_instr> rj_code; //!< instruction  for Jacobian/reactants
            vector<j_instr> pj_code; //!< instructions for Jacobian/products
            
            void precompute( vector<j_instr> &jcode, const vector<instr> &code);
            
            void update_delta() throw();
            
        public:
            const int     DeltaNu; //!< to find the dimension of K
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
