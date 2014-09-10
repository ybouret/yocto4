#include "yocto/lingua/pattern/dict.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        
        p_dict:: p_dict() throw() : patterns(), db() {}
        
        p_dict:: ~p_dict() throw() {}
        
        
        void p_dict:: insert( const string &id, pattern *p)
        {
            assert(p);
            patterns.push_back(p);
            try
            {
                const motif m = p;
                if( !db.insert(id,m) )
                    throw exception("lingua::p_dict(multiple '%s')", &id[0] );
            }
            catch(...)
            {
                delete patterns.pop_back();
                throw;
            }
        }
        
        void p_dict:: insert(const char   *id, pattern *p)
        {
            auto_ptr<pattern> q(p);
            const string ID(id);
            insert( ID, q.yield() );
        }
        
        bool p_dict:: has( const string &id ) const throw()
        {
            return db.search(id) != 0;
        }
        
        bool p_dict:: has( const char *id ) const
        {
            const string ID(id);
            return has(ID);
        }
        
        pattern * p_dict:: create( const string &id ) const
        {
            const pattern * const * pp = db.search(id);
            if(!pp)
            {
                throw exception("p_dict::create(no '%s')", &id[0]);
            }
            assert(*pp);
            return (*pp)->clone();
        }

        pattern * p_dict:: create( const char *id) const
        {
            const string ID(id);
            return create(ID);
        }
    }
}


#include "yocto/lingua/pattern/compiler.hpp"

namespace yocto
{
    namespace lingua
    {
        void p_dict::insert( const string &id, const string &expr)
        {
            pattern *p = compile(expr,this);
            insert(id,p);
        }
        
        void p_dict:: insert( const char   *id, const char   *expr)
        {
            const string I(id);
            const string E(expr);
            insert(I,E);
        }

        
    }
}
