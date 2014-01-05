#include "yocto/chemical/effector.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        effector:: ~effector() throw()
        {
        }
        
        effector:: effector( const string &id ) :
        name(id),
        factor(1)
        {
        }
        
        const string & effector::key() const throw() { return name; }
        
               
        effectors:: effectors()
        {
            
        }
        
        effectors:: ~effectors() throw()
        {
        }
        
        
        void effectors:: collect( solution &dSdt, double t, double zeta, const solution &S ) const
        {
            assert(dSdt.components==S.components);
            solution rate(S);
            dSdt.ldz();
            
            for( const_iterator i=begin(); i != end(); ++i )
            {
                const effector &eff = **i;
                rate.ldz();
                eff.call(rate, t, zeta,S);
                for( solution::iterator j=rate.begin(),k=dSdt.begin();j!=rate.end();++j,++k)
                {
                    const component &p = (*j);
                    component       &q = (*k);
                    q.concentration += eff.factor * p.concentration;
                }
            }
        }
        
        std::ostream & operator<<( std::ostream &os, const effectors &db)
        {
            os << "________________" << std::endl;
            os << std::endl;
            for( effectors::const_iterator i=db.begin(); i != db.end(); ++i )
            {
                const effector &eff = **i;
                os << "effector '" << eff.name << "'" << std::endl;
            }
            os << "________________";
            return os;
        }

        
        effector       & effectors:: operator[]( const string &id )
        {
            effector::ptr *ppEff  = search(id);
            if(!ppEff)
            {
                throw exception("no effector '%s'", id.c_str());
            }
            return **ppEff;
        }

        const effector       & effectors:: operator[]( const string &id ) const
        {
            const effector::ptr *ppEff  = search(id);
            if(!ppEff)
            {
                throw exception("no const effector '%s'", id.c_str());
            }
            return **ppEff;
        }
        
        
        
    }

}
