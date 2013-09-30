#include "yocto/crypto/sc/arc4.hpp"
#include "yocto/z++/zpipe.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include <iostream>

using namespace yocto;

class App : public object
{
public:
    crypto::arc4 cipher;
    zlib::zpipe  zcoder;
    
    App( const memory::ro_buffer &instr ) :
    cipher(instr),
    zcoder( 1024 )
    {
    }
    
    void encode( ios::ostream &target, ios::istream &source )
    {
        zcoder.def(target, source, 9, cipher);
    }
    
    void decode( ios::ostream &target, ios::istream &source)
    {
        zcoder.inf(target, source, cipher);
    }
    
    virtual ~App() throw()
    {
    }
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(App);
};

int main(int argc, char *argv[])
{
    
    try
    {
        bool encode = true;
        if( argc > 1 )
        {
            const string opt = argv[1];
            if( opt == "-d" )
            {
                encode = false;
            }
            else
                throw exception("invalid option");
        }
        
        const string key = "toto";
        auto_ptr<App> app( new App(key) );
        
        ios::icstream source( ios::cstdin );
        ios::ocstream target( ios::cstdout );
        if( encode )
            app->encode(target,source);
        else
            app->decode(target,source);
        return 0;
    }
    catch(const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled Exception" << std::endl;
    }
    return 1;
}