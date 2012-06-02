#include "yocto/aqueous/equilibrium.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace aqueous
    {
        
        equilibrium:: ~equilibrium() throw() {}
        
        
        equilibrium:: equilibrium( const string &id, const constant &eqK ) :
        name( id ),
        K (eqK ),
        actors(),
        reactants(),
        products(),
        tmpK(0)
        {
        }
        
        equilibrium:: equilibrium( const string &id, double eqK ) :
        name( id ),
        K( this, & equilibrium::getK ),
        actors(),
        reactants(),
        products(),
        tmpK( eqK )
        {
        }
        
        
        const string & equilibrium:: key() const throw() { return name; }
        
        double equilibrium:: getK(double) const throw() { return tmpK; }
        
        
        
        actor:: actor( const string &id, int stoch ) :
        name(id),
        nu(stoch)
        {
        }
        
        actor:: ~actor() throw() {}
        
        actor:: actor( const actor &a ) :
        name( a.name ),
        nu( a.nu )
        {
        }
        
        void equilibrium:: add( const string &id, int nu )
        {
            if( 0 == nu )
                throw exception("[%s]: invalid %d <%s>", name.c_str(), nu, id.c_str() );
            
            const actor A( name, nu );
            if( ! actors.insert(A) )
                throw exception("[%s]: multiple actor <%s>", name.c_str(), id.c_str() );
            
            try
            {
                if( nu > 0 )
                {
                    if( !products.insert(A) )
                        throw exception("[%s]: multiple product <%s>", name.c_str(), id.c_str() );
                }
                else
                {
                    if( !reactants.insert(A) )
                        throw exception("[%s]: multiple reactant <%s>", name.c_str(), id.c_str() );
                }
            }
            catch(...)
            {
                (void) actors.remove(name);
                throw;
            }
        }
        
        
    }
    
}
