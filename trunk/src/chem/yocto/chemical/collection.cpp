#include "yocto/chemical/collection.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        collection:: collection(size_t n) :
        count(0),
        db(min_of<size_t>(n,8),as_capacity),
        max_name_length(0)
        {
        }
        
        collection:: ~collection() throw()
        {
        }
        
        void collection:: increase() const throw()
        {
            ++( (size_t &)count);
        }
        
        void collection:: decrease() const throw()
        {
            assert(count>0);
            --( (size_t &)count);
        }
        
        size_t collection::size() const throw() { return db.size(); }
        
        species & collection::add( const string &name, const int charge)
        {
            if(count>0)
                throw exception("collection.insert(already in use)");
            
            species::pointer p( new species(name,charge) );
            if(!db.insert(p))
                throw exception("collection.insert(multiple '%s')", name.c_str());
            
            (size_t&)(p->indx) = db.size();
            if(name.size()>max_name_length)
            {
                (size_t&)max_name_length = name.size();
            }
            return *p;
        }
        
        void collection:: remove( const string &name )
        {
            if(count>0)
                throw exception("collection.remove(already in use)");
            if(db.remove(name))
            {
                rebuild_indices();
                
            }
            
        }
        
        species::pointer & collection:: operator[](const string &name)
        {
            species::pointer *pp = db.search(name);
            if(!pp)
                throw exception("no collection['%s']",name.c_str());
            return *pp;
        }
        
        const species::pointer & collection:: operator[](const string &name) const
        {
            const species::pointer *pp = db.search(name);
            if(!pp)
                throw exception("no CONST collection['%s']",name.c_str());
            return *pp;
        }
        
        species::pointer & collection:: operator[](const char *name)
        {
            const string NAME(name);
            return (*this)[NAME];
        }
        
        
        const species::pointer & collection:: operator[](const char *name) const
        {
            const string NAME(name);
            return (*this)[NAME];
        }
        
        collection::iterator collection::begin() throw()
        {
            return db.begin();
        }
        
        collection::iterator collection::end() throw()
        {
            return db.end();
        }
        
        
        collection::const_iterator collection::begin() const throw()
        {
            return db.begin();
        }
        
        collection::const_iterator collection::end() const throw()
        {
            return db.end();
        }
        
        void collection::rebuild_indices() const throw()
        {
            const_iterator i = db.begin();
            const size_t   n = db.size();
            for(size_t j=1;j<=n;++j,++i)
            {
                const species &sp = **i;
                (size_t &)(sp.indx) = j;
            }
        }
        
        
        
    }
}
