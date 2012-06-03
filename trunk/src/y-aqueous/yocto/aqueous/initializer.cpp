#include "yocto/aqueous/initializer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        constraint::~ constraint() throw() {}
        
        constraint:: constraint( const library &L, double v ) throw() :
        value(0), W(), lib(L) {}
        
        void constraint:: add( const string &id, double w )
        {
            if( ! lib.search(id) )
                throw exception("constraint( no '%s' )", id.c_str() );
            
            if( !W.insert(id, w) )
                throw exception("constraint( multiple '%s' )", id.c_str() );
        }
        
        
        initializer:: ~initializer() throw()
        {
        }
        
        initializer:: initializer( const library &L ) throw() :
        constraints(),
        lib(L)
        {
        }
        
        
        constraint & initializer:: create( double v )
        {
            constraint::ptr p( new constraint(lib,v) );
            constraints.push_back(p);
            return *p;
        }
        
        void initializer:: operator()( chemsys &cs )
        {
            std::cerr << "** Initializing system" << std::endl;
            array<double> &C  = cs.C;
            const size_t   M  = C.size();
            const size_t   N  = cs.size();
            const size_t   Nc = constraints.size();
            if( Nc+N != M )
                throw exception("#constraints mismatch!");
            
            //------------------------------------------------------------------
            // decode constraints
            //------------------------------------------------------------------
            std::cerr << "** Decoding Linear Constraints" << std::endl;
            matrix<double> P(Nc,M);
            vector<double> V(Nc,0.0);
            for( size_t i=1; i <= Nc; ++i )
            {               
                const constraint &c = *(constraints[i]);
                V[i] = c.value;
                for( size_t j=1; j <= M; ++j )
                {
                    const species &sp = *lib(j);
                    const double  *pW = c.W.search( sp.name );
                    if( pW )
                        P[i][j] = *pW;
                }
            }
            
            
        }
        
        
    }
    
}
