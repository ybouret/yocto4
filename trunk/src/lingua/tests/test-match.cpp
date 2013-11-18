#include "yocto/utest/run.hpp"
#include "yocto/lingua/match.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(match)
{
    
    if(argc>1)
    {
        vector<lingua::match> motifs;
        for(int i=1;i<argc;++i)
        {
            const lingua::match tmp( argv[i] );
            motifs.push_back(tmp);
        }
        
        ios::icstream fp( ios::cstdin );
        string line;
        while(line.clear(),fp.read_line(line)>0)
        {
            std::cerr << "Matching '" << line << "'" << std::endl;
            for( size_t i=1; i <= motifs.size(); ++i )
            {
                lingua::match &m = motifs[i];
                std::cerr << "motif #" << i  << std::endl;
                if(m(line,lingua::match::exactly))
                {
                    const string s = m.last_accepted();
                    std::cerr << "\tmatch exactly <" << s  << ">" << std::endl;
                }
                if(m(line,lingua::match::partly))
                {
                    const string s = m.last_accepted();
                    std::cerr << "\tmatch partly  <" << s  << ">" << std::endl;
                }
            }
            std::cerr << "--------------------" << std::endl;
        }
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
