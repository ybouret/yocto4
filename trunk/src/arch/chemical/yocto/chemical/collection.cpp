#include "yocto/chemical/collection.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include <iostream>

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
            assert(is_consistent());
            return *p;
        }
        
        species & collection::add( const char *name, const int charge)
        {
            const string id(name);
            return add(id,charge);
        }
        
        void collection:: remove( const string &name )
        {
            if(count>0)
                throw exception("collection.remove(already in use)");
            if(db.remove(name))
            {
                rebuild_indices();
                find_max_name_length();
            }
            assert(is_consistent());
        }
        
        
        void collection:: remove( const char *name )
        {
            const string id(name);
            remove(id);
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
        
        void collection:: find_max_name_length() const throw()
        {
            assert(is_consistent());
            const_iterator i = db.begin();
            const size_t   n = db.size();
            size_t         m = 0;
            for(size_t j=1;j<=n;++j,++i)
            {
                const species &sp = **i;
                if(sp.name.size()>m)
                {
                    m = sp.name.size();
                }
            }
            (size_t&)max_name_length = m;
        }

        void collection:: output( std::ostream &os ) const
        {
            assert(is_consistent());
            os << "{" << std::endl;
            for(const_iterator i=begin();i!=end();++i)
            {
                os << "\t";
                const species &sp = **i;
                os << sp.name;
                const size_t ns = sp.name.size();
                assert(ns<=max_name_length);
                const size_t blanks = max_name_length-ns;
                for(size_t j=0;j<blanks;++j)
                {
                    os << ' ';
                }
                os << " : " << sp.z << std::endl;
            }
            
            os << "}";
        }

        
        size_t collection:: index_of(const string &name) const
        {
            assert(is_consistent());
            return (*this)[name]->indx;
        }
        
        size_t collection:: index_of(const char *name) const
        {
            assert(is_consistent());
            return (*this)[name]->indx;
        }
        
        bool collection:: is_consistent() const throw()
        {
            size_t j=1;
            for(const_iterator i=begin();i!=end();++i,++j)
            {
                if( (*i)->indx != j )
                    return false;
            }
            return true;
        }

        
    }
}