#include "yocto/lang/generator.hpp"
#include "yocto/ios/imstream.hpp"

namespace yocto
{
    namespace lang
    {

        parser * parser::generate( const char *langID, ios::istream &fp )
        {
            generator G(langID);
            return G.compile(fp);
        }

        parser *parser::generate(const char  *langID,
                                 const char  *data,
                                 const size_t size)
        {
            assert(!(NULL==data&&size>0));
            ios::imstream fp(data,size);
            return generate(langID,fp);
        }

    }

}

