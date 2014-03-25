#include "yocto/aqueous/solution.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/types.hpp"

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
        
        std::ostream & operator<<( std::ostream &os, const solution &s )
        {
            size_t max_len = 0;
            {
                for( component::const_iterator i   = s.components.begin(); i != s.components.end(); ++i )
                {
                    const size_t len = (*i).name.size();
                    if( len > max_len ) max_len = len;
                }
            }
            
            {
                component::const_iterator i   = s.components.begin();
                os << "/--------" << std::endl;
                for( size_t k=1; k <= s.size; ++k, ++i )
                {
                    const component &C = *i;
                    os << "| [";
                    os << C.name.c_str();
                    os << "]";
                    for( size_t j=max_len-C.name.size();j>0;--j)
                        os << ' ';
                    os << " = " << C.C << std::endl;
                }
                os << "\\--------";
            }
            return os;
        }
        
        void solution:: mul( double a ) throw()
        {
            component::iterator i     = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                (*i).C *= a;
            }
        }
        
        void solution:: ldz() throw()
        {
            component::iterator i     = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                (*i).C = 0;
            }
        }
        
        
        double solution:: pH() const
        {
            return -log10( (*this)["H+"] );
        }
        
        
        component::iterator solution::begin()
        {
            return components.begin();
        }
        
        
        component::iterator solution::end()
        {
            return components.end();
        }
        
        component::const_iterator solution::begin() const
        {
            return components.begin();
        }
        
        
        component::const_iterator solution::end() const
        {
            return components.end();
        }
        
        double       & solution:: operator()( size_t i) throw()
        {
            return components(i).C;
        }
        
        const double       & solution:: operator()( size_t i) const throw()
        {
            return components(i).C;
        }
        
        
        void solution:: save_header( ios::ostream &fp ) const
        {
            component::const_iterator i     = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                fp(" \"%s\"", (*i).name.c_str());
            }
        }
        
        void solution:: save_values( ios::ostream &fp ) const
        {
            component::const_iterator i     = components.begin();
            for( size_t k=1; k <= size; ++k, ++i )
            {
                fp(" %.15g", (*i).C);
            }
        }
        
        void solution:: copy( const solution &other )
        {
            assert( other.size == size );
            component::iterator       p = components.begin();
            component::const_iterator q = other.components.begin();
            for( size_t k=size;k>0;--k,++p,++q)
            {
                assert( (*p).name == (*q).name );
                (*p).C = (*q).C;
            }
        }
        
    }
    
}

