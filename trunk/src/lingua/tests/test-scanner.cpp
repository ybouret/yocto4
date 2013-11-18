#include "yocto/utest/run.hpp"
#include "yocto/lingua/lexical/scanner.hpp"

using namespace yocto;
using namespace lingua;

namespace
{
    class Scanner : public lexical::scanner
    {
    public:
        explicit Scanner( size_t &lr) :
        lexical::scanner( "Scanner", lr)
        {
            dict().insert( "DIGIT",       "[:digit:]");
            Y_LEX_FORWARD(*this,"ID",     "[:word:]+");
            Y_LEX_FORWARD(*this,"INT",    "[-+]?{DIGIT}+");
            Y_LEX_FORWARD(*this,"FLT",    "[-+]?{DIGIT}+[.]{DIGIT}*");
            Y_LEX_DISCARD(*this,"BLANKS", "[:blank:]+");
            make("ENDL", "[:endl:]", this, & Scanner::OnEndl);
            no_dict();
        }
        
        virtual ~Scanner() throw() {}
        
        bool OnEndl( const token & ) throw()
        {
            std::cerr << "[ENDL #" << line << "]" << std::endl;
            ++line;
            return false;
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Scanner);
    };
    
}

YOCTO_UNIT_TEST_IMPL(scanner)
{
    
    size_t  line = 1;
    
    Scanner scan(line);
    source  src;
    src.attach_stdin();
    lexemes lxms;
    bool    fctl=false;
    while(true)
    {
        lexeme *lx = scan.get(src, fctl);
        if(lx)
        {
            lxms.push_back(lx);
            std::cerr << "[" << lx->label << "]=<" << *lx << ">" << std::endl;
        }
        if(fctl)
            throw exception("Unexpected Control Lexeme!");
        
        if( !lx )
            break;
    }
    
}
YOCTO_UNIT_TEST_DONE()
