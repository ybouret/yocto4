#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        lexer:: ~lexer() throw() {}

#define Y_LANG_LEXER_CTOR() \
name(id),                   \
line(0),                    \
scan(0),                    \
Root(0),                    \
scanners(2,as_capacity)


        lexer:: lexer(const string &id, const string &root_scanner) :
        Y_LANG_LEXER_CTOR()
        {
            initialize(root_scanner);
        }

        void lexer:: initialize(const string &root_scanner)
        {
            Root = new lexical::scanner(root_scanner,line);
            const p_scanner q(Root);
            if(!scanners.insert(q))
                throw exception("lexer[%s] unexpected scanner<%s> database failure !", name.c_str(), Root->name.c_str());
            scan = Root;
        }

        lexer:: lexer(const char   *id, const char   *root_scanner) :
        Y_LANG_LEXER_CTOR()
        {
            const string rs(root_scanner);
            initialize(rs);
        }

    }
    
}
