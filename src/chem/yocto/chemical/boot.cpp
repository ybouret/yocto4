#include "yocto/chemical/boot.hpp"

namespace yocto
{
    namespace chemical
    {
        constraint:: member:: member( const species::pointer &a, const int w ) throw() :
        sp(a),
        weight(w)
        {
            assert(weight!=0);
        }
        
        constraint::member:: ~member() throw() {}
        
        constraint:: member:: member( const member &m ) throw() :
        sp(m.sp),
        weight(m.weight)
        {
        }
        
        constraint  &constraint:: add( const species::pointer &sp, const int weight )
        {
            if(weight)
            {
                for(size_t i=members.size();i>0;--i)
                {
                    member &m = members[i];
                    if(m.sp->name==sp->name)
                    {
                        ((int&)(m.weight)) += weight;
                        return *this;
                    }
                }
                
                const member tmp(sp,weight);
                members.push_back(tmp);
            }
            return *this;
        }
        
        size_t constraint:: count() const throw()
        {
            return members.size();
        }
        
        const constraint::member & constraint:: operator[](size_t i) const throw()
        {
            assert(i>0);
            assert(i<=members.size());
            return members[i];
        }
        
    }
    
}


namespace yocto
{
    namespace chemical
    {
        
        
        constraint:: ~constraint() throw()
        {
        }
        
        
        constraint:: constraint(const double l) throw() :
        level(l),
	members()
        {
        }
    }
    
}


#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        boot:: ~boot() throw()
        {
        }
        
        boot:: boot() throw() : constraints()
        {
        }
        
        
        constraint & boot:: new_constraint(const double level)
        {
            constraint::pointer p( new constraint(level) );
            push_back(p);
            return *p;
        }
        
        void boot:: electroneutrality( const library &lib )
        {
            constraint::pointer p( new constraint(0) );
            for(library::const_iterator i = lib.begin(); i != lib.end(); ++i)
            {
                const species::pointer &sp = *i;
                if(sp->z)
                {
                    p->add(sp,sp->z);
                }
            }
            if(p->count())
            {
                push_back(p);
            }
            
        }
        
        void boot:: output( std::ostream &os ) const
        {
            const size_t nc = size();
            for(size_t i=1;i<=nc;++i)
            {
                const constraint &c = *(*this)[i];
                os << c.level << " = ";
                const size_t nm = c.count();
                for(size_t j=1;j<=nm;++j)
                {
                    const constraint::member &m = c[j];
                    const int w = m.weight;
                    const int a = w<0?-w:w;
                    if(m.weight<0)
                    {
                        os << '-';
                    }
                    else
                    {
                        os << '+';
                    }
                    if(a>1) os << a << '*';
                    os << '[' << m.sp->name << ']';
                }
                if(i<nc)
                    os << std::endl;
            }
        }
        
        
        void boot:: conserve(const double Ctot, const species::pointer &sp)
        {
            if(Ctot<0)
                throw exception("boot.conserve('%s'): negative concentration", sp->name.c_str());
            new_constraint(Ctot).add(sp,1);
        }
        
        void boot:: conserve(const double Ctot, const species::pointer &A, const species::pointer &B)
        {
            if(Ctot<0)
                throw exception("boot.conserve('%s','%s'): negative concentration", A->name.c_str(),B->name.c_str());
            new_constraint(Ctot).add(A,1).add(B,1);
        }
        
        void boot:: conserve( const double Ctot, const species::pointer &A, const species::pointer &B, const species::pointer &C)
        {
            if(Ctot<0)
                throw exception("boot.conserve('%s','%s','%s'): negative concentration", A->name.c_str(),B->name.c_str(),C->name.c_str());
            new_constraint(Ctot).add(A,1).add(B,1).add(C,1);
        }
        
    }
    
}
