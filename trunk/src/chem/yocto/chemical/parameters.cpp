#include "yocto/chemical/parameters.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace chemical
    {
        parameters:: ~parameters() throw()
        {
            lib.decrease();
        }
        
        parameters:: parameters(const library &user_lib,
                                const char    *names[],
                                const size_t   num_names) :
        count(num_names),
        lib(user_lib),
        db(count,as_capacity)
        {
            if(count>0)
            {
                assert(names);
                for(size_t i=0;i<count;++i)
                {
                    const string id = names[i];
                    if(id.length()<=0)
                        throw exception("parameters: empty name #%u", unsigned(i));
                    const size_t the_index = lib.size() + db.size() + 1;
                    if( !db.insert(id,the_index))
                    {
                        throw exception("parameters: multiple name '%s'", id.c_str());
                    }
                }
            }
            
            lib.increase();
        }
        
        parameters::iterator parameters:: begin() const throw()
        {
            return db.begin();
        }
        
        parameters::iterator parameters:: end() const throw()
        {
            return db.end();
        }
        
        
        
        size_t parameters:: operator[](const string &name) const
        {
            const size_t *p = db.search(name);
            if(!p) throw exception("no parameters['%s']",name.c_str());
            return *p;
        }
        
        size_t parameters:: operator[](const char   *name) const
        {
            const string NAME(name);
            return (*this)[NAME];
        }
        
        std::ostream & operator<<( std::ostream &os, const parameters &params)
        {
            os << "<" << std::endl;
            for(parameters::iterator i=params.begin();i!=params.end();++i)
            {
                os << " " << i->key << " @ " << *i << std::endl;
            }
            os << ">";
            return os;
        }
        
        
    }
    
}
