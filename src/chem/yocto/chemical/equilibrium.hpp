#ifndef YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include "yocto/core/list.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/types.hpp"
#include "yocto/math/fcn/drvs.hpp"


namespace yocto
{
    namespace chemical
    {
        typedef math::numeric<double>::function func_type;
        typedef math::derivative<double>        drvs_type;
        typedef ptrdiff_t                       integer_t;

        
        
        class equilibrium : public counted_object
        {
        public:
            typedef arc_ptr<equilibrium> pointer;
            
            //! an actor
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
            
            YOCTO_TRIPLE_DECL(YOCTO_TUPLE_STANDARD,
                              xi_ctrl,
                              bool,   blocked,
                              bool,   limited,
                              double, maximum);
            YOCTO_TRIPLE_END();
            
            //__________________________________________________________________
            //
            // Administrativia
            //__________________________________________________________________
            //! default ctor
            explicit equilibrium(const string &id);
            
            //! default dtor
            virtual ~equilibrium() throw();
            
            const string   name;        //!< the name
            func_type      K;           //!< a functor, calling callK(), which calls the virtual getK()
            mutable double reac_weight; //!< last value
            mutable double prod_weight; //!< last value
            xi_ctrl        forward;     //!< status
            xi_ctrl        reverse;     //!< status
            bool           blocked;     //!< fully blocked !
            const size_t   indx;        //!< index in equilibria
            
            void add( const species::pointer &sp, const int nu);
            void output( std::ostream &os ) const;
            friend inline std::ostream &operator<<(std::ostream &os, const equilibrium &eq) { eq.output(os); return os; }
            
            //__________________________________________________________________
            //
            // Chemistry
            //__________________________________________________________________
            //! best effort validation
            void validate() const;
            
            void initialize( array<ptrdiff_t> &Nu, array<bool> &active ) const throw();
            
            
            //! compute Gamma and K(t)
            double computeGamma( double t, const array<double> &C, double &Kt ) const;
            
            //! update Gamma from a previous K(t)
            double updateGamma( const array<double> &C, const double Kt ) const;
            
            //! compute Gamma, Phi and K(t)
            double computeGammaAndPhi( double t, const array<double> &C, double &Kt, array<double> &Phi ) const;
            
            //! update Gamma and Phi from a previous K(t)
            double updateGammaAndPhi( const array<double> &C, const double Kt, array<double> &Phi ) const;
            
            
            //! max extents
            void compute_limits( const array<double> &C )  throw();
            
            //! display computed limits
            void show_limits( std::ostream &os ) const;
            
            //! correct the advancing
            double apply_limits( const double xi ) const throw();
            
            //! Does species #indx is involved
            bool involves(const size_t indx) const throw();
            
            //! collect all involved species
            void collect( vector<size_t> &involved ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double getK(double t) const = 0;
            double callK(double) const;
            typedef core::list_of_cpp<actor> actors;
            actors reac;
            actors prod;
            static  bool HasConverged(const double lhs, const double rhs) throw();
            
        public:
            const int      SumNuP;   //!< sum of products coefficients
            const int      DeltaNu;  //!< created #mols
            double compute_scaling(const double Kt) const throw();
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
