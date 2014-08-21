#include "yocto/chemical/effector.hpp"

namespace yocto
{
    namespace chemical
    {
        effector:: ~effector() throw()
        {
        }
     
        effector::effector( const string &id ) :
        name(id),
        Vmax(1)
        {
            
        }
        
        
        //======================================================================
        //
        // Effectors
        //
        //======================================================================
        effectors:: effectors() throw() : effector::database()
        {
            
        }
        
        effectors:: ~effectors() throw()
        {
            
        }
        
        void effectors:: compute_rate( YOCTO_CHEMICAL_EFFECTOR_ARGS, solution &Stmp )
        {
            const size_t M = dSdt.size();
            dSdt.ldz();
            const size_t neff = size();
            iterator     iter = begin();
            for( size_t i=neff;i>0;--i,++iter)
            {
                const effector &eff = **iter;
                eff.call(Stmp, t, zeta, S, S_out);
                const double fac = eff.Vmax;
                for(size_t j=M;j>0;--j)
                {
                    dSdt[j] += fac * Stmp[j];
                }
            }
        }
    
        
    }
}
