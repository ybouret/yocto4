#ifndef YOCTO_SEEM_COMPILER_INCLUDED
#define YOCTO_SEEM_COMPILER_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace Seem
    {

        typedef lingua::syntax::xnode   vNode;
        typedef shared_ptr<const vNode> vCode;

        class Compiler : public counted_object
        {
        public:
            virtual ~Compiler() throw();
            explicit Compiler(const bool emitFile=false);

            vCode compile(ios::istream &fp);
            vCode compile(const string &s);
            vCode compile(const char   *s);

            vCode load( ios::istream &fp ) const;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);
            arc_ptr<lingua::parser>                impl;
        public:
            arc_ptr<const lingua::syntax::grammar> gram;
        };

    }
}

#endif

