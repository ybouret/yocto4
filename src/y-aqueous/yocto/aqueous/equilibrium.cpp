#include "yocto/aqueous/equilibrium.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/ipower.hpp"

namespace yocto 
{
    
    namespace aqueous
    {
        
        equilibrium:: ~equilibrium() throw() {}
        
        
        equilibrium:: equilibrium( const library &L, const string &id, const constant &eqK ) :
        name( id ),
        K (eqK ),
        actors(),
        reactants(),
        products(),
        tmpK(0),
        lib(L)
        {
        }
        
#if defined(_MSC_VER)
		// this in ctor
#pragma warning ( disable : 4355 )
#endif
        equilibrium:: equilibrium( const library &L, const string &id, double eqK ) :
        name( id ),
        K( this, & equilibrium::getK ),
        actors(),
        reactants(),
        products(),
        tmpK( eqK ),
        lib(L)
        {
        }
        
        
        const string & equilibrium:: key() const throw() { return name; }
        
        double equilibrium:: getK(double) const throw() { return tmpK; }
        
        
        actor:: actor(const string &id, 
                      int           stoch ) :
        name(id),
        nu(stoch)
        {
        }
        
        actor:: ~actor() throw() {}
        
        actor:: actor( const actor &a ) :
        name( a.name ),
        nu(   a.nu   )
        {
        }
        
        const string & actor:: key() const throw() { return name; }
        
        void equilibrium:: add( const string &id, int nu )
        {
            
            if( ! lib.search(id) )
            {
                throw exception("[%s]: unknown  {%s}", name.c_str(), id.c_str() );
            }
            
            
            if( 0 == nu )
                throw exception("[%s]: invalid %d {%s}", name.c_str(), nu, id.c_str() );
            
            const actor A( id, nu ); 
            if( ! actors.insert(A) )
                throw exception("[%s]: multiple actor {%s}", name.c_str(), id.c_str() );
            
            try
            {
                if( nu > 0 )
                {
                    if( !products.insert(A) )
                        throw exception("[%s]: multiple product {%s}", name.c_str(), id.c_str() );
                }
                else
                {
                    (int&)A.nu = -A.nu;
                    if( !reactants.insert(A) )
                        throw exception("[%s]: multiple reactant {%s}", name.c_str(), id.c_str() );
                }
            }
            catch(...)
            {
                (void) actors.remove(name);
                throw;
            }
        }
        
        void equilibrium:: add( const char   *id, int nu )
        {
            const string ID(id);
            add(ID,nu);
        }
        
        double equilibrium:: Gamma( const solution &s, double t ) const  
        {
            double lhs = K(t);
            double rhs = 1;
            for( actor::db::const_iterator r = reactants.begin(); r != reactants.end(); ++r )
            {
                const actor &a = *r; assert(a.nu>0);
                lhs *= ipower(s[ a.name ],a.nu);
            }
            
            for( actor::db::const_iterator p = products.begin(); p != products.end(); ++p )
            {
                const actor &a = *p; assert(a.nu>0);
                rhs *= ipower(s[ a.name ],a.nu);
            }
            
            return lhs - rhs;
        }
        
        void equilibrium:: fill( array<double> &row_nu, array<ptrdiff_t> &row_nuR, array<ptrdiff_t> &row_nuP ) const throw()
        {
            assert( row_nu.size()  == lib.size() );
            assert( row_nuR.size() == lib.size() );
            assert( row_nuP.size() == lib.size() );
            
            for( size_t j=1; j <= lib.size(); ++j )
            {
                const string &id = lib(j)->name;
                const actor *pA  = actors.search(id);
                if( pA )
                {
                    const int nu = pA->nu; assert(nu!=0);
                    row_nu[j] = nu;
                    if( nu > 0 ) 
                    {
                        row_nuP[j] = nu;
                    }
                    else
                    {
                        row_nuR[j] = -nu;
                    }
                }
                else 
                {
                    row_nu[j]  = 0;
                    row_nuR[j] = 0;
                    row_nuP[j] = 0;
                }
            }
            
        }
        
        
        std::ostream & operator<<( std::ostream &os, const equilibrium &eq )
        {
            os << "[" << eq.name << "] : ";
            for( actor::db::const_iterator r = eq.reactants.begin(); r != eq.reactants.end(); ++r )
            {
                const actor &a = *r; assert(a.nu>0);
                os << a.nu << "{" << a.name << "} ";
            }
            os << "<=>";
            for( actor::db::const_iterator p = eq.products.begin(); p != eq.products.end(); ++p )
            {
                const actor &a = *p; assert(a.nu>0);
                os << a.nu << "{" << a.name << "} ";
            }
            os << "| K=" << eq.K(0);
            return os;
        }
        
    }
    
}
