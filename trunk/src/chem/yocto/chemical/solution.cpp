#include "yocto/chemical/solution.hpp"
#include "yocto/exception.hpp"
#include <iostream>
#include <cmath>

namespace yocto
{
    namespace chemical
    {
        
        solution:: ~solution() throw()
        {
            memory::kind<memory::global>::release_as(++C,m);
            lib.liberate();
        }
        
        solution:: solution(  library &from ) :
        lib(from),
        n( lib.size() ),
        m(n),
        C( memory::kind<memory::global>::acquire_as<double>(m) - 1 )
        {
            lib.withhold();
        }
        
        solution::solution(const solution &other) :
        lib( other.lib ),
        n( lib.size()  ),
        m(n),
        C( memory::kind<memory::global>::acquire_as<double>(m) - 1 )
        {
            lib.withhold();
            for(size_t i=n;i>0;--i) C[i] = other.C[i];
        }
        
        
        
        size_t solution:: size() const throw()
        {
            return n;
        }
        
        size_t solution:: capacity() const throw()
        {
            return n;
        }
        
        const char *solution:: name() const throw()
        {
            return "chemical::solution";
        }
        
        const double * solution:: get_item() const throw()
        {
            return C;
        }
        
        void solution::reserve(size_t)
        {
            throw exception("%s: no possible reserve", name());
        }
        
        void solution:: ldz() throw()
        {
            for(size_t i=n;i>0;--i) C[i] = 0;
        }
        
        
        void solution:: free() throw()
        {
            ldz();
        }
        
        void solution:: release() throw()
        {
            ldz();
        }
        
        double & solution:: operator()( const string &id )
        {
            const species::pointer &p = lib[id];
            assert(p->indx>=1);
            assert(p->indx<=n);
            return C[p->indx];
        }
        
        const double & solution:: operator()( const string &id ) const
        {
            const species::pointer &p = lib[id];
            assert(p->indx>=1);
            assert(p->indx<=n);
            return C[p->indx];
        }
        
        
        double       & solution:: operator()( const char   *id )
        {
            const string ID(id);
            return (*this)(ID);
        }
        
        const double       & solution:: operator()( const char   *id ) const
        {
            const string ID(id);
            return (*this)(ID);
        }
        
        
        void solution:: output(std::ostream &os) const
        {
            os << "{" << std::endl;
            library::iterator j = lib.begin();
            for(size_t i=1;i<=n;++i,++j)
            {
                const species &sp = **j;
                assert(sp.indx==i);
                os << '\t' << '[' << sp.name << ']';
                for(size_t k=sp.name.size();k<lib.max_name_length;++k) os  << ' ';
                os << " = " << C[i] << std::endl;
            }
            os << "}" << std::endl;
        }
        
        void solution:: load( const array<double> &X) throw()
        {
            assert(X.size()>=size());
            for(size_t i=n;i>0;--i) C[i] = X[i];
        }
        
        void solution::save( array<double> &X ) const throw()
        {
            assert(X.size()>=size());
            for(size_t i=n;i>0;--i) X[i] = C[i];
        }
        
        
        solution & solution :: operator=( const solution & other )
        {
            if( &lib != &other.lib)
                throw exception("solutions from different collection");
            assert(this->size()==other.size());
            for(size_t i=n;i>0;--i) C[i] = other.C[i];
            return *this;
        }
        
        solution & solution :: operator=( const array<double> &X ) throw()
        {
            load(X);
            return *this;
        }
        
        double solution:: pH() const
        {
            const species::pointer &p = lib["H+"];
            const array<double>    &X = *this;
            return -log10(X[p->indx]);
        }
        
        double solution:: charge() const throw()
        {
            double ans = 0;
            library::iterator j=lib.begin();
            for(size_t i=1;i<=n;++i,++j)
            {
                ans += C[i] * (**j).z;
            }
            return ans;
        }
        
        double solution:: osm() const throw()
        {
            double ans = 0;
            for(size_t i=n;i>0;--i)
            {
                ans += C[i];
            }
            return ans;
        }

        double solution:: ionic_strength() const throw()
        {
            double ans = 0;
            library::iterator j=lib.begin();
            for(size_t i=1;i<=n;++i,++j)
            {
                const int z =(**j).z;
                ans += C[i] * (z*z);
            }
            return ans;
        }

        
        
        
    }
}
