#include "yocto/lang/generator.hpp"
#include "yocto/ios/imstream.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {

        parser * parser::generate( const char *langID, ios::istream &fp, const bool output_files )
        {
            std::cerr << "Generating Parser for " << langID << std::endl;
            generator G(langID,output_files);
            return G.compile(fp);
        }

        parser *parser::generate(const char  *langID,
                                 const char  *data,
                                 const size_t size,
                                 const bool   output_files)
        {
            assert(!(NULL==data&&size>0));
            ios::imstream fp(data,size);
            return generate(langID,fp,output_files);
        }

    }

}

