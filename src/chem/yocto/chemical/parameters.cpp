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
        
        parameters:: parameters(const library &user_lib) :
        lib(user_lib),
        db(8,as_capacity)
        {
        }
        
        size_t parameters:: count() const throw() { return db.size(); }
        
        void parameters:: append(const string &name)
        {
            const size_t the_index = lib.size() + db.size() + 1;
            if( !db.insert(name, the_index) )
            {
                throw exception("parameters.append(multiple '%s')", name.c_str());
            }
        }
        
        
        void parameters:: append(const char *name)
        {
            const string NAME(name);
            append(NAME);
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


    }
    
}
