#include "yocto/string/base64.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/exception.hpp"

using namespace yocto;

int main(int argc, char *argv[] )
{
    try
    {
        base64::decoder dec;
        ios::icstream   input(  ios::cstdin  );
        ios::ocstream   output( ios::cstdout );
        
        char C = 0;
        while( input.query(C) )
        {
            dec.write(C);
        }
        dec.flush();
        
        while( dec.query(C) )
            output.write(C);
        output.flush();
        
        return 0;
    }
    catch(const exception &e)
    {
    }
    catch(...)
    {
    }
    return -1;
}