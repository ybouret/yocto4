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
        
        void effectors:: compute_rate( YOCTO_CHEMICAL_EFFECTOR_ARGS )
        {
            const size_t M = lib.size();
            tmp.make(M,0.0);
            const size_t neff = size();
            iterator     iter = begin();
            for( size_t i=neff;i>0;--i,++iter)
            {
                const effector &eff = **iter;
                eff.call(tmp, t, zeta, S, S_out, lib);
                const double fac = eff.Vmax;
                for(size_t j=M;j>0;--j)
                {
                    dSdt[j] += fac * tmp[j];
                }
            }
        }
    
        
    }
}
