#include "yocto/utest/run.hpp"
#include "yocto/crypto/ios/encrypter.hpp"
#include "yocto/crypto/ios/decrypter.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/osstream.hpp"

using namespace yocto;

static inline
void __encipher( ios::ostream &target, ios::istream &source, crypto::ios_cipher &enc)
{
    enc.process(target,source,0);
}

static inline
void __check( crypto::ios_cipher &enc, crypto::ios_cipher &dec, const char *name )
{
    
    std::cerr << std::endl;
    std::cerr << "\t\t<" << name << ">" << std::endl;
   
    string src;
    for( size_t i=1;i<=100;++i)
    {
        
        string       tgt;
        
        {
            ios::imstream source(src);
            ios::osstream target(tgt);
            __encipher(target, source, enc);
            //tgt.output_visible(std::cerr) << std::endl;
        }
        
        string chk;
        {
            ios::imstream source(tgt);
            ios::osstream target(chk);
            __encipher(target,source,dec);
            //chk.output_visible(std::cerr) << std::endl;
        }
        
        if(chk!=src)
            throw exception("Cipher Failure!");
        std::cerr << ".";
        src += char(32 + (i%80));
    }
    std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(ios)
{
    static const char *names[] = {
        "ecb",
        "cbc",
        "ctr",
        "cfb",
        "ofb",
        "pcbc"
    };
    
    const string key = "toto";
    
    for(size_t i=0;i<sizeof(names)/sizeof(names[0]); ++i)
    {
        const char *name = names[i];
        {
            crypto::ios_encrypter enc( crypto::ios128, name, key);
            crypto::ios_decrypter dec( crypto::ios128, name, key);
            __check(enc, dec, name);
        }
        
        {
            crypto::ios_encrypter enc( crypto::ios192, name, key);
            crypto::ios_decrypter dec( crypto::ios192, name, key);
            __check(enc, dec, name);
        }
        
        {
            crypto::ios_encrypter enc( crypto::ios256, name, key);
            crypto::ios_decrypter dec( crypto::ios256, name, key);
            __check(enc, dec, name);
        }
        
        
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
