#include "yocto/hashing/pearson.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/code/utils.hpp"

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

        int I[256];
        for(size_t i=0;i<256;++i) I[i] = -1;
        hashing::Pearson::list_t L;
        L.initialize();

        std::cerr.flush();
        for(size_t i=0,k=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j)
            {
                fprintf(stderr,"%3d ", I[k++]);
            }
            fprintf(stderr, "\n");
        }
        fflush(stderr);

        P.build_from(I);

        for(size_t i=1;i<=min_of<size_t>(1,nw);++i)
        {
            const string &word = words[i];
            const int     hw   = P(word);
            std::cerr << "'" << word << "' => " << hw << std::endl;
        }


    }
    
    
}
YOCTO_UNIT_TEST_DONE()
