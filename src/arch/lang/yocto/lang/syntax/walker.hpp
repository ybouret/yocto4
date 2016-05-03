#ifndef YOCTO_LANG_SYNTAX_WALKER_INCLUDED
#define YOCTO_LANG_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/hashing/mph.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class walker : public object
            {
            public:

                virtual ~walker() throw();
                int hash(const string &label) const throw();

                inline void graphviz(const string &filename) const
                {
                    hasher.graphviz(filename);
                }
                
            protected:
                explicit walker(const char *nsdb) throw();
                hashing::mperf hasher;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(walker);
            };

        }
    }
}

#endif
