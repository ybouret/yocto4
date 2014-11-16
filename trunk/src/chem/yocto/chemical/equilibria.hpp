#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/chemical/library.hpp"

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef vector<double>          vector_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        
        class equilibria
        {
        public:
            explicit equilibria() throw();
            virtual ~equilibria() throw();
            
            
            const size_t M; //!< compiled #species
            const size_t N; //!< compiled #equilibria
        
            vector_t     C;      //!< [M] local conc
            vector_t     dC;     //!< [M] local dC
            vector<bool> active; //!< [M] reactive species
            vector_t     Cneg;   //!< [M] store negative concentration
            
            vector_t     K;      //!< [N] constants
            vector_t     Gamma;  //!< [N] Gamma
            vector_t     xi;     //!< [N] extents
            imatrix_t    Nu;     //!< [NxM] current topology
            matrix_t     Phi;    //!< [NxM], dGamma/dC
            imatrix_t    Nu2;    //!< [NxN] Nu*Nu'
            imatrix_t    ANu2;   //!< [NxN] adjoint of Nu2
            ptrdiff_t    dNu2;   //!< determinant(Nu2)
            imatrix_t    iNu2;   //!< [NxM], More-Penrose pseudo inverse * det(Nu2)
            
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
            void enforce_limits() throw(); //!< update xi
            
            bool balance( array<double> &C0 );
            
            equilibrium       & operator[](size_t i) throw();
            const equilibrium & operator[](size_t i) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            vector<equilibrium::pointer> eqs;
            
            //! count negative concentration
            size_t count_negative() throw();
        };
        
    }
    
}


#endif
