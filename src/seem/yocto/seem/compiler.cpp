#include "yocto/seem/compiler.hpp"
#include "yocto/ios/imstream.hpp"

namespace yocto
{
    namespace Seem
    {
        Compiler:: ~Compiler() throw()
        {
        }

        namespace
        {
            static const char grammar_data[] =
            {
#include "seem.inc"
            };
        }

        Compiler:: Compiler(const bool emitFiles) :
        impl( lingua::parser::generate(grammar_data,sizeof(grammar_data),emitFiles) ),
        gram( & *impl )
        {

        }

        vCode Compiler:: compile(ios::istream &fp)
        {
            impl->restart();
            const vCode ans( impl->parse(fp) );
            return ans;
        }

        vCode Compiler::compile(const string &s)
        {
            ios::imstream fp(s);
            return compile(fp);
        }

        vCode Compiler::compile(const char *s)
        {
            ios::imstream fp(s,length_of(s));
            return compile(fp);
        }

    }
}
