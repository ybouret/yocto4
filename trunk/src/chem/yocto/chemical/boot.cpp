#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // item
        //
        ////////////////////////////////////////////////////////////////////////
        
        boot:: item::item( const species::pointer &sp, const int weight) :
        spec(sp),
        coef(weight)
        {
            if( weight == 0 )
                throw exception("boot::item(null weight for '%s')", sp->name.c_str());
        }
        
        boot:: item:: ~item() throw()
        {
            
        }
        
        boot:: item:: item( const item &other ) throw() :
        spec(other.spec),
        coef(other.coef)
        {
            
        }
        
        const string & boot:: item:: key() const throw()
        {
            return spec->name;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // constraint
        //
        ////////////////////////////////////////////////////////////////////////
        boot::constraint:: constraint(const double value) throw() :
        sum(value)
        {
        }
        
        boot::constraint:: ~constraint() throw()
        {
        }
        
        boot::constraint & boot::constraint:: weight(const species::pointer &p, const int w)
        {
            
            const item it(p,w);
            if(!insert(it))
                throw exception("boot.constraint(multiple '%s')", p->name.c_str());
            return *this;
        }
        
        std::ostream & operator<<(std::ostream &os, const boot::constraint &cc)
        {
            os << cc.sum << "\t = \t";
            for(boot::constraint::const_iterator i = cc.begin();
                i != cc.end(); ++i)
            {
                const boot::item &it = *i;
                const int     w    = it.coef;
                const string &name = it.spec->name;
                if(w>0)
                {
                    os << " +";
                    os << w;
                    
                }
                if(w<0)
                {
                    os << " -";
                    os << -w;
                }
                os << '*' << '[' << name << ']';
            }
            
            return os;
        }
        
        void boot::constraint:: fill( array<integer_t> &P ) const
        {
            const size_t M = P.size();
            for( const_iterator i=begin();i!=end();++i)
            {
                const item  &it = *i;
                const size_t j  = it.spec->indx;
                if(j<1||j>M)
                    throw exception("invalid index for '%s'", it.spec->name.c_str());
                P[j] = it.coef;
            }
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // boot
        //
        ////////////////////////////////////////////////////////////////////////
        
        boot:: boot(): constraints()
        {
        }
        
        boot:: ~boot() throw()
        {
        }
        
        
        boot::constraint & boot::create(const double value)
        {
            constraint::pointer p( new constraint(value) );
            constraints.push_back(p);
            return *p;
        }
        
        void boot:: conserve(const species::pointer &sp, const double C)
        {
            constraint::pointer p( new constraint(C) );
            p->weight(sp, 1);
            constraints.push_back(p);
        }
        
        void boot:: conserve(const species::pointer &sp1, const species::pointer &sp2, const double C)
        {
            constraint::pointer p( new constraint(C) );
            p->weight(sp1, 1);
            p->weight(sp2, 1);
            constraints.push_back(p);
        }
        
        void boot:: conserve(const species::pointer &sp1, const species::pointer &sp2, const species::pointer &sp3, const double C)
        {
            constraint::pointer p( new constraint(C) );
            p->weight(sp1, 1);
            p->weight(sp2, 1);
            p->weight(sp3, 1);
            constraints.push_back(p);
        }
        
        
        std::ostream & operator<<( std::ostream &os, const boot &ini)
        {
            for(size_t i=1;i<=ini.constraints.size();++i)
            {
                const boot::constraint &cc = *ini.constraints[i];
                os << cc;
                if(i<ini.constraints.size()) os << std::endl;
            }
            return os;
        }
        
        
        bool boot:: electroneutrality( const collection &lib )
        {
            bool has_charge = false;
            for( collection::const_iterator i = lib.begin(); i != lib.end(); ++i )
            {
                if( (**i).z != 0 )
                {
                    has_charge = true;
                    break;
                }
            }
            
            if( has_charge )
            {
                constraint &CC = create(0);
                for( collection::const_iterator i = lib.begin(); i != lib.end(); ++i )
                {
                    const species::pointer &sp = *i;
                    const int               z  = sp->z;
                    if(z!=0)
                        CC.weight(sp, z);
                }
                return true;
            }
            else
                return false;
        }
        
    }
}
