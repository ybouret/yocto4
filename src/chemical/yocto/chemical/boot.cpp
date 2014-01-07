#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: constituent:: constituent(const species::ptr &sp,
                                         const double        w ) throw() :
        spec(sp),
        weight(w)
        {
            assert(fabs(w)>0);
        }
        
        boot:: constituent:: constituent( const constituent &other ) throw() :
        spec( other.spec ),
        weight( other.weight )
        {
        }
        
        boot:: constituent:: ~constituent() throw() {}
        
        const string & boot:: constituent:: key() const throw()
        {
            return spec->name;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: constraint:: constraint( const double v ) throw() :
        constituents(),
        value(v)
        {
        }
        
        boot:: constraint:: ~constraint() throw() {}
        
        boot::constraint & boot:: constraint:: weight(const species::ptr &sp, const double w)
        {
            const constituent::ptr p( new constituent(sp,w) );
            if(!insert(p))
                throw exception("constraint: multiple constituent '%s'", sp->name.c_str());
            return *this;
        }
        
        std::ostream & operator<<( std::ostream &os, const boot::constraint &cstr)
        {
            os << cstr.value << " =";
            for( boot::constraint::const_iterator i=cstr.begin(); i != cstr.end(); ++i)
            {
                const boot::constituent &it = **i;
                const double             w  = it.weight;
                if(w>0)
                    os << " + " << w;
                else
                    os << " - " << -w;
                
                os << "*[" << it.spec->name << "]";
            }
            return os;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: loader:: loader() throw() : constraints(), ran()
        {
            ran.wseed();
        }
        
        boot:: loader:: ~loader() throw() {}
        
        
        boot::constraint & boot:: loader:: add(double value)
        {
            const constraint::ptr p( new constraint(value) );
            push_back(p);
            return *back();
        }
        
        std::ostream & operator<<( std::ostream &os, const boot::loader &ld)
        {
            static const size_t nn = 63;
            os << '/'; for(size_t i=nn;i>0;--i) os << '-'; os << std::endl;
            for(size_t i=1; i <= ld.size();++i)
            {
                os << "| " << *ld[i] << std::endl;
            }
            os << '\\'; for(size_t i=nn;i>0;--i) os << '-';
            return os;
        }
        
        void boot::loader:: define( const species::ptr &sp, const double conc)
        {
            (void) add(conc).weight(sp, 1);
        }
        
        void boot::loader:: conserve( const species::ptr &A, const species::ptr &B, const double conc)
        {
            (void) add(conc).weight(A,1).weight(B,1);
        }

        void boot::loader:: conserve( const species::ptr &A, const species::ptr &B,  const species::ptr &C, const double conc)
        {
            (void) add(conc).weight(A,1).weight(B,1).weight(C,1);
        }
        
        void boot::loader:: electroneutrality(const collection &lib)
        {
            size_t num_charged = 0;
            for( collection::const_iterator i=lib.begin(); i!=lib.end();++i)
            {
                if( (**i).z != 0 )
                    ++num_charged;
            }
            if(num_charged<=0)
                return;
            
            constraint &Q = add(0);
            for( collection::const_iterator i=lib.begin(); i!=lib.end();++i)
            {
                const species::ptr &sp = *i;
                const int           z  = sp->z;
                if(z!=0)
                    Q.weight(sp, sp->z);
            }
        }
        
        void boot::loader:: fill( matrix_t &P, array<double> &Lam) const throw()
        {
            const size_t Nc = size();
            assert(P.rows==Nc);
            assert(Lam.size()==Nc);
            for(size_t i=1;i<=Nc;++i)
            {
                const constraint &cstr = *(*this)[i];
                array<double>    &P_i  = P[i];
                Lam[i] = cstr.value;
                for( constraint::const_iterator j=cstr.begin(); j!=cstr.end();++j)
                {
                    const constituent &cc = **j;
                    const size_t k = cc.spec->indx;
                    const double w = cc.weight;
                    assert(k>=1);
                    assert(k<=P.cols);
                    P_i[k] = w;
                }
            }
            
        }

        
    }
    
}

