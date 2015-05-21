#include "yocto/lang/generator.hpp"
#include "yocto/ios/imstream.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {

        parser * parser::generate( const char *langID, ios::istream &fp )
        {
            std::cerr << "Generating Parser for " << langID << std::endl;
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

