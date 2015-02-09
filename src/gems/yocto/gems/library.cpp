#include "yocto/gems/library.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        library:: library() :
        db(16,as_capacity),
        locked(false)
        {
        }

        library:: ~library() throw()
        {
        }

        properties & library::insert(const string &name, const double mass)
        {
            if(locked)
            {
                throw exception("library is locked!");
            }

            properties::pointer p( new properties(name,mass) );
            if(!db.insert(p))
            {
                throw exception("Multiple properties '%s' or colliding uuid!", name.c_str());
            }
            assert(p->refcount()>0);
            return *p;
        }

    }

}
