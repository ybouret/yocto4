#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/quick.hpp"

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
                                         const int           w ) throw() :
        spec(sp),
        weight(w)
        {
            assert(w!=0);
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
        
        boot::constraint & boot:: constraint:: weight(const species::ptr &sp, const int w)
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
                    os << " +" << w;
                else
                    os << " -" << -w;
                
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
        
        void boot::loader:: fill( imatrix_t &A, array<double> &B) const throw()
        {
            const size_t Nc = size();
            assert(A.rows  ==Nc);
            assert(B.size()==Nc);
            for(size_t i=1;i<=Nc;++i)
            {
                const constraint &cstr = *(*this)[i];
                B[i] = cstr.value;
                for( constraint::const_iterator j=cstr.begin(); j!=cstr.end();++j)
                {
                    const constituent &cc = **j;
                    const size_t k = cc.spec->indx;
                    const int    w = cc.weight;
                    assert(k>=1);
                    assert(k<=A.cols);
                    A[i][k] = w;
                }
            }
        }
        
        void boot::loader:: find_fixed( array<double> &X0, uvector_t &ifix  ) const
        {
            const size_t M   = X0.size();
            const size_t Nc  = size();
            
            for(size_t i=1;i<=M;++i)
            {
                X0[i] = 0;
            }
            ifix.free();
            
            for(size_t i=1;i<=Nc;++i)
            {
                const constraint &cstr = *(*this)[i];
                if( cstr.size() == 1)
                {
                    const constituent &cc = *cstr.front();
                    const size_t       j  = cc.spec->indx;
                    const int          w  = cc.weight;
                    assert(j>=1);
                    assert(j<=M);
                    assert(w!=0);
                    const double rhs  = cstr.value;
                    const double val  = rhs/w;
                    if(val<0)
                        throw exception("invalid fixed concentration for '%s'", cc.spec->name.c_str());
                    X0[j]    = val;
#if 0
                    for(size_t k=1;k<=ifix.size();++k)
                    {
                        if(ifix[k]==j)
                            throw exception("multiple fixed '%s'", cc.spec->name.c_str());
                    }
#endif
                    if(!ifix.insert(j))
                        throw exception("multiple fixed '%s'", cc.spec->name.c_str());
                        
                }
            }
            //quicksort(ifix);
        }
        
        void boot:: loader:: find_kernel( matrix_t &K, size_t M) const
        {
            const size_t Nc = size();
            size_t       nk = 0;
            for(size_t i=1;i<=Nc;++i)
            {
                const constraint &cstr = *(*this)[i];
                if( cstr.size()>1 && fabs(cstr.value)<=0)
                {
                    ++nk;
                }
            }
            if(nk>0)
            {
                K.make(nk,M);
                size_t ik = 0;
                for(size_t i=1;i<=Nc;++i)
                {
                    const constraint &cstr = *(*this)[i];
                    if( cstr.size()>1 && fabs(cstr.value)<=0)
                    {
                        ++ik; assert(ik<=nk);
                        for( constraint::const_iterator j=cstr.begin(); j!=cstr.end();++j)
                        {
                            const constituent &cc = **j;
                            const size_t k = cc.spec->indx;
                            const int    w = cc.weight;
                            assert(k>=1);
                            assert(k<=K.cols);
                            K[ik][k] = w;
                        }

                    }
                }
            }
        }
        
    }
    
}

