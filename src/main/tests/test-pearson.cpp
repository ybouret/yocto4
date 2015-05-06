#include "yocto/hashing/pearson.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pearson)
{
    hashing::Pearson P;

    if(argc>1)
    {
        vector<string> words;
        {
            const string   filename = argv[1];
            ios::icstream  fp(filename);
            string        line;
            while( line.clear(), fp.read_line(line)>=0) words.push_back(line);
            std::cerr << "words=" << words << std::endl;
        }
        const size_t nw = words.size();

        for(size_t i=1;i<=nw;++i)
        {
            const string &word = words[i];
            const int     hw   = P(word);
            std::cerr << "'" << word << "' => " << hw << std::endl;
        }


    }


}
YOCTO_UNIT_TEST_DONE()
