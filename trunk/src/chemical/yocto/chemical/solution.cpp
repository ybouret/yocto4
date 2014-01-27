#include "yocto/chemical/solution.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    
    namespace chemical
    {
        
        component:: ~component() throw() {}
        
        component:: component( const species::ptr &sp ) throw() :
        spec(sp),
        concentration(0)
        {
        }
        
        const string & component:: key() const throw()
        {
            return spec->name;
        }
        
        component:: component( const component &other ) throw() :
        spec(other.spec),
        concentration(other.concentration)
        {
            
        }
        
        solution:: solution( const collection &lib ) :
        components(  lib.size() ),
        composition( components, as_capacity),
        name_max(0)
        {
            size_t &the_max = (size_t &) name_max;
            for(collection::const_iterator i=lib.begin(); i != lib.end(); ++i )
            {
                const species::ptr &spec = *i;
                const component     comp(spec);
                if( !composition.insert(comp) )
                    throw exception("solution(unexpected failure for '%s')", spec->name.c_str());
                const size_t tmp = spec->name.size();
                if(tmp>the_max) the_max = tmp;
            }
        }
        
        solution:: solution( const solution &other ) :
        components(  other.components  ),
        composition( other.composition ),
        name_max(    other.name_max    )
        {
            
        }
        
        solution:: ~solution() throw()
        {
        }
        
        double & solution:: operator[]( const string &name )
        {
            component *p = composition.search(name);
            if(!p)
                throw exception("solution[no '%s']", name.c_str());
            return p->concentration;
        }
        
        const double & solution:: operator[]( const string &name ) const
        {
            const component *p = composition.search(name);
            if(!p)
                throw exception("solution[no '%s'] const", name.c_str());
            return p->concentration;
        }
        
        double & solution:: operator[]( const char *id )
        {
            const string name(id);
            return (*this)[name];
        }
        
        const double & solution:: operator[]( const char *id ) const
        {
            const string name(id);
            return (*this)[name];
        }
        
        double solution::pH() const
        {
            return -math::Log10( (*this)["H+"] );
        }
        
        void solution:: load( const array<double> &C ) throw()
        {
            assert(C.size()>=composition.size());
            size_t j=0;
            for( component::db::iterator i=composition.begin(); i != composition.end(); ++i )
            {
                component &comp = *i;
                comp.concentration = C[++j];
            }
        }
        
              
        void solution:: save( array<double> &C ) const throw()
        {
            assert(C.size()>=composition.size());
            size_t j=0;
            for( component::db::const_iterator i=composition.begin(); i != composition.end(); ++i )
            {
                const component &comp = *i;
                C[++j] = comp.concentration;
            }
        }
        
        std::ostream & operator<<( std::ostream &os, const solution &s)
        {
            os << "{" << std::endl;
            size_t j=0;
            for( component::db::const_iterator i=s.composition.begin(); i != s.composition.end(); ++i )
            {
                const component &comp = *i;
                const string    &name = comp.spec->name;
                os << "\t[" << name << "]";
                assert(name.size()<=s.name_max);
                for(size_t i=s.name_max-comp.spec->name.size();i>0;--i) os << ' ';
                os << " : " << comp.concentration;
                if(++j<s.components) os << ",";
                os << std::endl;
            }
            os << "}";
            return os;
        }
        
        component::db::iterator solution::begin() throw() { return composition.begin(); }
        component::db::iterator solution::end()   throw() { return composition.end();   }
        component::db::const_iterator solution::begin() const throw() { return composition.begin(); }
        component::db::const_iterator solution::end()   const throw() { return composition.end();   }
        
        void solution:: ldz() throw()
        {
            for( iterator i=begin(); i != end(); ++i )
                (*i).concentration = 0;
        }
        
        double solution:: sum_zC() const throw()
        {
            double ans = 0;
            for( const_iterator i = begin(); i != end(); ++i)
            {
                const component &p = *i;
                ans += p.concentration * p.spec->z;
            }
            return ans;
        }
        
        double & solution:: operator[]( size_t i ) throw()
        {
            assert(i>0);
            assert(i<=components);
            iterator j = begin();
            for(size_t k=1;k<i;++k) ++j;
            return (*j).concentration;
        }
        
        const double & solution:: operator[]( size_t i ) const throw()
        {
            assert(i>0);
            assert(i<=components);
            const_iterator j = begin();
            for(size_t k=1;k<i;++k) ++j;
            return (*j).concentration;
        }
        
        
        void solution:: write_header( ios::ostream &os ) const
        {
            for( const_iterator i = begin(); i != end(); ++i)
            {
                os.write(' ');
                os << (*i).spec->name;
            }
        }
        
        void solution:: write_values( ios::ostream &os ) const
        {
            for( const_iterator i = begin(); i != end(); ++i)
            {
                os(" %.15g", (*i).concentration);
            }
        }
        
        component * solution:: lookup( const string &id) throw()
        {
            return composition.search(id);
        }

        
    }
    
}

