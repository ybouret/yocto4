#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/chemical/library.hpp"

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef vector<double>          vector_t;
        typedef vector<bool>            bvector_t;
        typedef vector<size_t>          uvector_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        
        typedef vector<equilibrium::pointer> evector_t;
        
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
            uvector_t    Ineg;   //!< [0..M] indices of negative concentration
            uvector_t    Isub;   //!< [0..M] indices of involved concentrations for balancing
            bvector_t    flag;   //!< [M] flag for ops
            
            vector_t     K;      //!< [N] constants
            vector_t     Gamma;  //!< [N] Gamma
            vector_t     xi;     //!< [N] extents
            imatrix_t    Nu;     //!< [NxM] current topology
            matrix_t     Phi;    //!< [NxM], dGamma/dC
            evector_t    online; //!< [0..N], online eqs for balancing
            equilibrium &add( equilibrium *pEq );
            void         remove(const string &name);
            
            void compile_for( const library &lib );
            
            void clear() throw();
            
            void output( std::ostream &os ) const;
            friend inline std::ostream & operator<<( std::ostream &os, const equilibria &eqs )
            {
                eqs.output(os);
                return os;
            }
            
            void compute_limits() throw(); //!< from internal C
            
            bool balance( array<double> &C0 );
            
            equilibrium       & operator[](size_t i) throw();
            const equilibrium & operator[](size_t i) const throw();
            
            double computeE( array<double> &dEdC );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            vector<equilibrium::pointer> eqs;
            
            //! count negative concentration
            size_t count_negative() throw();
        };
        
    }
    
}


#endif
