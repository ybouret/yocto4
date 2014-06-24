#include "yocto/chemical/solution.hpp"
#include "yocto/exception.hpp"
#include <iostream>

namespace yocto
{
    namespace chemical
    {
        
        solution:: ~solution() throw()
        {
            memory::kind<memory::global>::release_as(++C,m);
            lib.decrease();
        }
        
        solution:: solution( const collection &from ) :
        lib(from),
        n( lib.size() ),
        m(n),
        C( memory::kind<memory::global>::acquire_as<double>(m) - 1 )
        {
            lib.increase();
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
        
        
        void solution:: free() throw()
        {
            for(size_t i=1;i<=n;++i) C[i] = 0;
        }
        
        void solution:: release() throw()
        {
            this->free();
        }

        double & solution:: operator[]( const string &id )
        {
            const species::pointer &p = lib[id];
            assert(p->indx>=1);
            assert(p->indx<=n);
            return C[p->indx];
        }
        
        const double & solution:: operator[]( const string &id ) const
        {
            const species::pointer &p = lib[id];
            assert(p->indx>=1);
            assert(p->indx<=n);
            return C[p->indx];
        }
        
        void solution:: output(std::ostream &os) const
        {
            os << "{" << std::endl;
            collection::const_iterator j=lib.begin();
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
        
    }
}
