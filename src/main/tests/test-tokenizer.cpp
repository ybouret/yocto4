#include "yocto/utest/run.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;


static inline bool is_sep(char C) throw()
{
    return C == ';';
}

YOCTO_UNIT_TEST_IMPL(tokenizer)
{
    
    string        line;
    ios::icstream fp( ios::cstdin );
    
    vector<string> words(16,as_capacity);
    while( fp.gets(line) )
    {
        words.free();
        std::cerr << "'" << line << "'" << std::endl;
        tokenizer::split(words,line, is_sep);
        std::cerr << "words=" << words << " #=" << words.size() << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
