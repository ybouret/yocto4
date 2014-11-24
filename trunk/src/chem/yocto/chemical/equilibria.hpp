#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/chemical/library.hpp"

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/code/rand32.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>         matrix_t;
        typedef vector<double>               vector_t;
        
        typedef vector<bool>                 bvector_t;
        typedef vector<size_t>               uvector_t;
        typedef math::matrix<ptrdiff_t>      imatrix_t;
        typedef vector<equilibrium::pointer> evector_t;
        typedef vector<ptrdiff_t>            ivector_t;
        
        
        class boot;
        
        class equilibria
        {
        public:
            explicit equilibria() throw();
            virtual ~equilibria() throw();
            
            
            const size_t M; //!< compiled #species
            const size_t N; //!< compiled #equilibria
        
            vector_t     C;      //!< [M] local conc
            vector_t     dC;     //!< [M] local dC
            bvector_t    active; //!< [M] reactive species
            vector_t     Cs;     //!< [M] starting point for probing
            uvector_t    Jneg;   //!< [0..M] indices of negative concentration
            ivector_t    beta;   //!< [M]    balancing: dE/dC
            ivector_t    dCp;    //!< [M]    balancing: integer descent direction
            vector_t     alpha;  //!< [0..M] balancing: max scaling
            uvector_t    aindx;  //!< [0..M] balancing: associated #species
            
            vector_t     K;      //!< [N] constants
            vector_t     Gamma;  //!< [N] Gamma
            vector_t     GamSF;  //!< [N] Gamma Scaling Factor
            vector_t     xi;     //!< [N] extents
            imatrix_t    Nu;     //!< [NxM] current topology
            matrix_t     Phi;    //!< [NxM] dGamma/dC
            matrix_t     W;      //!< [NxN]
            evector_t    online; //!< [0..N] balancing: online eqs for balancing
            ivector_t    xip;    //!< [N]    balancing: integer descent extent
            
            
            
            equilibrium &add( equilibrium *pEq );
            void         remove(const string &name);
            
            void compile_for( const library &lib );
            void restore_topology();
            
            void clear() throw();
            
            void output( std::ostream &os ) const;
            friend inline std::ostream & operator<<( std::ostream &os, const equilibria &eqs )
            {
                eqs.output(os);
                return os;
            }
            
            //! compute all limits for equilibria
            void compute_limits() throw();
            
            //! balance concentration
            bool balance( array<double> &C0 );
            
            equilibrium       & operator[](size_t i) throw();
            const equilibrium & operator[](size_t i) const throw();
            
            //! compute all constantes
            void computeK(double t);
            void computeGammaScaling();
            void computeGammaAndPhi(double t);
            void updateGammaAndPhi();
            void updateGamma();
            
            
            //!  normalize AFTER a balancing was done..
            bool normalize( array<double> &C0, double t, bool recomputeK );
            
            //! clip xi according to C
            void clip_extents();
            
            
            void solve( const boot &loader, const double t);
            
            void load( const boot &loader, const double t );
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            vector<equilibrium::pointer> eqs;
            
            
            //! evaluate energy and compute descent direction
            double computeE( array<ptrdiff_t> & );
            bool   rebalance_with(const imatrix_t &Q, const bvector_t &local_active);
            
            
        public:
            uniform_generator<double> ran;
            
            
        };
        
    }
    
}


#endif
