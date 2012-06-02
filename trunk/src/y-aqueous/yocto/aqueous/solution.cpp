#include "yocto/aqueous/solution.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        component:: ~component() throw()
        {
        }
        
        component:: component( const string &id ) :
        name(id),
        C(0)
        {
        }
        
        const string & component:: key() const throw() { return name; }
        
        component:: component( const component &other ) :
        name( other.name ),
        C( other.C )
        {
        }
        
        solution:: ~solution() throw() 
        {
        }
        
        solution:: solution( const library &L ) :
        lib(L),
        components( lib.size(), as_capacity ),
        size( lib.size() )
        {
            for( species::iterator i = lib.begin(); i != lib.end(); ++i )
            {
                const component tmp( (*i)->name );
                if( ! components.insert(tmp) )
                    throw exception("solution(unexpected multiple '%s')", tmp.name.c_str());
            }
        }
        
        solution:: solution( const solution &s ) :
        lib( s.lib ),
        components( s.components ),
        size( s.size )
        {
        }
        
        double &       solution:: operator[]( const string &name )
        {
            component *p = components.search( name );
            if( !p )
                throw exception("no solution['%s']",name.c_str());
            return p->C;
        }
        
        const double &  solution:: operator[]( const string &name ) const
        {
            const component *p = components.search( name );
            if( !p )
                throw exception("no solution['%s']",name.c_str());
            return p->C;
        }
        
        double &       solution:: operator[]( const char *id )
        {
            
            const string name(id);
            return (*this)[name];
        }
        
        const double & solution:: operator[]( const char *id ) const
        {
            
            const string name(id);
            return (*this)[name];
        }
        
        double solution:: sum_zC() const throw()
        {
            assert(size==lib.size());
            
            double                    ans = 0;
            component::const_iterator i   = components.begin();
            library::const_iterator   j   = lib.begin();
            
            for( size_t k=size; k>0; --k,++i,++j )
            {
                const component &X = *i;
                const species   &Y = **j;
                assert( X.name == Y.name );
                ans += Y.z * X.C;
            }
            
            return ans;
        }
        
        void solution:: put( array<double> &vec ) const throw()
        {
            assert(vec.size()>=size);
            component::const_iterator i   = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                vec[k] = (*i).C;
            }
        }
        
        void solution:: get( const array<double> &vec ) throw()
        {
            assert(vec.size()>=size);
            component::iterator i   = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                (*i).C = vec[k]; 
            }
            
        }
        
    }
    
}

